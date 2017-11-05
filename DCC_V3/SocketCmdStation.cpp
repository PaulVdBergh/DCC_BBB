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
 * File:   SocketCmdStation.cpp
 * Author: Paul Van den Bergh
 * 
 * Created on 3 oktober 2017, 15:52
 */

#include "SocketCmdStation.h"

#include "LocDecoder.h"
#include "Z21LANProtocol.h"

#include <algorithm>

namespace DCC_V3
{

	SocketCmdStation::SocketCmdStation(int& sock_me, const struct sockaddr_in& address)
	:	m_sock_me(sock_me), m_Address(address)
	{
	}

	SocketCmdStation::~SocketCmdStation()
	{
	}

	SocketCmdStation* SocketCmdStation::find(int& sock_me, const struct sockaddr_in& address)
	{
		lock_guard<recursive_mutex> guard(sm_MStations);
		auto station = find_if(sm_Stations.begin(), sm_Stations.end(),
		                [&address](CommandStation * pS)
		                {
			                SocketCmdStation* pSS = dynamic_cast<SocketCmdStation*> (pS);
			                if(pSS == NULL)
			                {
				                return(false);
			                }
			                const struct sockaddr_in & otherAddress = pSS->getAddress();
			                return((otherAddress.sin_addr.s_addr == address.sin_addr.s_addr) && (otherAddress.sin_port == address.sin_port));
		                });
		if (station == sm_Stations.end())
		{
			SocketCmdStation* pStation = new SocketCmdStation(sock_me, address);
			sm_Stations.push_back(pStation);
			return pStation;
		}
		return (dynamic_cast<SocketCmdStation*>(*station));
	}

	void SocketCmdStation::notifySetStop()
	{
		sendto(m_sock_me, LAN_X_BC_STOPPED, LAN_X_BC_STOPPED[0], 0, (struct sockaddr*)&m_Address, sizeof(struct sockaddr_in));
	}

	void SocketCmdStation::notifyRailPowerOff()
	{
		sendto(m_sock_me, LAN_X_BC_TRACK_POWER_OFF, LAN_X_BC_TRACK_POWER_OFF[0], 0, (struct sockaddr*)&m_Address, sizeof(struct sockaddr_in));
	}

	void SocketCmdStation::notifyRailPowerOn()
	{
		sendto(m_sock_me, LAN_X_BC_TRACK_POWER_ON, LAN_X_BC_TRACK_POWER_ON[0], 0, (struct sockaddr*)&m_Address, sizeof(struct sockaddr_in));
	}

	void SocketCmdStation::notifyLocInfoChange(const uint16_t& locAddress)
	{
		Decoder* pDecoder = Decoder::getDecoder(locAddress);
		LocDecoder* pLoc = dynamic_cast<LocDecoder*>(pDecoder);
		if(pLoc)
		{
			uint8_t locInfo[14];
			pLoc->getLANLocInfo(locInfo);
			sendto(m_sock_me, locInfo, locInfo[0], 0, (struct sockaddr*)&m_Address, sizeof(struct sockaddr_in));
		}
	}

} /* namespace DCC_V3 */
