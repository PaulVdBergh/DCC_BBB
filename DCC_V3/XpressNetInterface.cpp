/*
 * XpressNetInterface.cpp
 *
 *  Created on: Oct 9, 2017
 *      Author: paul
 */

#include "XpressNetInterface.h"

#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

//#include <termios.h>
#include <asm/termbits.h>

#include <cstring>

#include "LocDecoder.h"

namespace DCC_V3
{

	XpressNetInterface* XpressNetInterface::getInstance(const char* pDevice)
	{
		map<string, XpressNetInterface*>::iterator it =
		        sm_XpressNetInterfaces.find(pDevice);
		if (it == sm_XpressNetInterfaces.end())
		{
			XpressNetInterface* pNew = new XpressNetInterface(pDevice);
			sm_XpressNetInterfaces[pDevice] = pNew;
			return pNew;
		}
		return it->second;
	}

	XpressNetInterface::XpressNetInterface(const char* pDevice)
			: m_pDevice(pDevice)
	{
		struct termios2 settings;

		m_fdSerial = open(m_pDevice.c_str(), O_RDWR | O_NOCTTY);
		if (m_fdSerial < 0)
		{
			//	TODO Daemon has no terminals, use syslog
			perror("open");
		}

		int r = ioctl(m_fdSerial, TCGETS2, &settings);
		if (r)
		{
			perror("ioctl");
		}

		settings.c_ispeed = settings.c_ospeed = 62500;
		settings.c_cflag &= ~CBAUD;
		settings.c_cflag |= BOTHER;

		settings.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR
		        | ICRNL | IXON);
		settings.c_oflag &= ~OPOST;
		settings.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
		settings.c_cflag &= ~(CSIZE | PARENB);
		settings.c_cflag |= CS8;

		r = ioctl(m_fdSerial, TCSETS2, &settings);
		if (r)
		{
			perror("ioctl");
		}

