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
 * File:   Decoder.cpp
 * Author: Paul Van den Bergh
 * 
 * Created on 3 oktober 2017, 15:38
 */

#include "Decoder.h"

#include <algorithm>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

namespace DCC_V3
{

	Decoder::Decoder(const uint16_t& address)
			: m_Address(address)
	{
	}

	Decoder::~Decoder()
	{
	}

	bool Decoder::getDccMessage(uint8_t* pMsg)
	{
		return false;
	}

	Decoder* Decoder::getDecoder(const uint16_t& address)
	{
		lock_guard<recursive_mutex> lock(sm_MDecoders);
		map<uint16_t, Decoder*>::iterator it = sm_Decoders.find(address);
		if (it == sm_Decoders.end())
		{
			return (NULL);
		}
		return (it->second);
	}

	void Decoder::threadFunc()
	{
		uint8_t DCCMessage[32];
		int fd;
		fd = open("/dev/rpmsg_pru30", O_RDWR);
		if (fd < 0)
		{
			perror("/dev/rpmsg_pru30");
		}

		while(true)
		{
			if (!sm_Decoders.empty())
			{
				for (auto decoderpair : sm_Decoders)
				{
					if (decoderpair.second)
					{
						if (decoderpair.second->getDccMessage(DCCMessage))
						{
							write(fd, DCCMessage, DCCMessage[0]);
						}
					}
				}
			}
			else
			{
				sleep(1);
			}
		}
	}

	thread Decoder::m_thread = thread([] {threadFunc();});
	map<uint16_t, Decoder*> Decoder::sm_Decoders;
	recursive_mutex Decoder::sm_MDecoders;

} /* namespace DCC_V3 */
