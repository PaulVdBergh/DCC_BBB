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
 * File:   LocDecoder.h
 * Author: Paul Van den Bergh
 *
 * Created on 3 oktober 2017, 15:57
 */

#ifndef LOCDECODER_H
#define LOCDECODER_H

#include"Decoder.h"

namespace DCC_V3
{

	class LocDecoder: public Decoder
	{
		public:
			LocDecoder(const uint16_t& address);
			virtual ~LocDecoder();

			virtual bool getDccMessage(uint8_t* pMsg);

			void getLocInfo(uint8_t* pInfo);
			void getLocMode(uint8_t* pMode);

			bool getBusy()
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				return (m_LocInfo.DB2 & 0x08);
			}
			;

			uint8_t getSpeedsteps()
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				return (m_LocInfo.DB2 & 0x07);
			}
			;

			bool getDirection()
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				return (m_LocInfo.DB3 & 0x80);
			}
			;

			uint8_t getSpeed()
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				return (m_LocInfo.DB3 & 0x7F);
			}
			;

			bool getDualTraction()
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				return (m_LocInfo.DB4 & 0x40);
			}
			;

			bool getSmartSearch()
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				return (m_LocInfo.DB4 & 0x20);
			}
			;

			bool getLight()
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				return (m_LocInfo.DB4 & 0x10);
			}
			;

			bool getF0()
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				return (m_LocInfo.DB4 & 0x10);
			}
			;

			bool getF1()
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				return (m_LocInfo.DB4 & 0x01);
			}
			;

			bool getF2()
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				return (m_LocInfo.DB4 & 0x02);
			}
			;

			bool getF3()
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				return (m_LocInfo.DB4 & 0x04);
			}
			;

			bool getF4()
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				return (m_LocInfo.DB4 & 0x08);
			}
			;

			bool getF5()
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				return (m_LocInfo.DB5 & 0x01);
			}
			;

			bool getF6()
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				return (m_LocInfo.DB5 & 0x02);
			}
			;

			bool getF7()
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				return (m_LocInfo.DB5 & 0x04);
			}
			;

			bool getF8()
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				return (m_LocInfo.DB5 & 0x08);
			}
			;

			bool getF9()
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				return (m_LocInfo.DB5 & 0x10);
			}
			;

			bool getF10()
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				return (m_LocInfo.DB5 & 0x20);
			}
			;

			bool getF11()
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				return (m_LocInfo.DB5 & 0x40);
			}
			;

			bool getF12()
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				return (m_LocInfo.DB5 & 0x80);
			}
			;

			bool getF13()
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				return (m_LocInfo.DB6 & 0x01);
			}
			;

			bool getF14()
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				return (m_LocInfo.DB6 & 0x02);
			}
			;

			bool getF15()
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				return (m_LocInfo.DB6 & 0x04);
			}
			;

			bool getF16()
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				return (m_LocInfo.DB6 & 0x08);
			}
			;

			bool getF17()
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				return (m_LocInfo.DB6 & 0x10);
			}
			;

			bool getF18()
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				return (m_LocInfo.DB6 & 0x20);
			}
			;

			bool getF19()
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				return (m_LocInfo.DB6 & 0x40);
			}
			;

			bool getF20()
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				return (m_LocInfo.DB6 & 0x80);
			}
			;

			bool getF21()
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				return (m_LocInfo.DB7 & 0x01);
			}
			;

			bool getF22()
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				return (m_LocInfo.DB7 & 0x02);
			}
			;

			bool getF23()
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				return (m_LocInfo.DB7 & 0x04);
			}
			;

			bool getF24()
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				return (m_LocInfo.DB7 & 0x08);
			}
			;

			bool getF25()
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				return (m_LocInfo.DB7 & 0x10);
			}
			;

			bool getF26()
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				return (m_LocInfo.DB7 & 0x20);
			}
			;

			bool getF27()
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				return (m_LocInfo.DB7 & 0x40);
			}
			;

			bool getF28()
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				return (m_LocInfo.DB7 & 0x80);
			}
			;

			void setBusy(bool value)
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				value ? (m_LocInfo.DB2 |= 0x08) : (m_LocInfo.DB2 &= ~(0x08));
			}
			;

			void setSpeedsteps(uint8_t value)
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				m_LocInfo.DB2 &= ~(0x07);
				m_LocInfo.DB2 |= (value & 0x07);
			}
			;

			void setLocoDrive(uint8_t value)
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				m_LocInfo.DB3 = value;
			}
			;

			void setDirection(bool value)
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				value ? (m_LocInfo.DB3 |= 0x80) : (m_LocInfo.DB3 &= ~(0x80));
			}
			;

			void setSpeed(uint8_t value)
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				m_LocInfo.DB3 &= ~(0x7F);
				m_LocInfo.DB3 |= (value & 0x7F);
			}
			;

			void setDualTraction(bool value)
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				value ? (m_LocInfo.DB4 |= 0x40) : (m_LocInfo.DB4 &= ~(0x40));
			}
			;

			void setSmartSearch(bool value)
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				value ? (m_LocInfo.DB4 |= 0x20) : (m_LocInfo.DB4 &= ~(0x20));
			}
			;

			void setLight(bool value)
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				value ? (m_LocInfo.DB4 |= 0x10) : (m_LocInfo.DB4 &= ~(0x10));
			}
			;

			void setF0(bool value)
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				value ? (m_LocInfo.DB4 |= 0x10) : (m_LocInfo.DB4 &= ~(0x10));
			}
			;

			void setF1(bool value)
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				value ? (m_LocInfo.DB4 |= 0x01) : (m_LocInfo.DB4 &= ~(0x01));
			}
			;

			void setF2(bool value)
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				value ? (m_LocInfo.DB4 |= 0x02) : (m_LocInfo.DB4 &= ~(0x02));
			}
			;

			void setF3(bool value)
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				value ? (m_LocInfo.DB4 |= 0x04) : (m_LocInfo.DB4 &= ~(0x04));
			}
			;

			void setF4(bool value)
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				value ? (m_LocInfo.DB4 |= 0x08) : (m_LocInfo.DB4 &= ~(0x08));
			}
			;

			void setF5(bool value)
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				value ? (m_LocInfo.DB5 |= 0x01) : (m_LocInfo.DB5 &= ~(0x01));
			}
			;

			void setF6(bool value)
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				value ? (m_LocInfo.DB5 |= 0x02) : (m_LocInfo.DB5 &= ~(0x02));
			}
			;

			void setF7(bool value)
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				value ? (m_LocInfo.DB5 |= 0x04) : (m_LocInfo.DB5 &= ~(0x04));
			}
			;

			void setF8(bool value)
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				value ? (m_LocInfo.DB5 |= 0x08) : (m_LocInfo.DB5 &= ~(0x08));
			}
			;

			void setF9(bool value)
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				value ? (m_LocInfo.DB5 |= 0x10) : (m_LocInfo.DB5 &= ~(0x10));
			}
			;

			void setF10(bool value)
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				value ? (m_LocInfo.DB5 |= 0x20) : (m_LocInfo.DB5 &= ~(0x20));
			}
			;

			void setF11(bool value)
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				value ? (m_LocInfo.DB5 |= 0x40) : (m_LocInfo.DB5 &= ~(0x40));
			}
			;

			void setF12(bool value)
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				value ? (m_LocInfo.DB5 |= 0x80) : (m_LocInfo.DB5 &= ~(0x80));
			}
			;

			void setF13(bool value)
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				value ? (m_LocInfo.DB6 |= 0x01) : (m_LocInfo.DB6 &= ~(0x01));
			}
			;

			void setF14(bool value)
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				value ? (m_LocInfo.DB6 |= 0x02) : (m_LocInfo.DB6 &= ~(0x02));
			}
			;

			void setF15(bool value)
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				value ? (m_LocInfo.DB6 |= 0x04) : (m_LocInfo.DB6 &= ~(0x04));
			}
			;

			void setF16(bool value)
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				value ? (m_LocInfo.DB6 |= 0x08) : (m_LocInfo.DB6 &= ~(0x08));
			}
			;

			void setF17(bool value)
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				value ? (m_LocInfo.DB6 |= 0x10) : (m_LocInfo.DB6 &= ~(0x10));
			}
			;

			void setF18(bool value)
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				value ? (m_LocInfo.DB6 |= 0x20) : (m_LocInfo.DB6 &= ~(0x20));
			}
			;

			void setF19(bool value)
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				value ? (m_LocInfo.DB6 |= 0x40) : (m_LocInfo.DB6 &= ~(0x40));
			}
			;

			void setF20(bool value)
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				value ? (m_LocInfo.DB6 |= 0x80) : (m_LocInfo.DB6 &= ~(0x80));
			}
			;

			void setF21(bool value)
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				value ? (m_LocInfo.DB7 |= 0x01) : (m_LocInfo.DB7 &= ~(0x01));
			}
			;

			void setF22(bool value)
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				value ? (m_LocInfo.DB7 |= 0x02) : (m_LocInfo.DB7 &= ~(0x02));
			}
			;

			void setF23(bool value)
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				value ? (m_LocInfo.DB7 |= 0x04) : (m_LocInfo.DB7 &= ~(0x04));
			}
			;

			void setF24(bool value)
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				value ? (m_LocInfo.DB7 |= 0x08) : (m_LocInfo.DB7 &= ~(0x08));
			}
			;

			void setF25(bool value)
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				value ? (m_LocInfo.DB7 |= 0x10) : (m_LocInfo.DB7 &= ~(0x10));
			}
			;

			void setF26(bool value)
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				value ? (m_LocInfo.DB7 |= 0x20) : (m_LocInfo.DB7 &= ~(0x20));
			}
			;

			void setF27(bool value)
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				value ? (m_LocInfo.DB7 |= 0x40) : (m_LocInfo.DB7 &= ~(0x40));
			}
			;

			void setF28(bool value)
			{
				lock_guard<recursive_mutex> lock(m_MLocInfo);
				value ? (m_LocInfo.DB7 |= 0x80) : (m_LocInfo.DB7 &= ~(0x80));
			}
			;

		protected:

		private:

			struct LocInfo
			{
					uint32_t DataLen = 0x0040000E;
					uint8_t X_Header = 0xEF;
					uint8_t Addr_MSB;
					uint8_t Addr_LSB;
					uint8_t DB2;
					uint8_t DB3;
					uint8_t DB4;
					uint8_t DB5;
					uint8_t DB6;
					uint8_t DB7;
					uint8_t XOR;
			}__attribute__((packed)) m_LocInfo;

			recursive_mutex m_MLocInfo;

			struct LocMode
			{
					uint32_t DataLen = 0x00600007;
					uint8_t Addr_MSB;
					uint8_t Addr_LSB;
					uint8_t Modus;
			}__attribute__((packed)) m_LocMode;

			recursive_mutex m_MLocMode;

	};
} /* namespace DCC_V3 */
#endif /* LOCDECODER_H */

