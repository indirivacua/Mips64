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

#include "MemoryRegion.h"

DataMemory::DataMemory(int size) {
  MemoryRegion *mem = new MemoryRegion(size);
  this->registerRegion("data", mem, 0, size);
}

BOOL DataMemory::registerRegion(const std::string &name, MemoryRegion *m, WORD32 addr, int size) { 
  RegionInfo ri;
  ri.name = name;
  ri.mem = m;
  ri.start = addr;
  ri.size = size;

  regions.push_back(ri);
  return TRUE;
}

MemoryRegion *DataMemory::getRegion(WORD32 addr, WORD32 &newaddr) {
  for (std::vector<RegionInfo>::iterator r = regions.begin(); r != regions.end(); ++r) {
    if (r->start <= addr && (r->start + r->size) >= addr) {
      newaddr = addr - r->start;
      return r->mem;
    }
  }
  return NULL;
}

DataMemory::~DataMemory() {
  //delete regions; 
}

BOOL DataMemory::reset() {
 // return regions->reset();
return TRUE;
}

#define DATA_ERR   9  // see pipeline.h
#define DATA_MISALIGNED 17  // see pipeline.h

int DataMemory::readByte(WORD32 addr, BYTE &data) {
  WORD32 regaddr;
  MemoryRegion *mem = this->getRegion(addr, regaddr);
  return mem->readByte(regaddr, data);
}

int DataMemory::readHalf(WORD32 addr, WORD16 &data) {
  WORD32 regaddr;
  MemoryRegion *mem = this->getRegion(addr, regaddr);
  return mem->readHalf(regaddr, data);
}

int DataMemory::readWord32(WORD32 addr, WORD32 &data) {
  WORD32 regaddr;
  MemoryRegion *mem = this->getRegion(addr, regaddr);
  return mem->readWord32(regaddr, data);
}

int DataMemory::readWord64(WORD32 addr, WORD64 &data) {
  WORD32 regaddr;
  MemoryRegion *mem = this->getRegion(addr, regaddr);
  return mem->readWord64(regaddr, data);
}


BOOL DataMemory::writeByte(WORD32 addr, BYTE b) {
  WORD32 regaddr;
  MemoryRegion *mem = this->getRegion(addr, regaddr);
  return mem->writeByte(regaddr, b);
}

BOOL DataMemory::writeHalf(WORD32 addr, WORD16 h) {
  WORD32 regaddr;
  MemoryRegion *mem = this->getRegion(addr, regaddr);
  return mem->writeHalf(regaddr, h);
}

BOOL DataMemory::writeWord32(WORD32 addr, WORD32 w) {
  WORD32 regaddr;
  MemoryRegion *mem = this->getRegion(addr, regaddr);
  return mem->writeWord32(regaddr, w);
}

BOOL DataMemory::writeWord64(WORD32 addr, WORD64 d) {
  WORD32 regaddr;
  MemoryRegion *mem = this->getRegion(addr, regaddr);
  return mem->writeWord64(regaddr, d);
}

BOOL DataMemory::getAsciiz(WORD32 addr, BYTE *dst, int size) {
  WORD32 regaddr;
  MemoryRegion *mem = this->getRegion(addr, regaddr);
  return mem->getAsciiz(regaddr, dst, size);
}

BOOL DataMemory::isValidAddress(WORD32 addr) {
  WORD32 regaddr;
  MemoryRegion *mem = this->getRegion(addr, regaddr);
  if (!mem)
    return false;
  return mem->isValidAddress(regaddr);
}

BOOL DataMemory::setAddressDescription(WORD32 addr, const std::string &description) {
  WORD32 regaddr;
  MemoryRegion *mem = this->getRegion(addr, regaddr);
  return mem->setAddressDescription(regaddr, description);
}
