/*

Mips64 - A portable WinMIPS64 wannabe replacement

Copyright (C) 2003-2013 Mike Scott <mscott@indigo.ie>
Copyright (C) 2014 Andoni Zubimendi <andoni.zubimendi@gmail.com>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation version 2.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

*/

#ifndef __CPUCONFIG_H
#define __CPUCONFIG_H


#include "mytypes.h"

class CPUConfig {

public:
  CPUConfig();

  int codesize;
  int datasize;
  unsigned int ADD_LATENCY;
  unsigned int MUL_LATENCY;
  unsigned int DIV_LATENCY;
  BOOL delay_slot;
  BOOL branch_target_buffer;
  BOOL forwarding;
};

#endif
