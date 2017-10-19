/*
 * XpressNetInterface.h
 *
 *  Created on: Oct 9, 2017
 *      Author: paul
 */

#ifndef XPRESSNETINTERFACE_H_
#define XPRESSNETINTERFACE_H_

#include "Interface.h"

#include <map>
#include <thread>
#include <string>
using namespace std;

namespace DCC_V3
{

	class XpressNetInterface: public Interface
	{
		public:
			static XpressNetInterface* getInstance(const char* pDevice);
			virtual ~XpressNetInterface();

		protected:
			XpressNetInterface(const char* pDevice);

		private:
			string	m_pDevice;
			thread	m_thread;
			int		m_fdStop;
			int		m_fdSerial;

			void threadFunc(void);

			static map<string, XpressNetInterface*>	sm_XpressNetInterfaces;
	};

} /* namespace DCC_V3 */

#endif /* XPRESSNETINTERFACE_H_ */
