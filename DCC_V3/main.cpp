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
 * File:   main.cpp
 * Author: Paul Van den Bergh
 *
 * Created on 3 oktober 2017, 10:56
 */

#include <cstdlib>
using namespace std;

#include "SocketInterface.h"
#include "System.h"
#include "XpressNetInterface.h"
using namespace DCC_V3;

#include <sys/stat.h>
#include <fcntl.h>

/*
 * 
 */
int main(int argc, char** argv)
{
#ifdef DEAMON

	int maxfd;
	int fd;

	switch (fork())
	{
		case -1:
		{
			return -1;
		}
		case 0:
		{
			break;
		}
		default:
		{
			_exit(EXIT_SUCCESS);
		}
	}

	if (setsid() == -1)
	return -1;

	switch (fork())
	{
		case -1:
		{
			return -1;
		}
		case 0:
		{
			break;
		}
		default:
		{
			_exit(EXIT_SUCCESS);
		}
	}

	umask(0);
	chdir("/");

	maxfd = sysconf(_SC_OPEN_MAX);
	if (maxfd == -1)
	{
		maxfd = 8192;
	}

	for (fd = 0; fd < maxfd; fd++)
	{
		close(fd);
	}

	close(STDIN_FILENO);
	fd = open("/dev/null", O_RDWR);
	dup2(STDIN_FILENO, STDOUT_FILENO);
	dup2(STDIN_FILENO, STDERR_FILENO);

#endif // DEAMON

	SocketInterface* pSocketInterface = SocketInterface::getInstance(21105);
	XpressNetInterface* pXpressNetInterface = XpressNetInterface::getInstance("/dev/ttyO1");
	while(1)
	{
		sleep(100);
	}
	delete pXpressNetInterface;
	delete pSocketInterface;

	return 0;
}
