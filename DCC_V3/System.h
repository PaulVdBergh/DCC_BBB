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
 * File:   System.h
 * Author: Paul Van den Bergh.
 *
 * Created on 4 november 2017, 20:08
 */

#ifndef SYSTEM_H
#define SYSTEM_H

#include <stdint.h>

struct SystemState
{
		uint8_t DataLen = 0x14;
		uint8_t filler1 = 0x00;
		uint8_t Header = 0x84;
		uint8_t filler2 = 0x00;
		int16_t MainCurrent = 0x0000;
		int16_t ProgCurrent = 0x0000;
		int16_t FilteredMainCurrent = 0x0000;
		int16_t Temperature = 0x0000;
		uint16_t SupplyVoltage = 0x0000;
		uint16_t VCCVoltage = 0x0000;
		uint8_t CentralState = 0x00;
		uint8_t CentralStateEx = 0x00;
		uint8_t reserved1 = 0x00;
		uint8_t reserved2 = 0x00;
}__attribute__((packed));

extern struct SystemState SystemState;

#endif /* SYSTEM_H */
