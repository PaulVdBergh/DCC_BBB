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

namespace DCC_V3
{

	SocketCmdStation::SocketCmdStation(int& sock_me, const struct sockaddr_in& address)
	:	m_sock_me(sock_me), m_Address(address)
	{
	}

	SocketCmdStation::~SocketCmdStation()
	{
	}

} /* namespace DCC_V3 */
