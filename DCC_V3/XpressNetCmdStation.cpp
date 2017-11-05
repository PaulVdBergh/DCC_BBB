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
 * XpressNetCmdStation.cpp
 *
 *  Created on: Nov 5, 2017
 *      Author: paul
 */

#include "XpressNetCmdStation.h"

#include <algorithm>
#include <unistd.h>

namespace DCC_V3
{

	XpressNetCmdStation::XpressNetCmdStation(const uint8_t& address, int& fdserial)
	:	m_Address(address)
	,	m_fdSerial(fdserial)
	{
		// TODO Auto-generated constructor stub
	}

	XpressNetCmdStation::~XpressNetCmdStation()
	{
		// TODO Auto-generated destructor stub
	}

	XpressNetCmdStation* XpressNetCmdStation::find(const uint8_t& address, int& fdSerial)
	{
		lock_guard<recursive_mutex> lock(sm_MStations);
		auto station = find_if(sm_Stations.begin(), sm_Stations.end(),
				[&address, &fdSerial](CommandStation* pS)
				{
					XpressNetCmdStation* pXS = dynamic_cast<XpressNetCmdStation*>(pS);
					if(pXS == NULL)
					{
						return false;
					}
					return((pXS->getSerial() == fdSerial) && (pXS->getAddress() == address));
				});
		if(station == sm_Stations.end())
		{
			XpressNetCmdStation* pStation = new XpressNetCmdStation(address, fdSerial);
			sm_Stations.push_back(pStation);
			return pStation;
		}
		return (dynamic_cast<XpressNetCmdStation*>(*station));
	}

	void XpressNetCmdStation::notifySetStop(void)
	{
		uint8_t msg[] = {0x05, 0x60, 0x81, 0x00, 0x81};
		write(m_fdSerial, msg, msg[0]);
	}

	void XpressNetCmdStation::notifyRailPowerOff(void)
	{
		uint8_t msg[] = {0x05, 0x60, 0x61, 0x00, 0x61};
		write(m_fdSerial, msg, msg[0]);
	}

	void XpressNetCmdStation::notifyRailPowerOn(void)
	{
		uint8_t msg[] = {0x05, 0x60, 0x61, 0x01, 0x60};
		write(m_fdSerial, msg, msg[0]);
	}

	void XpressNetCmdStation::notifyLocInfoChange(const uint16_t& locAddress)
	{
	}

} /* namespace DCC_V3 */
