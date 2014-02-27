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
  this->registerRegion(mem, 0, size);
}

BOOL DataMemory::registerRegion(MemoryRegion *m, WORD32 addr, int size) { 
  regions = m; 
  //...
  return TRUE;
}

MemoryRegion *DataMemory::getRegion(WORD32 addr) {
  return regions;
}

DataMemory::~DataMemory() {
  delete regions; 
}

BOOL DataMemory::reset() {
  return regions->reset();
}

#define DATA_ERR   9  // see pipeline.h
#define DATA_MISALIGNED 17  // see pipeline.h

int DataMemory::readByte(WORD32 addr, BYTE &data) {
  MemoryRegion *mem = this->getRegion(addr);
  return mem->readByte(addr, data);
}

int DataMemory::readHalf(WORD32 addr, WORD16 &data) {
  MemoryRegion *mem = this->getRegion(addr);
  return mem->readHalf(addr, data);
}

int DataMemory::readWord32(WORD32 addr, WORD32 &data) {
  MemoryRegion *mem = this->getRegion(addr);
  return mem->readWord32(addr, data);
}

int DataMemory::readWord64(WORD32 addr, WORD64 &data) {
  MemoryRegion *mem = this->getRegion(addr);
  return mem->readWord64(addr, data);
}


BOOL DataMemory::writeByte(WORD32 addr, BYTE b) {
  MemoryRegion *mem = this->getRegion(addr);
  return mem->writeByte(addr, b);
}

BOOL DataMemory::writeHalf(WORD32 addr, WORD16 h) {
  MemoryRegion *mem = this->getRegion(addr);
  return mem->writeHalf(addr, h);
}

BOOL DataMemory::writeWord32(WORD32 addr, WORD32 w) {
  MemoryRegion *mem = this->getRegion(addr);
  return mem->writeWord32(addr, w);
}

BOOL DataMemory::writeWord64(WORD32 addr, WORD64 d) {
  MemoryRegion *mem = this->getRegion(addr);
  return mem->writeWord64(addr, d);
}

BOOL DataMemory::getAsciiz(WORD32 addr, BYTE *dst, int size) {
  MemoryRegion *mem = this->getRegion(addr);
  return mem->getAsciiz(addr, dst, size);
}

BOOL DataMemory::isValidAddress(WORD32 addr) {
  MemoryRegion *mem = this->getRegion(addr);
  return mem->isValidAddress(addr);
}

BOOL DataMemory::setAddressDescription(WORD32 addr, const std::string &description) {
  MemoryRegion *mem = this->getRegion(addr);
  return mem->setAddressDescription(addr, description);
}
