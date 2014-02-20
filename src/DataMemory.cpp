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

#ifndef __DATAMEMORY_H
#define __DATAMEMORY_H

#include "Memory.h"

class DataMemory : public Memory {
public:
  DataMemory(int);
  virtual ~DataMemory();

  BYTE readByte(WORD32 addr);
  WORD16 readHalf(WORD32 addr);
  WORD32 readWord32(WORD32 addr);
  WORD64 readWord64(WORD32 addr);
  double readFloat(WORD32 addr);

  BOOL writeByte(WORD32 addr, BYTE);
  BOOL writeHalf(WORD32 addr, WORD16);
  BOOL writeWord32(WORD32 addr, WORD32);
  BOOL writeWord64(WORD32 addr, WORD64);
  double writeFloat(WORD32 addr, double);

};

#endif
