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
#include "System.h"
#include "Z21LANProtocol.h"

#include <cstring>

namespace DCC_V3
{

	CommandStation::CommandStation()
	{
		m_pLan_BroadcastFlags = new uint8_t[LAN_BROADCASTFLAGS[0]];
		memcpy(m_pLan_BroadcastFlags, LAN_BROADCASTFLAGS, LAN_BROADCASTFLAGS[0]);
	}

	CommandStation::~CommandStation()
	{
		delete[] m_pLan_BroadcastFlags;
	}

	const uint32_t& CommandStation::getBroadcastFlags()
	{
		return *((uint32_t*)&m_pLan_BroadcastFlags[4]);
	}

	void CommandStation::setBroadcastFlags(const uint32_t& flags)
	{
		*((uint32_t*)&m_pLan_BroadcastFlags[4]) = flags;
	}

	const uint8_t* CommandStation::getLAN_BROADCASTFLAGS()
	{
		return m_pLan_BroadcastFlags;
	}

	void CommandStation::notifyAllStop()
	{
		SystemState.CentralState |= 0x01;
		lock_guard<recursive_mutex> lock(sm_MStations);
		for(auto pStation : sm_Stations)
		{
			pStation->notifySetStop();
		}
	}

	void CommandStation::notifyAllRailPowerOff()
	{
		SystemState.CentralState |= 0x02;
		lock_guard<recursive_mutex> lock(sm_MStations);
		for(auto pStation : sm_Stations)
		{
			pStation->notifyRailPowerOff();
		}
	}

	void CommandStation::notifyAllRailPowerOn()
	{
		SystemState.CentralState = 0x00;
		lock_guard<recursive_mutex> lock(sm_MStations);
		for(auto pStation : sm_Stations)
		{
			pStation->notifyRailPowerOn();
		}
	}

	void CommandStation::notifyAllLocInfoChange(const uint16_t& locAddress)
	{
		lock_guard<recursive_mutex> lock(sm_MStations);
		for(auto pStation : sm_Stations)
		{
			pStation->notifyLocInfoChange(locAddress);
		}
	}

	vector<CommandStation*> CommandStation::sm_Stations;
	recursive_mutex CommandStation::sm_MStations;

} /* namespace DCC_V3 */
