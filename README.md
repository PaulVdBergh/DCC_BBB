# DCC_BBB

This is the repository for the BeagleBone Black sources in the DCC project

The DCC project is an attempt to implement a DCC (Digital Command Control) Command Station based on the BeagleBone Black platform.
The project consists of four parts:
- DCC_BBB (this repo) : the Linux code for the BeagleBone.
- DCC_PRU : the PRU code for generating the actual DCC signal (including RailCom cutout).
- DCC_AVR : the AVR code for additional interfaces (for example Expressnet, RailComDetector etc.).
- DCC_Hardware : the schematics and PCB-layout files for the hardware (KiCad)

The project is created on Debian 64bit:

~$ uname -a

Linux debian 4.9.0-3-amd64 #1 SMP Debian 4.9.30-2+deb9u5 (2017-09-19) x86_64 GNU/Linux

Using

Eclipse IDE for C/C++ Developers

Version: Oxygen.1 Release (4.7.1)
Build id: 20170914-1200

It is cross-compiled for architecture armhf:

~$ dpkg --print-foreign-architectures

armhf

~$ arm-linux-gnueabihf-g++ --version

arm-linux-gnueabihf-g++ (Debian 6.3.0-18) 6.3.0 20170516
Copyright (C) 2016 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

And debugged on BeagleBone Black using:

~$ gdb-multiarch --version

GNU gdb (Debian 7.12-6) 7.12.0.20161007-git
Copyright (C) 2016 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
and "show warranty" for details.
This GDB was configured as "x86_64-linux-gnu".
Type "show configuration" for configuration details.
For bug reporting instructions, please see:
<http://www.gnu.org/software/gdb/bugs/>.
Find the GDB manual and other documentation resources online at:
<http://www.gnu.org/software/gdb/documentation/>.
For help, type "help".
Type "apropos word" to search for commands related to "word".
