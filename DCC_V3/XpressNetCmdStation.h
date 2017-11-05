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
 * XpressNetCmdStation.h
 *
 *  Created on: Nov 5, 2017
 *      Author: paul
 */

#ifndef XPRESSNETCMDSTATION_H_
#define XPRESSNETCMDSTATION_H_

#include "CommandStation.h"

namespace DCC_V3
{

	class XpressNetCmdStation: public CommandStation
	{
		public:
			XpressNetCmdStation(const uint8_t& address, int& fdserial);
			virtual ~XpressNetCmdStation();

			static XpressNetCmdStation* find(const uint8_t& address, int& fdSerial);

			const uint8_t& 	getAddress(void) { return m_Address; }
			const int& 		getSerial(void) { return m_fdSerial; }

			virtual void 	notifySetStop(void);
			virtual void 	notifyRailPowerOff(void);
			virtual void 	notifyRailPowerOn(void);
			virtual void 	notifyLocInfoChange(const uint16_t& locAddress);

		protected:
			uint8_t	m_Address;
			int		m_fdSerial;

		private:

	};

} /* namespace DCC_V3 */

#endif /* XPRESSNETCMDSTATION_H_ */
