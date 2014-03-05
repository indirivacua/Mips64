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

#include "mytypes.h"
#include "utils.h"
#include "NullRegion.h"

BOOL NullRegion::reset() {
  return TRUE;
}

int NullRegion::readByte(WORD32 addr, BYTE &data) {
  return DATA_ERR;
}

int NullRegion::readHalf(WORD32 addr, WORD16 &data) {
  return DATA_ERR;
}

int NullRegion::readWord32(WORD32 addr, WORD32 &data) {
  return DATA_ERR;
}

int NullRegion::readWord64(WORD32 addr, WORD64 &data) {
  return DATA_ERR;
}


BOOL NullRegion::writeByte(WORD32 addr, BYTE b) {
  return DATA_ERR;
}

BOOL NullRegion::writeHalf(WORD32 addr, WORD16 h) {
  return DATA_ERR;
}

BOOL NullRegion::writeWord32(WORD32 addr, WORD32 w) {
  return DATA_ERR;
}

BOOL NullRegion::writeWord64(WORD32 addr, WORD64 d) {
  return DATA_ERR;
}

BOOL NullRegion::getAsciiz(WORD32 addr, BYTE *dst, int size) {
  return FALSE;
}

BOOL NullRegion::isValidAddress(WORD32 addr) {
  return FALSE; 
}

BOOL NullRegion::setAddressDescription(WORD32 addr, const std::string &description) {
  return FALSE;
}
