/*
 * Copyright (C) 2017 Paul Van den Bergh
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/* 
 * File:   SocketInterface.cpp
 * Author: Paul Van den Bergh
 * 
 * Created on 3 oktober 2017, 16:04
 */

#include "SocketInterface.h"

#include "CommandStation.h"
#include "LocDecoder.h"
#include "Z21LANProtocol.h"

#include <algorithm>
#include <sys/epoll.h>
#include <cstring>
#include <iostream>
#include <syslog.h>
using namespace std;

namespace DCC_V3
{

	SocketInterface* SocketInterface::getInstance(const in_port_t& port)
	{
		map<in_port_t, SocketInterface*>::iterator it = sm_SocketInterfaces.find(
		        port);
		if (it == sm_SocketInterfaces.end())
		{
			SocketInterface* pNew = new SocketInterface(port);
			sm_SocketInterfaces[port] = pNew;
			return pNew;
		}
		return it->second;
	}

	SocketInterface::SocketInterface(const in_port_t& port)
			: m_port(port)
	{
		int result;

		m_fdStop = eventfd(0, 0);

		m_sock_me = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

		int optVal = 1;
		result = setsockopt(m_sock_me, SOL_SOCKET, SO_REUSEADDR, &optVal,
		        sizeof(optVal));
		if (-1 == result)
		{
			exit(-1);
		}

		result = setsockopt(m_sock_me, SOL_SOCKET, SO_REUSEPORT, &optVal,
		        sizeof(optVal));
		if (-1 == result)
		{
			exit(-1);
		}

		memset(&m_sockaddr_me, 0, sizeof(struct sockaddr_in));
		m_sockaddr_me.sin_family = AF_INET;
		m_sockaddr_me.sin_port = htons(m_port);
		m_sockaddr_me.sin_addr.s_addr = htonl(INADDR_ANY);
		result = bind(m_sock_me, (struct sockaddr*)&m_sockaddr_me,
		        sizeof(m_sockaddr_me));
		if (-1 == result)
		{
			exit(-1);
		}

		m_thread = thread([this]
		{
			threadFunc();
		});
	}

	SocketInterface::~SocketInterface()
	{
		int result;
		uint64_t stop = 1;
		if (sizeof(stop) != write(m_fdStop, &stop, sizeof(stop)))
		{
			exit(-1);
		}
		m_thread.join();
		result = shutdown(m_sock_me, SHUT_RDWR);
		if (-1 == result)
		{
			cout << __FUNCTION__ << " " << strerror(errno);
		}
		result = close(m_sock_me);
		if (-1 == result)
		{
			cout << __FUNCTION__ << " " << strerror(errno);
		}
	}

