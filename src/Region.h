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

#ifndef __REGION_H
#define __REGION_H

#define DATA_ERR   9  // see pipeline.h
#define DATA_MISALIGNED 17  // see pipeline.h


class Region {

public:
  virtual BOOL reset() = 0;

  virtual BOOL setAddressDescription(WORD32 addr, const std::string &description) = 0;

  virtual int readByte(WORD32 addr, BYTE &) = 0;
  virtual int readHalf(WORD32 addr, WORD16 &) = 0;
  virtual int readWord32(WORD32 addr, WORD32 &) = 0;
  virtual int readWord64(WORD32 addr, WORD64 &) = 0;

  virtual BOOL writeByte(WORD32 addr, BYTE) = 0;
  virtual BOOL writeHalf(WORD32 addr, WORD16) = 0;
  virtual BOOL writeWord32(WORD32 addr, WORD32) = 0;
  virtual BOOL writeWord64(WORD32 addr, WORD64) = 0;

  virtual BOOL getAsciiz(WORD32 addr, BYTE *dst, int size) = 0;

  virtual BOOL isValidAddress(WORD32 addr) = 0;
  virtual WORD32 getSize() const = 0;

};

#endif
