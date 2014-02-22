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

#include "DataMemory.h"

DataMemory::DataMemory(int) : Memory(size) {
}

DataMemory::~DataMemory() {
}


BYTE DataMemory::readByte(WORD32 addr) {
  return 0;
}

WORD16 DataMemory::readHalf(WORD32 addr) {
  return 0;
}

WORD32 DataMemory::readWord32(WORD32 addr) {
  return 0;
}

WORD64 DataMemory::readWord64(WORD32 addr) {
  return 0;
}

double DataMemory::readFloat(WORD32 addr) {
  return 0;
}

BOOL DataMemory::writeByte(WORD32 addr, BYTE) {
  return TRUE;
}

BOOL DataMemory::writeHalf(WORD32 addr, WORD16) {
  return TRUE;
}

BOOL DataMemory::writeWord32(WORD32 addr, WORD32) {
  return TRUE;
}

BOOL DataMemory::writeWord64(WORD32 addr, WORD64) {
  return TRUE;
}

double DataMemory::writeFloat(WORD32 addr, double) {
  return TRUE;
}