	void SocketInterface::threadFunc()
	{
		openlog(NULL, LOG_PID, LOG_MAKEPRI(LOG_NOTICE, LOG_USER));

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
		ev.data.fd = m_sock_me;
		ev.events = EPOLLIN;

		if (-1 == (epoll_ctl(epfd, EPOLL_CTL_ADD, m_sock_me, &ev)))
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
				else if (m_sock_me == evlist[notification].data.fd)
				{
					uint8_t recvbuffer[128];
					struct sockaddr_in si_other;
					socklen_t slen = sizeof(si_other);

					int recv_len = recvfrom(m_sock_me, recvbuffer, 128, 0,
					        (struct sockaddr*)&si_other, &slen);
					uint8_t* payload = recvbuffer;
					while(payload < (recvbuffer + recv_len))
					{
						char msg[256];
						char* location = msg;
						location += sprintf(msg, "msg received : ");
						for (uint8_t ii = 0; ii < payload[0]; ii++)
						{
							location += sprintf(location, " 0x%02X",
							        payload[ii]);
						}
						location += sprintf(location, "   ");
						switch (*(uint32_t*)payload)
						{
							case 0x00100004: //  LAN_GET_SERIAL_NUMBER
							{
								location += sprintf(location,
								        "LAN_GET_SERIAL_NUMBER\n");
								sendto(m_sock_me, LAN_SERIAL_NUMBER,
								        LAN_SERIAL_NUMBER[0], 0,
								        (struct sockaddr*)&si_other,
								        sizeof(si_other));
								break;
							}

							case 0x00180004: //  LAN_GET_CODE
							{
								location += sprintf(location, "LAN_GET_CODE\n");
								sendto(m_sock_me, LAN_CODE, LAN_CODE[0], 0,
								        (struct sockaddr*)&si_other,
								        sizeof(si_other));
								break;
							}

							case 0x001A0004: //  LAN_GET_HWINFO
							{
								location += sprintf(location,
								        "LAN_GET_HWINFO\n");
								sendto(m_sock_me, LAN_HWINFO, LAN_HWINFO[0], 0,
								        (struct sockaddr*)&si_other,
								        sizeof(si_other));
								break;
							}

							case 0x00300004: //  LAN_LOGOFF
							{
								location += sprintf(location, "LAN_LOGOFF\n");
								break;
							}

							case 0x00510004: //  LAN_GET_BROADCASTFLAGS
							{
								location += sprintf(location,
								        "LAN_GET_BROADCASTFLAGS\n");
								const uint8_t* pMsg =
								        (CommandStation::find(m_sock_me,
								                si_other))->getLAN_BROADCASTFLAGS();
								sendto(m_sock_me, pMsg, pMsg[0], 0,
								        (struct sockaddr*)&si_other,
								        sizeof(si_other));
								break;
							}

							case 0x00850004: //  LAN_SYSTEMSTATE_GETDATA
							{
								location += sprintf(location,
								        "LAN_SYSTEMSTATE_GETDATA\n");
								sendto(m_sock_me,
								        &SocketInterface::sm_LAN_SystemState,
								        sizeof(struct SocketInterface::LAN_SystemState),
								        0, (struct sockaddr*)&si_other,
								        sizeof(si_other));
								break;
							}

							case 0x00A20004: //  LAN_LOCONET_FROM_LAN
							{
								location += sprintf(location,
								        "LAN_LOCONET_FROM_LAN\n");
								break;
							}

							case 0x00810005: //  LAN_RMBUS_GETDATA
							{
								location += sprintf(location,
								        "LAN_RMBUS_GETDATA\n");
								break;
							}

							case 0x00820005: //  LAN_RMBUS_PROGRAMMODULE
							{
								location += sprintf(location,
								        "LAN_RMBUS_PROGRAMMODULE\n");
								break;
							}

							case 0x00400006:
							{
								if (*(uint16_t*)&payload[4] == 0x8080) //  LAN_X_SET_STOP
								{
									location += sprintf(location,
									        "LAN_X_SET_STOP\n");
									SocketInterface::sm_LAN_SystemState.CentralState |=
									        0x01;
									CommandStation::replyAll(0x00000001,
									        LAN_X_BC_STOPPED);

									uint8_t locInfo[14];
									lock_guard<recursive_mutex> guard(
									        Decoder::sm_MDecoders);
									map<uint16_t, Decoder*>::iterator it =
									        Decoder::sm_Decoders.begin();
									while(it != Decoder::sm_Decoders.end())
									{
										LocDecoder* pLoc =
										        dynamic_cast<LocDecoder*>(it->second);
										if (pLoc)
										{
											pLoc->setSpeed(0x00);
											pLoc->getLocInfo(locInfo);
											CommandStation::replyAll(0x00000001,
											        locInfo);
										}
										it++;
									}
								}
								break;
							}

							case 0x00600006: //  LAN_GET_LOCMODE
							{
								uint16_t locAddress = (payload[4] << 8)
								        + payload[5];
								Decoder* pDecoder = Decoder::getDecoder(
								        locAddress);
								if (pDecoder == NULL)
								{
									pDecoder = new LocDecoder(locAddress);
								}
								LocDecoder* pLoc =
								        dynamic_cast<LocDecoder*>(pDecoder);
								if (pLoc)
								{
									uint8_t locMode[7];
									pLoc->getLocMode(locMode);
									sendto(m_sock_me, locMode, locMode[0], 0,
									        (struct sockaddr*)&si_other,
									        sizeof(si_other));
								}
								location += sprintf(location,
								        "LAN_GET_LOCMODE  Address = %i\n",
								        locAddress);
								break;
							}

							case 0x00700006: //  LAN_GET_TURNOUTMODE
							{
								location += sprintf(location,
								        "LAN_GET_TURNOUTMODE\n");
								break;
							}

							case 0x00A30006: //  LAN_LOCONET_DISPATCH_ADDR
							{
								location += sprintf(location,
								        "LAN_LOCONET_DISPATCH_ADDR\n");
								break;
							}

							case 0x00400007:
							{
								switch (*(uint16_t*)&payload[4])
								{
									case 0x2121: //  LAN_X_GET_VERSION
									{
										if (payload[6] == 0x00) //  xor check
										{
											location += sprintf(location,
											        "LAN_X_GET_VERSION\n");
											sendto(m_sock_me, LAN_X_VERSION,
											        LAN_X_VERSION[0], 0,
											        (struct sockaddr*)&si_other,
											        sizeof(si_other));
										}
										break;
									}

									case 0x2421: //  LAN_X_GET_STATUS
									{
										if (payload[6] == 0x05)
										{
											location += sprintf(location,
											        "LAN_X_GET_STATUS\n");
											uint8_t status[8];
											memcpy(status, LAN_X_STATUS, 8);
											status[6] =
											        SocketInterface::sm_LAN_SystemState.CentralState;
											status[7] = 0;
											for (uint8_t i = 4; i < 7; i++)
											{
												status[7] ^= status[i];
											}
											sendto(m_sock_me, status, 8, 0,
											        (struct sockaddr*)&si_other,
											        sizeof(si_other));
										}
										break;
									}

									case 0x8021: //  LAN_X_SET_TRACK_POWER_OFF
									{
										if (payload[6] == 0xA1)
										{
											location +=
											        sprintf(location,
											                "LAN_X_SET_TRACK_POWER_OFF\n");
											SocketInterface::sm_LAN_SystemState.CentralState |=
											        0x02;
											CommandStation::replyAll(0x00000001,
											        LAN_X_BC_TRACK_POWER_OFF);
										}
										break;
									}

									case 0x8121: //  LAN_X_SET_TRACK_POWER_ON
									{
										if (payload[6] == 0xA0)
										{
											location +=
											        sprintf(location,
											                "LAN_X_SET_TRACK_POWER_ON\n");
											SocketInterface::sm_LAN_SystemState.CentralState =
											        0x00;
											CommandStation::replyAll(0x00000001,
											        LAN_X_BC_TRACK_POWER_ON);
										}
										break;
									}

									case 0x0AF1: //  LAN_X_GET_FIRMWARE_VERSION
									{
										if (payload[6] == 0xFB)
										{
											location +=
											        sprintf(location,
											                "LAN_X_GET_FIRMWARE_VERSION\n");
											sendto(m_sock_me,
											        LAN_X_FIRMWARE_VERSION,
											        LAN_X_FIRMWARE_VERSION[0],
											        0,
											        (struct sockaddr*)&si_other,
											        sizeof(si_other));
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

							case 0x00610007: //  LAN_SET_LOCOMODE
							{
								location += sprintf(location,
								        "LAN_SET_LOCOMODE\n");
								break;
							}

							case 0x00710007: //  LAN_SET_TURNOUTMODE
							{
								location += sprintf(location,
								        "LAN_SET_TURNOUTMODE\n");
								break;
							}

							case 0x00890007: //  LAN_RAILCOM_GETDATA
							{
								location += sprintf(location,
								        "LAN_RAILCOM_GETDATA\n");
								break;
							}

							case 0x00A40007: //  LAN_LOCONET_DETECTOR
							{
								location += sprintf(location,
								        "LAN_LOCONET_DETECTOR\n");
								break;
							}

							case 0x00C40007: //  LAN_CAN_DETECTOR
							{
								location += sprintf(location,
								        "LAN_CAN_DETECTOR\n");
								break;
							}

							case 0x00400008:
							{
								switch (payload[4])
								{
									case 0x22:
									{
										if (payload[5] == 0x11) //  LAN_X_DCC_READ_REGISTER
										{
											location +=
											        sprintf(location,
											                "LAN_X_DCC_READ_REGISTER\n");

										}
										break;
									}

									case 0x43: //  LAN_X_GET_TURNOUT_INFO
									{
										location += sprintf(location,
										        "LAN_X_GET_TURNOUT_INFO\n");
										break;
									}

									default:
									{
										break;
									}
								}
								break;
							}

							case 0x00500008: //  LAN_SET_BROADCASTFLAGS
							{
								location += sprintf(location,
								        "LAN_SET_BROADCASTFLAGS : 0x%08X\n",
								        (*(uint32_t*)&payload[4]));
								(CommandStation::find(m_sock_me, si_other))->setBroadcastFlags(
								        *(uint32_t*)&payload[4]);
								break;
							}

							case 0x00400009:
							{
								switch (payload[4])
								{
									case 0x23:
									{
										switch (payload[5])
										{
											case 0x11: //  LAN_X_CV_READ
											{
												location += sprintf(location,
												        "LAN_X_CV_READ\n");
												break;
											}

											case 0x12: //  LAN_X_DCC_WRITE_REGISTER
											{
												location +=
												        sprintf(location,
												                "LAN_X_DCC_WRITE_REGISTER\n");
												break;
											}

											default:
											{
												break;
											}
										}
									}

									case 0x53: //  LAN_X_SET_TURNOUT
									{
										location += sprintf(location,
										        "LAN_X_SET_TURNOUT\n");
										break;
									}

									case 0xE3: //  LAN_X_GET_LOCO_INFO
									{
										uint16_t locAddress = ((payload[6]
										        & 0x3F) << 8) + payload[7];
										location +=
										        sprintf(location,
										                "LAN_X_GET_LOCO_INFO  Loc = %i\n",
										                locAddress);
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
											uint8_t infoMessage[14];
											pLoc->getLocInfo(infoMessage);
											CommandStation::replyAll(0x00000001,
											        infoMessage);
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

							case 0x0040000A:
							{
								if (payload[4] == 0xE4)
								{
									uint16_t locAddress = ((payload[6] & 0x3F)
									        << 8) + payload[7];
									Decoder* pDecoder = Decoder::getDecoder(
									        locAddress);
									if (pDecoder == NULL)
									{
										pDecoder = new LocDecoder(locAddress);
									}
									LocDecoder* pLoc =
									        dynamic_cast<LocDecoder*>(pDecoder);
									if (pLoc)
									{
										if (payload[5] == 0xF8) //  LAN_X_SET_LOCO_FUNCTION
										{
											uint8_t FunctionIndex = payload[8]
											        & 0x3F;
											uint8_t Type = payload[8] >> 5;
											location +=
											        sprintf(location,
											                "LAN_X_SET_LOCO_FUNCTION  loc %i function %i\n",
											                locAddress,
											                FunctionIndex);
											switch (FunctionIndex)
											{
												case 0:
												{
													if (!(Type & 0x02))
													{
														pLoc->setF0(
														        Type & 0x01);
													}
													else if (Type == 0x02)
													{
														pLoc->setF0(
														        !pLoc->getF0());
													}
													break;
												}
												case 1:
												{
													if (!(Type & 0x02))
													{
														pLoc->setF1(
														        Type & 0x01);
													}
													else if (Type == 0x02)
													{
														pLoc->setF1(
														        !pLoc->getF1());
													}
													break;
												}
												case 2:
												{
													if (!(Type & 0x02))
													{
														pLoc->setF2(
														        Type & 0x01);
													}
													else if (Type == 0x02)
													{
														pLoc->setF2(
														        !pLoc->getF2());
													}
													break;
												}
												case 3:
												{
													if (!(Type & 0x02))
													{
														pLoc->setF3(
														        Type & 0x01);
													}
													else if (Type == 0x02)
													{
														pLoc->setF3(
														        !pLoc->getF3());
													}
													break;
												}
												case 4:
												{
													if (!(Type & 0x02))
													{
														pLoc->setF4(
														        Type & 0x01);
													}
													else if (Type == 0x02)
													{
														pLoc->setF4(
														        !pLoc->getF4());
													}
													break;
												}
												case 5:
												{
													if (!(Type & 0x02))
													{
														pLoc->setF5(
														        Type & 0x01);
													}
													else if (Type == 0x02)
													{
														pLoc->setF5(
														        !pLoc->getF5());
													}
													break;
												}
												case 6:
												{
													if (!(Type & 0x02))
													{
														pLoc->setF6(
														        Type & 0x01);
													}
													else if (Type == 0x02)
													{
														pLoc->setF6(
														        !pLoc->getF6());
													}
													break;
												}
												case 7:
												{
													if (!(Type & 0x02))
													{
														pLoc->setF7(
														        Type & 0x01);
													}
													else if (Type == 0x02)
													{
														pLoc->setF7(
														        !pLoc->getF7());
													}
													break;
												}
												case 8:
												{
													if (!(Type & 0x02))
													{
														pLoc->setF8(
														        Type & 0x01);
													}
													else if (Type == 0x02)
													{
														pLoc->setF8(
														        !pLoc->getF8());
													}
													break;
												}
												case 9:
												{
													if (!(Type & 0x02))
													{
														pLoc->setF9(
														        Type & 0x01);
													}
													else if (Type == 0x02)
													{
														pLoc->setF9(
														        !pLoc->getF9());
													}
													break;
												}
												case 10:
												{
													if (!(Type & 0x02))
													{
														pLoc->setF10(
														        Type & 0x01);
													}
													else if (Type == 0x02)
													{
														pLoc->setF10(
														        !pLoc->getF10());
													}
													break;
												}
												case 11:
												{
													if (!(Type & 0x02))
													{
														pLoc->setF11(
														        Type & 0x01);
													}
													else if (Type == 0x02)
													{
														pLoc->setF11(
														        !pLoc->getF11());
													}
													break;
												}
												case 12:
												{
													if (!(Type & 0x02))
													{
														pLoc->setF12(
														        Type & 0x01);
													}
													else if (Type == 0x02)
													{
														pLoc->setF12(
														        !pLoc->getF12());
													}
													break;
												}
												case 13:
												{
													if (!(Type & 0x02))
													{
														pLoc->setF13(
														        Type & 0x01);
													}
													else if (Type == 0x02)
													{
														pLoc->setF13(
														        !pLoc->getF13());
													}
													break;
												}
												case 14:
												{
													if (!(Type & 0x02))
													{
														pLoc->setF14(
														        Type & 0x01);
													}
													else if (Type == 0x02)
													{
														pLoc->setF14(
														        !pLoc->getF14());
													}
													break;
												}
												case 15:
												{
													if (!(Type & 0x02))
													{
														pLoc->setF15(
														        Type & 0x01);
													}
													else if (Type == 0x02)
													{
														pLoc->setF15(
														        !pLoc->getF15());
													}
													break;
												}
												case 16:
												{
													if (!(Type & 0x02))
													{
														pLoc->setF16(
														        Type & 0x01);
													}
													else if (Type == 0x02)
													{
														pLoc->setF16(
														        !pLoc->getF16());
													}
													break;
												}
												case 17:
												{
													if (!(Type & 0x02))
													{
														pLoc->setF17(
														        Type & 0x01);
													}
													else if (Type == 0x02)
													{
														pLoc->setF17(
														        !pLoc->getF17());
													}
													break;
												}
												case 18:
												{
													if (!(Type & 0x02))
													{
														pLoc->setF18(
														        Type & 0x01);
													}
													else if (Type == 0x02)
													{
														pLoc->setF18(
														        !pLoc->getF18());
													}
													break;
												}
												case 19:
												{
													if (!(Type & 0x02))
													{
														pLoc->setF19(
														        Type & 0x01);
													}
													else if (Type == 0x02)
													{
														pLoc->setF19(
														        !pLoc->getF19());
													}
													break;
												}
												case 20:
												{
													if (!(Type & 0x02))
													{
														pLoc->setF20(
														        Type & 0x01);
													}
													else if (Type == 0x02)
													{
														pLoc->setF20(
														        !pLoc->getF20());
													}
													break;
												}
												case 21:
												{
													if (!(Type & 0x02))
													{
														pLoc->setF21(
														        Type & 0x01);
													}
													else if (Type == 0x02)
													{
														pLoc->setF21(
														        !pLoc->getF21());
													}
													break;
												}
												case 22:
												{
													if (!(Type & 0x02))
													{
														pLoc->setF22(
														        Type & 0x01);
													}
													else if (Type == 0x02)
													{
														pLoc->setF22(
														        !pLoc->getF22());
													}
													break;
												}
												case 23:
												{
													if (!(Type & 0x02))
													{
														pLoc->setF23(
														        Type & 0x01);
													}
													else if (Type == 0x02)
													{
														pLoc->setF23(
														        !pLoc->getF23());
													}
													break;
												}
												case 24:
												{
													if (!(Type & 0x02))
													{
														pLoc->setF24(
														        Type & 0x01);
													}
													else if (Type == 0x02)
													{
														pLoc->setF24(
														        !pLoc->getF24());
													}
													break;
												}
												case 25:
												{
													if (!(Type & 0x02))
													{
														pLoc->setF25(
														        Type & 0x01);
													}
													else if (Type == 0x02)
													{
														pLoc->setF25(
														        !pLoc->getF25());
													}
													break;
												}
												case 26:
												{
													if (!(Type & 0x02))
													{
														pLoc->setF26(
														        Type & 0x01);
													}
													else if (Type == 0x02)
													{
														pLoc->setF26(
														        !pLoc->getF26());
													}
													break;
												}
												case 27:
												{
													if (!(Type & 0x02))
													{
														pLoc->setF27(
														        Type & 0x01);
													}
													else if (Type == 0x02)
													{
														pLoc->setF27(
														        !pLoc->getF27());
													}
													break;
												}
												case 28:
												{
													if (!(Type & 0x02))
													{
														pLoc->setF28(
														        Type & 0x01);
													}
													else if (Type == 0x02)
													{
														pLoc->setF28(
														        !pLoc->getF28());
													}
													break;
												}
												default:
												{
													break;
												}
											}
										}
										else if ((payload[5] & 0xF0) == 0x10) //  LAN_X_SET_LOCO_DRIVE
										{
											location +=
											        sprintf(location,
											                "LAN_X_SET_LOCO_DRIVE  Loc = %i \n",
											                locAddress);
											if (0
											        == SocketInterface::sm_LAN_SystemState.CentralState)
											{
												pLoc->setLocoDrive(payload[8]);
											}

											uint8_t infoMessage[14];
											pLoc->getLocInfo(infoMessage);
											CommandStation::replyAll(0x00000001,
											        infoMessage);
										}
										else
										{
											break;
										}
										uint8_t locInfo[14];
										pLoc->getLocInfo(locInfo);
										CommandStation::replyAll(0x00000001,
										        locInfo);
									}
								}
								else if (payload[4] == 0x24)
								{
									if (payload[5] == 0x12) //  LAN_X_CV_WRITE
									{
										location += sprintf(location,
										        "LAN_X_CV_WRITE\n");

									}
									else if (payload[5] == 0xFF) //  LAN_X_MM_WRITE_BYTE
									{
										location += sprintf(location,
										        "LAN_X_MM_WRITE_BYTE\n");

									}
								}
								break;
							}

							case 0x0040000C:
							{
								switch (*(uint16_t*)&payload[4])
								{
									case 0x30E6:
									{
										if ((payload[8] & 0xFC) == 0xEC) //  LAN_X_CV_POM_WRITE_BYTE
										{
											location +=
											        sprintf(location,
											                "LAN_X_CV_POM_WRITE_BYTE\n");

										}
										else if ((payload[8] & 0xFC) == 0xE8)
										{
											if (payload[10] == 0x00) //  LAN_X_POM_READ_BYTE
											{
												location +=
												        sprintf(location,
												                "LAN_X_POM_READ_BYTE\n");

											}
											else //  LAN_X_POM_WRITE_BIT
											{
												location +=
												        sprintf(location,
												                "LAN_X_POM_WRITE_BIT\n");

											}
										}
										break;
									}

									case 0x31E6:
									{
										if ((payload[8] & 0xFC) == 0xEC) //  LAN_X_CV_POM_ACCESSORY_WRITE_BYTE
										{
											location +=
											        sprintf(location,
											                "LAN_X_CV_POM_ACCESSORY_WRITE_BYTE\n");

										}
										else if ((payload[8] & 0xFC) == 0xE8)
										{
											if (payload[10] == 0x00) //  LAN_X_POM_ACCESSORY_READ_BYTE
											{
												location +=
												        sprintf(location,
												                "LAN_X_POM_ACCESSORY_READ_BYTE\n");
											}
											else //  LAN_X_POM_ACCESSORY_WRITE_BIT
											{
												location +=
												        sprintf(location,
												                "LAN_X_POM_ACCESSORY_WRITE_BIT\n");
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
						syslog(LOG_NOTICE, msg);
						payload += payload[0];
					}
				}
			}
		}
	}

	struct SocketInterface::LAN_SystemState SocketInterface::sm_LAN_SystemState;
	map<in_port_t, SocketInterface*> SocketInterface::sm_SocketInterfaces;

} /* namespace DCC_V3 */
