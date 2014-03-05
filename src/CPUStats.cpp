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

#include "CPUStats.h"

CPUStats::CPUStats() {
  reset();
}

void CPUStats::reset() {
  cycles = 0;
  instructions = 0;
  loads = 0;
  stores = 0;
  branch_taken_stalls = 0;
  branch_misprediction_stalls = 0;
  raw_stalls = 0;
  waw_stalls = 0;
  war_stalls = 0;
  structural_stalls = 0;
}

