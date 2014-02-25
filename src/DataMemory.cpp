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

#include <iostream>

#include "mytypes.h"
#include "utils.h"
#include "DataMemory.h"

DataMemory::DataMemory(int size) {
  this->size = size;
  this->data = new BYTE[size];
  this->status = new BYTE[size];
  this->line = new std::string[size/8];
  std::cout << "DataMemory:" << size << " " << this << std::endl;
}

DataMemory::~DataMemory() {
  delete data;
  delete status;
  delete line;
}

#define DATA_VACANT     0
#define DATA_WRITTEN    1 // see mytpes.h

BOOL DataMemory::reset() {
  unsigned int i;
  for (i = 0; i < size; i++) {
    this->status[i] = DATA_VACANT;
    this->data[i] = 0;
  }
  for (i = 0; i < size/8; i++) {
    this->line[i] = "";
  }
  return TRUE;
}

#define DATA_ERR   9  // see pipeline.h
#define DATA_MISALIGNED 17  // see pipeline.h

int DataMemory::readByte(WORD32 addr, BYTE &data) {
  if (status[addr] == DATA_VACANT)
    return DATA_ERR;

  data = *(BYTE *)(&this->data[addr]);
  return OK;
}

int DataMemory::readHalf(WORD32 addr, WORD16 &data) {
  for (int i=0; i < 2; ++i)
     if (status[addr + i] == DATA_VACANT)
      return DATA_ERR;
  if (addr % 2)
    return DATA_MISALIGNED;

  data = (*(WORD16 *)(&this->data[addr]));
  return OK;
}

int DataMemory::readWord32(WORD32 addr, WORD32 &data) {
  for (int i=0; i < 4; ++i)
     if (status[addr + i] == DATA_VACANT)
      return DATA_ERR;
  if (addr % 4)
    return DATA_MISALIGNED;

  data =  (*(WORD32 *)(&this->data[addr]));
  return OK;
}

int DataMemory::readWord64(WORD32 addr, WORD64 &data) {
  for (int i=0; i < 8; ++i)
     if (status[addr + i] == DATA_VACANT)
      return DATA_ERR;
  if (addr % 8)
    return DATA_MISALIGNED;

  data = (*(WORD64 *)(&this->data[addr]));
  return OK;
}


BOOL DataMemory::writeByte(WORD32 addr, BYTE b) {
  *(BYTE *)(&this->data[addr]) = b;
  status[addr] = DATA_WRITTEN;
  return TRUE;
}

BOOL DataMemory::writeHalf(WORD32 addr, WORD16 h) {
  //h = swap16(h);
  *(WORD16 *)(&this->data[addr]) = h;
  for (int i = 0; i < 2; ++i)
    status[addr + i] = DATA_WRITTEN;
  return TRUE;
}

BOOL DataMemory::writeWord32(WORD32 addr, WORD32 w) {
  //w = swap32(w);
  *(WORD32 *)(&data[addr]) = w;
  for (int i = 0; i < 4; ++i)
    status[addr + i] = DATA_WRITTEN;
  return TRUE;
}

BOOL DataMemory::writeWord64(WORD32 addr, WORD64 d) {
  //d = swap64(d);
  *(WORD64 *)(&data[addr]) = d;
  for (int i = 0; i < 8; ++i)
    status[addr + i] = DATA_WRITTEN;
  return TRUE;
}

BOOL DataMemory::getAsciiz(WORD32 addr, BYTE *dst, int size) {
  BYTE *ptr = data + addr;
  while (*ptr && --size) {
    *dst = *ptr;
    ++ptr;
    ++dst;
  }
  *dst = 0;
  return TRUE;
}

BOOL DataMemory::isValidAddress(WORD32 addr) {
  return (addr <= size);
}

BOOL DataMemory::setAddressDescription(WORD32 addr, const std::string &description) {

  line[addr/STEP] = description;
  return TRUE;
}
