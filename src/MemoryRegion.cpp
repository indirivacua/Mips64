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
#include "MemoryRegion.h"

MemoryRegion::MemoryRegion(int size) {
  this->size = size;
  this->data = new BYTE[size];
  this->status = new BYTE[size];
  this->line = new std::string[size/8];
  //std::cout << "MemoryRegion:" << size << " " << this << std::endl;
  checkreads = TRUE;
}

MemoryRegion::~MemoryRegion() {
  delete data;
  delete status;
  delete line;
}

#define DATA_VACANT     0
#define DATA_WRITTEN    1 // see mytpes.h

BOOL MemoryRegion::reset() {
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

int MemoryRegion::readByte(WORD32 addr, BYTE &data) {
  BYTE *ptr = this->data + addr;
  if (status[addr] == DATA_VACANT)
    return DATA_ERR;

  data = *ptr;
  return OK;
}

int MemoryRegion::readHalf(WORD32 addr, WORD16 &data) {
  BYTE *ptr = this->data + addr;
  for (int i = 0; i < 2; ++i)
     if (status[addr + i] == DATA_VACANT)
      return DATA_ERR;
  if (addr % 2)
    return DATA_MISALIGNED;

  data = (*(WORD16 *)(ptr));
  return OK;
}

int MemoryRegion::readWord32(WORD32 addr, WORD32 &data) {
  BYTE *ptr = this->data + addr;
  for (int i = 0; i < 4; ++i)
     if (status[addr + i] == DATA_VACANT)
      return DATA_ERR;
  if (addr % 4)
    return DATA_MISALIGNED;

  data =  (*(WORD32 *)(ptr));
  return OK;
}

int MemoryRegion::readWord64(WORD32 addr, WORD64 &data) {
  BYTE *ptr = this->data + addr;
  data = 0;
  if (checkreads)
    for (int i = 0; i < 8; ++i)
       if (status[addr + i] == DATA_VACANT)
        return DATA_ERR;
  if (addr % 8)
    return DATA_MISALIGNED;

  data = (*(WORD64 *)(ptr));
  return OK;
}


BOOL MemoryRegion::writeByte(WORD32 addr, BYTE b) {
  BYTE *ptr = data + addr;
  *ptr = b;
  status[addr] = DATA_WRITTEN;
  return TRUE;
}

BOOL MemoryRegion::writeHalf(WORD32 addr, WORD16 h) {
  //h = swap16(h);
  BYTE *ptr = data + addr;
  *(WORD16 *)(ptr) = h;
  for (int i = 0; i < 2; ++i)
    status[addr + i] = DATA_WRITTEN;
  return TRUE;
}

BOOL MemoryRegion::writeWord32(WORD32 addr, WORD32 w) {
  //w = swap32(w);
  BYTE *ptr = this->data + addr;
  *(WORD32 *)(ptr) = w;
std::cout << "writeWord32(" << addr << ", " << w<< ") --" << (int) ptr << std::endl;
  for (int i = 0; i < 4; ++i)
    status[addr + i] = DATA_WRITTEN;
  return TRUE;
}

BOOL MemoryRegion::writeWord64(WORD32 addr, WORD64 d) {
  //d = swap64(d);
  BYTE *ptr = data + addr;
  *(WORD64 *)(ptr) = d;
  for (int i = 0; i < 8; ++i)
    status[addr + i] = DATA_WRITTEN;
  return TRUE;
}

BOOL MemoryRegion::getAsciiz(WORD32 addr, BYTE *dst, int size) {
  BYTE *ptr = data + addr;
  while (*ptr && --size) {
    *dst = *ptr;
    ++ptr;
    ++dst;
  }
  *dst = 0;
  return TRUE;
}

BOOL MemoryRegion::isValidAddress(WORD32 addr) {
  return (addr <= size);
}

BOOL MemoryRegion::setAddressDescription(WORD32 addr, const std::string &description) {

  line[addr/STEP] = description;
  return TRUE;
}

void MemoryRegion::disableCheckReads() {
  checkreads = FALSE;
}
