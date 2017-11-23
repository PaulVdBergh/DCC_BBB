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
 * Manager.h
 *
 *  Created on: Nov 23, 2017
 *      Author: paul
 */

#ifndef MANAGER_H_
#define MANAGER_H_

#include <vector>

#include "types.h"
#include "ExploringBB/GPIO.h"

using namespace std;

namespace DCC_V3
{

#define RAILPOWER_PIN	(48)	//	BBB P9_15 $PINS 16 - 0x840/040 - GPIO1_16 - GPIO 48

	class Client;	//	forward declaration

	class Manager
	{
		public:
			Manager();
			virtual ~Manager();

			powerstate_t getPowerState(void) { return m_PowerState; }
			void	setPowerState(const powerstate_t& newstate);

		protected:

		private:
			powerstate_t m_PowerState;

			exploringBB::GPIO*	m_pRailPowerPin;

			vector<Client*> m_Clients;

	};

} /* namespace DCC_V3 */

#endif /* MANAGER_H_ */
