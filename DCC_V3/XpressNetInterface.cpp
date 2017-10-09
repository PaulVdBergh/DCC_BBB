/*
 * XpressNetInterface.cpp
 *
 *  Created on: Oct 9, 2017
 *      Author: paul
 */

#include "XpressNetInterface.h"

#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <unistd.h>

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
	}

	void XpressNetInterface::threadFunc()
	{
		struct epoll_event ev;
		struct epoll_event evlist[5];
		int epfd;
		bool bContinue = true;

		if(-1 == (epfd = epoll_create(5)))
		{
			exit(-1);
		}

		ev.data.fd = m_fdStop;
		ev.events = EPOLLIN;
		if(-1 == (epoll_ctl(epfd, EPOLL_CTL_ADD, m_fdStop, &ev)))
		{
			exit(-1);
		}

		while(bContinue)
		{
			int notifications = epoll_wait(epfd, evlist, 5, -1);
			if(-1 == notifications)
			{
				if(EINTR == notifications)
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

			for(int notification = 0; notification < notifications; notification++)
			{
				if(m_fdStop == evlist[notification].data.fd)
				{
					bContinue = false;
				}

			}
		}
	}

	map<string, XpressNetInterface*> XpressNetInterface::sm_XpressNetInterfaces;

} /* namespace DCC_V3 */
