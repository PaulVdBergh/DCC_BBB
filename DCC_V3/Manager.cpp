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
 * Manager.cpp
 *
 *  Created on: Nov 23, 2017
 *      Author: paul
 */

#include "Manager.h"
#include "Client.h"

namespace DCC_V3
{

	Manager::Manager()
	:	m_PowerState(POWER_OFF)
	,	m_pRailPowerPin(new exploringBB::GPIO(RAILPOWER_PIN))
	{
	}

	Manager::~Manager()
	{
		delete m_pRailPowerPin;
	}

	void Manager::setPowerState(const powerstate_t& newState)
	{
		if(m_PowerState != newState)
		{
			m_PowerState = newState;
			m_pRailPowerPin->setValue((POWER_OFF == newState) ? exploringBB::LOW : exploringBB::HIGH);

			for(auto client : m_Clients)
			{
				client->notifyPowerState(m_PowerState);
			}
		}
	}

} /* namespace DCC_V3 */
