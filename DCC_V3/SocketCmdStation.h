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
 * File:   SocketCmdStation.h
 * Author: Paul Van den Bergh
 *
 * Created on 3 oktober 2017, 15:52
 */

#ifndef SOCKETCMDSTATION_H
#define SOCKETCMDSTATION_H

#include "CommandStation.h"

namespace DCC_V3
{

	class SocketCmdStation: public CommandStation
	{
		public:
			SocketCmdStation(int& sock_me, const struct sockaddr_in& address);
			virtual ~SocketCmdStation();

			static SocketCmdStation* find(int& sock_me, const struct sockaddr_in& address);

			virtual void notifySetStop(void);
			virtual void notifyRailPowerOff(void);
			virtual void notifyRailPowerOn(void);
			virtual void notifyLocInfoChange(const uint16_t& locAddress);

			const struct sockaddr_in& getAddress(void)
			{
				return m_Address;
			}

			int& getMySocket(void)
			{
				return m_sock_me;
			}

		protected:
			int m_sock_me;
			struct sockaddr_in m_Address;

		private:

	};

} /* namespace DCC_V3 */

#endif /* SOCKETCMDSTATION_H */
