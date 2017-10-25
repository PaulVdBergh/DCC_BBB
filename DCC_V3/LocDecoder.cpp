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
 * File:   LocDecoder.cpp
 * Author: Paul Van den Bergh
 * 
 * Created on 3 oktober 2017, 15:57
 */

#include "LocDecoder.h"

#include <cstring>

namespace DCC_V3
{

	LocDecoder::LocDecoder(const uint16_t& address)
			: Decoder(address)
	{
		memset(&m_LocInfo.Addr_MSB, 0, sizeof(m_LocInfo) - 5);
		m_LocInfo.Addr_MSB = (m_Address >> 8) & 0x3F;
		m_LocInfo.Addr_LSB = m_Address & 0xFF;

		memset(&m_LocMode.Addr_MSB, 0, sizeof(m_LocMode) - 4);
		m_LocMode.Addr_MSB = (m_Address >> 8) & 0x3F;
		m_LocMode.Addr_LSB = m_Address & 0xFF;
		m_LocMode.Modus = 0; // DCC Format (if 1 -> MM Format)

		setDirection(true);
		setSpeed(0);

		lock_guard<recursive_mutex> guard(sm_MDecoders);
		sm_Decoders[address] = this;
	}

	LocDecoder::~LocDecoder()
	{
	}

	bool LocDecoder::getDccMessage(uint8_t* pMsg)
	{
		static uint8_t state = 0;

		switch(state)
		{
			case 0:
			{
				//	Speed message
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				pMsg[0] = 4;
				pMsg[1] = m_LocInfo.Addr_LSB;
				pMsg[2] = 0x40;
				if(getDirection()) pMsg[2] |= 0x20;
				pMsg[2] |= getSpeed();
				pMsg[3] = pMsg[1] ^ pMsg[2];
				state++;
				return true;
			}
			case 1:
			{
				//	Function (F0 - F4) Message
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				pMsg[0] = 4;
				pMsg[1] = m_LocInfo.Addr_LSB;
				pMsg[2] = 0x80 | (m_LocInfo.DB4 & 0x1F);
				pMsg[3] = pMsg[1] ^ pMsg[2];
				state++;
				return true;
			}
			default:
			{
				state = 0;
			}
		}
		return false;
	}

	void LocDecoder::getLocInfo(uint8_t* pInfo)
	{
		lock_guard<recursive_mutex> lock(m_MLocInfo);
		m_LocInfo.XOR = 0;
		for (uint8_t index = 4; index < 13; index++)
		{
			m_LocInfo.XOR ^= ((uint8_t*)&m_LocInfo)[index];
		}
		memcpy(pInfo, &m_LocInfo, sizeof(m_LocInfo));
	}

	void LocDecoder::getLocMode(uint8_t* pMode)
	{
		lock_guard<recursive_mutex> lock(m_MLocMode);
		memcpy(pMode, &m_LocMode, *(uint8_t*)&m_LocMode.DataLen);
	}

} /* namespace DCC_V3 */
