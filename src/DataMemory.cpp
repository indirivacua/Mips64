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

#include "Region.h"
#include "MemoryRegion.h"

DataMemory::DataMemory(int size) {
  MemoryRegion *mem = new MemoryRegion(size);
  this->registerRegion("data", mem, 0);
}

DataMemory::~DataMemory() {
  //delete regions;  ??
}

BOOL DataMemory::registerRegion(const std::string &name, Region *m, WORD32 addr) { 
  RegionInfo ri;
  ri.name = name;
  ri.mem = m;
  ri.start = addr;
  ri.size = m->getSize();

  regions.push_back(ri);
  return TRUE;
}

Region *DataMemory::getRegion(WORD32 addr, WORD32 &newaddr) {
  for (std::vector<RegionInfo>::iterator r = regions.begin(); r != regions.end(); ++r) {
    if (r->start <= addr && (r->start + r->size) >= addr) {
      newaddr = addr - r->start;
      return r->mem;
    }
  }
  return &null;
}

BOOL DataMemory::reset() {
  for (std::vector<RegionInfo>::iterator r = regions.begin(); r != regions.end(); ++r) {
   r->mem->reset();
  }
  return TRUE;
}

int DataMemory::readByte(WORD32 addr, BYTE &data) {
  WORD32 regaddr;
  Region *mem = this->getRegion(addr, regaddr);
  return mem->readByte(regaddr, data);
}

int DataMemory::readHalf(WORD32 addr, WORD16 &data) {
  WORD32 regaddr;
  Region *mem = this->getRegion(addr, regaddr);
  return mem->readHalf(regaddr, data);
}

int DataMemory::readWord32(WORD32 addr, WORD32 &data) {
  WORD32 regaddr;
  Region *mem = this->getRegion(addr, regaddr);
  return mem->readWord32(regaddr, data);
}

int DataMemory::readWord64(WORD32 addr, WORD64 &data) {
  WORD32 regaddr;
  Region *mem = this->getRegion(addr, regaddr);
  return mem->readWord64(regaddr, data);
}


BOOL DataMemory::writeByte(WORD32 addr, BYTE b) {
  WORD32 regaddr;
  Region *mem = this->getRegion(addr, regaddr);
  //std::cout << "memory"  << ":" << " writeByte(" << addr << "," << (int) b << ")" << std::endl;
  return mem->writeByte(regaddr, b);
}

BOOL DataMemory::writeHalf(WORD32 addr, WORD16 h) {
  WORD32 regaddr;
  Region *mem = this->getRegion(addr, regaddr);
  return mem->writeHalf(regaddr, h);
}

BOOL DataMemory::writeWord32(WORD32 addr, WORD32 w) {
  WORD32 regaddr;
  Region *mem = this->getRegion(addr, regaddr);
  return mem->writeWord32(regaddr, w);
}

BOOL DataMemory::writeWord64(WORD32 addr, WORD64 d) {
  WORD32 regaddr;
  Region *mem = this->getRegion(addr, regaddr);
  return mem->writeWord64(regaddr, d);
}

BOOL DataMemory::getAsciiz(WORD32 addr, BYTE *dst, int size) {
  WORD32 regaddr;
  Region *mem = this->getRegion(addr, regaddr);
  return mem->getAsciiz(regaddr, dst, size);
}

BOOL DataMemory::isValidAddress(WORD32 addr) {
  WORD32 regaddr;
  Region *mem = this->getRegion(addr, regaddr);
  return mem->isValidAddress(regaddr);
}

BOOL DataMemory::setAddressDescription(WORD32 addr, const std::string &description) {
  WORD32 regaddr;
  Region *mem = this->getRegion(addr, regaddr);
  return mem->setAddressDescription(regaddr, description);
}
