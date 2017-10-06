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
 * File:   Decoder.cpp
 * Author: Paul Van den Bergh
 * 
 * Created on 3 oktober 2017, 15:38
 */

#include "Decoder.h"

#include <algorithm>

namespace DCC_V3
{

	Decoder::Decoder(const uint16_t& address)
			: m_Address(address)
	{
	}

	Decoder::~Decoder()
	{
	}

	Decoder* Decoder::getDecoder(const uint16_t& address)
	{
		lock_guard<recursive_mutex> lock(sm_MDecoders);
		map<uint16_t, Decoder*>::iterator it = sm_Decoders.find(address);
		if (it == sm_Decoders.end())
		{
			return (NULL);
		}
		return (it->second);
	}

	map<uint16_t, Decoder*> Decoder::sm_Decoders;
	recursive_mutex Decoder::sm_MDecoders;

} /* namespace DCC_V3 */
