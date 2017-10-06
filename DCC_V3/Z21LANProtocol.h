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
 * File:   Z21LANProtocol.h
 * Author: Paul Van den Bergh
 *
 * Created on 3 oktober 2017, 16:13
 */

#ifndef Z21LANPROTOCOL_H
#define Z21LANPROTOCOL_H

#include <stdint.h>

namespace DCC_V3
{
	//  Queries
	extern const uint8_t LAN_GET_SERIAL_NUMBER[];
	extern const uint8_t LAN_GET_CODE[];
	extern const uint8_t LAN_GET_HWINFO[];
	extern const uint8_t LAN_LOGOFF[];
	extern const uint8_t LAN_X_GET_VERSION[];
	extern const uint8_t LAN_X_GET_STATUS[];
	extern const uint8_t LAN_X_SET_TRACK_POWER_OFF[];
	extern const uint8_t LAN_X_SET_TRACK_POWER_ON[];
	extern const uint8_t LAN_X_DCC_READ_REGISTER[];
	extern const uint8_t LAN_X_CV_READ[];
	extern const uint8_t LAN_X_DCC_WRITE_REGISTER[];
	extern const uint8_t LAN_X_CV_WRITE[];
	extern const uint8_t LAN_X_MM_WRITE_BYTE[];
	extern const uint8_t LAN_X_GET_TURNOUT_INFO[];
	extern const uint8_t LAN_X_SET_TURNOUT[];
	extern const uint8_t LAN_X_SET_STOP[];
	extern const uint8_t LAN_X_GET_LOCO_INFO[];
	extern const uint8_t LAN_X_SET_LOCO_DRIVE[];
	extern const uint8_t LAN_X_SET_LOCO_FUNCTION[];
	extern const uint8_t LAN_X_CV_POM_WRITE_BYTE[];
	extern const uint8_t LAN_X_CV_POM_WRITE_BIT[];
	extern const uint8_t LAN_X_CV_POM_READ_BYTE[];
	extern const uint8_t LAN_X_CV_POM_ACCESSORY_WRITE_BYTE[];
	extern const uint8_t LAN_X_CV_POM_ACCESSORY_WRITE_BIT[];
	extern const uint8_t LAN_X_CV_POM_ACCESSORY_READ_BYTE[];
	extern const uint8_t LAN_X_GET_FIRMWARE_VERSION[];
	extern const uint8_t LAN_SET_BROADCASTFLAGS[];
	extern const uint8_t LAN_GET_BROADCASTFLAGS[];
	extern const uint8_t LAN_GET_LOCOMODE[];
	extern const uint8_t LAN_SET_LOCOMODE[];
	extern const uint8_t LAN_GET_TURNOUTMODE[];
	extern const uint8_t LAN_SET_TURNOUTMODE[];
	extern const uint8_t LAN_RMBUS_GETDATA[];
	extern const uint8_t LAN_RMBUS_PROGRAMMODULE[];
	extern const uint8_t LAN_SYSTEMSTATE_GETDATA[];
	extern const uint8_t LAN_RAILCOM_GETDATA[];
	extern const uint8_t LAN_LOCONET_FROM_LAN[];
	extern const uint8_t LAN_LOCONET_DISPATCH_ADDR[];
	extern const uint8_t LAN_LOCONET_DETECTOR[];
	extern const uint8_t LAN_CAN_DETECTOR[];

	//  Responses
	extern const uint8_t LAN_SERIAL_NUMBER[];
	extern const uint8_t LAN_CODE[];
	extern const uint8_t LAN_HWINFO[];
	extern const uint8_t LAN_BROADCASTFLAGS[];
	extern const uint8_t LAN_SYSTEMSTATE[];
	extern const uint8_t LAN_X_BC_STOPPED[];
	extern const uint8_t LAN_X_VERSION[];
	extern const uint8_t LAN_X_STATUS[];
	extern const uint8_t LAN_X_BC_TRACK_POWER_OFF[];
	extern const uint8_t LAN_X_BC_TRACK_POWER_ON[];
	extern const uint8_t LAN_X_FIRMWARE_VERSION[];

} /* namespace DCC_V3 */
#endif /* Z21LANPROTOCOL_H */

