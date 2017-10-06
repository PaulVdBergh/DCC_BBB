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
 * File:   CommandStation.h
 * Author: Paul Van den Bergh
 *
 * Created on 3 oktober 2017, 15:30
 */

#ifndef COMMANDSTATION_H
#define COMMANDSTATION_H

#include <arpa/inet.h>
#include <vector>
#include <mutex>
using namespace std;

namespace DCC_V3
{

	class CommandStation
	{
		public:
			CommandStation();
			virtual ~CommandStation();

			const uint32_t& getBroadcastFlags(void);
			void setBroadcastFlags(const uint32_t& flags);
			const uint8_t* getLAN_BROADCASTFLAGS(void);

			static CommandStation* find(int& sock_me,
			        const struct sockaddr_in& address);
			static void replyAll(const uint32_t& flags, const uint8_t* payload);

		protected:
			uint8_t* m_pLan_BroadcastFlags;

		private:
			static vector<CommandStation*> sm_Stations;
			static recursive_mutex sm_MStations;

	};
} /* namespace DCC_V3 */

#endif /* COMMANDSTATION_H */

