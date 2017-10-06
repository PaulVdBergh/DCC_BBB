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
 * File:   Decoder.h
 * Author: Paul Van den Bergh
 *
 * Created on 3 oktober 2017, 15:38
 */

#ifndef DECODER_H
#define DECODER_H

#include <map>
#include <mutex>
using namespace std;

namespace DCC_V3
{

	class Decoder
	{
			friend class SocketInterface;
		public:
			Decoder(const uint16_t& address);
			virtual ~Decoder();

			const uint16_t& getAddress(void)
			{
				return m_Address;
			}

			static Decoder* getDecoder(const uint16_t& address);

		protected:
			uint16_t m_Address;

			static map<uint16_t, Decoder*> sm_Decoders;
			static recursive_mutex sm_MDecoders;
		private:

	};

#endif /* DECODER_H */

} /* namespace DCC_V3 */
