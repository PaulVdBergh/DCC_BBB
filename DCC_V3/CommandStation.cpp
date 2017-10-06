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
 * File:   CommandStation.cpp
 * Author: Paul Van den Bergh
 * 
 * Created on 3 oktober 2017, 15:30
 */

#include "CommandStation.h"
#include "SocketCmdStation.h"
#include "Z21LANProtocol.h"

#include <cstring>
#include <algorithm>

namespace DCC_V3
{

	CommandStation::CommandStation()
	{
		m_pLan_BroadcastFlags = new uint8_t[LAN_BROADCASTFLAGS[0]];
		memcpy(m_pLan_BroadcastFlags, LAN_BROADCASTFLAGS,
		        LAN_BROADCASTFLAGS[0]);
	}

	CommandStation::~CommandStation()
	{
		delete[] m_pLan_BroadcastFlags;
	}

	const uint32_t& CommandStation::getBroadcastFlags()
	{
		return *((uint32_t*) &m_pLan_BroadcastFlags[4]);
	}
	;

	void CommandStation::setBroadcastFlags(const uint32_t& flags)
	{
		*((uint32_t*) &m_pLan_BroadcastFlags[4]) = flags;
	}
	;

	const uint8_t* CommandStation::getLAN_BROADCASTFLAGS()
	{
		return m_pLan_BroadcastFlags;
	}
	;

	CommandStation* CommandStation::find(int& sock_me,
	        const struct sockaddr_in& address)
	{
		lock_guard<recursive_mutex> guard(sm_MStations);
		auto station =
		        find_if(sm_Stations.begin(), sm_Stations.end(),
		                [&address](CommandStation * pS)
		                {
			                SocketCmdStation* pSS = dynamic_cast<SocketCmdStation*> (pS);
			                if(pSS == NULL)
			                {
				                return(false);
			                }
			                const struct sockaddr_in & otherAddress = pSS->getAddress();
			                return(
					                (otherAddress.sin_addr.s_addr == address.sin_addr.s_addr)
					                &&
					                (otherAddress.sin_port == address.sin_port)
			                );
		                });
		if (station == sm_Stations.end())
		{
			SocketCmdStation* pStation = new SocketCmdStation(sock_me, address);
			sm_Stations.push_back(pStation);
			return pStation;
		}
		return (*station);
	}

	void CommandStation::replyAll(const uint32_t& flags, const uint8_t* payload)
	{
		vector<CommandStation*>::iterator it = sm_Stations.begin();
		while (it != sm_Stations.end())
		{
			if (flags & (*it)->getBroadcastFlags())
			{
				SocketCmdStation* pSS = dynamic_cast<SocketCmdStation*>(*it);
				if (pSS)
				{
					sendto(pSS->getMySocket(), payload, payload[0], 0,
					        (struct sockaddr*) &(pSS->getAddress()),
					        sizeof(struct sockaddr_in));
				}
			}
			it++;
		}
	}

	vector<CommandStation*> CommandStation::sm_Stations;
	recursive_mutex CommandStation::sm_MStations;

} /* namespace DCC_V3 */