		m_fdStop = eventfd(0, 0);
		m_thread = thread([this]
		{
			threadFunc();
		});

	}

	XpressNetInterface::~XpressNetInterface()
	{
		uint64_t stop = 1;
		if (sizeof(stop) != write(m_fdStop, &stop, sizeof(stop)))
		{
			exit(-1);
		}
		m_thread.join();

		close(m_fdSerial);
		close(m_fdStop);
	}

	void XpressNetInterface::threadFunc()
	{
		struct epoll_event ev;
		struct epoll_event evlist[5];
		int epfd;
		bool bContinue = true;

		if (-1 == (epfd = epoll_create(5)))
		{
			exit(-1);
		}

		ev.data.fd = m_fdStop;
		ev.events = EPOLLIN;
		if (-1 == (epoll_ctl(epfd, EPOLL_CTL_ADD, m_fdStop, &ev)))
		{
			exit(-1);
		}

		memset(&ev, 0, sizeof(ev));
		ev.data.fd = m_fdSerial;
		ev.events = EPOLLIN;

		if (-1 == (epoll_ctl(epfd, EPOLL_CTL_ADD, m_fdSerial, &ev)))
		{
			exit(-1);
		}

		while(bContinue)
		{
			int notifications = epoll_wait(epfd, evlist, 5, -1);
			if (-1 == notifications)
			{
				if (EINTR == notifications)
				{
					continue;
				}
				else
				{
					//	TODO : better error recovery !!
					bContinue = false;
					continue;
				}
			}

			for (int notification = 0; notification < notifications;
			        notification++)
			{
				if (m_fdStop == evlist[notification].data.fd)
				{
					bContinue = false;
				}
				else if (m_fdSerial == evlist[notification].data.fd)
				{
					//	TODO add XpressNet message handling here
					uint8_t msg[32];
					ssize_t nbrRead = read(m_fdSerial, msg, 1);
					if (nbrRead == -1)
					{
						perror("read m_fdSerial");
					}
					else if (nbrRead != 1)
					{
						perror("read m_fdSerial nbrRead");
					}
					else
					{
						nbrRead = read(m_fdSerial, &msg[1], msg[0] - 1);
						while(nbrRead != msg[0] - 1)
						{
							ssize_t nbr = read(m_fdSerial, &msg[nbrRead], 1);
							if (nbr == 1)
							{
								++nbrRead;
								continue;
							}
						}
						//	Process msg here
						uint8_t check = 0;
						printf("Received %i bytes from address %i : ", msg[0],
						        msg[1]);
						for (uint8_t i = 2; i < msg[0]; i++)
						{
							printf("Ox%02X ", msg[i]);
							check ^= msg[i];
						}
						printf("\n");

						if (check)
						{
							// The message has an invalid checksum.
							// Notify the client and discard message.

							continue;
						}

						// The message has good checksum, so process it.
						switch (msg[2])
						{
							case 0x21:
							{
								switch (msg[3])
								{
									case 0x21://	Command station software-version request
									{
										uint8_t response[7] = {0x07, 0x60, 0x63,
										        0x21, 0x30, 0x00, 0x72};
										response[1] += msg[1];
										for (uint8_t x = 0x40; x != 0;
										        x = x >> 1)
										{
											if (response[1] & x)
											{
												response[1] ^= 0x80;
											}
										}

										ssize_t nbrWritten = write(m_fdSerial,
										        response, response[0]);
										if (nbrWritten == -1)
										{
											perror("Write response");
										}
										break;
									}

									case 0x24://	Command station status request
									{
										uint8_t response[6] = {0x06, 0x60, 0x62,
										        0x22, 0x00, 0x00};
										response[1] += msg[1];
										for (uint x = 0x40; x != 0; x >>= 1)
										{
											if (response[1] & x)
											{
												response[1] ^= 0x80;
											}
										}

										response[5] = response[2] ^ response[3]
										        ^ response[4];
										ssize_t nbrWritten = write(m_fdSerial,
										        response, response[0]);
										if (nbrWritten == -1)
										{
											perror("Write response");
										}
										break;
									}
									default:
									{
										break;
									}
								}
								break;
							}

							case 0xE3:
							{
								switch (msg[3])
								{
									case 0x00://	Locomotive information request
									{
										uint16_t locAddress = (msg[4] << 8)
										        + msg[5];
										Decoder* pDecoder = Decoder::getDecoder(
										        locAddress);
										if (pDecoder == NULL)
										{
											pDecoder = new LocDecoder(
											        locAddress);
										}
										LocDecoder* pLoc =
										        dynamic_cast<LocDecoder*>(pDecoder);
										if (pLoc)
										{
											uint8_t response[8] = {0x08, 0x60,
											        0xE4, 0x00, 0x00, 0x00,
											        0x00, 0x00};
											response[1] += msg[1];

											response[3] = pLoc->getSpeedsteps();
											response[4] = pLoc->getDirection()
											        | pLoc->getSpeed();
											response[5] = pLoc->getLight()
											        | pLoc->getF4()
											        | pLoc->getF3()
											        | pLoc->getF2()
											        | pLoc->getF1();
											response[6] = pLoc->getF5()
											        | pLoc->getF6()
											        | pLoc->getF7()
											        | pLoc->getF8()
											        | pLoc->getF9()
											        | pLoc->getF10()
											        | pLoc->getF11()
											        | pLoc->getF12();
											for (uint8_t i = 2; i < 7; i++)
											{
												response[7] ^= response[i];
											}
											ssize_t nbrWritten = write(
											        m_fdSerial, response,
											        response[0]);
											if (nbrWritten == -1)
											{
												perror("Write response");
											}
										}
										break;
									}

									default:
									{
										break;
									}
								}
								break;
							}

							default:
							{
								break;
							}
						}
					}
				}

			}
		}
	}

	map<string, XpressNetInterface*> XpressNetInterface::sm_XpressNetInterfaces;

} /* namespace DCC_V3 */
