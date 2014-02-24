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
#include "CodeMemory.h"

#include <iostream>


CodeMemory::CodeMemory(WORD32 size) {
        this->size = size;
	line = new std::string[size/4];
	assembly = new std::string[size/4];
	mnemonic = new std::string[size/4];
        code = new WORD32[size/4];
        status = new BYTE[size/4];
        reset();
}

CodeMemory::~CodeMemory() {
        delete [] code;
        delete [] status;
	delete [] line;
	delete [] assembly;
	delete [] mnemonic;
}

WORD32 CodeMemory::readInstruction(WORD32 addr) const {
	return code[addr/4];
}

BOOL CodeMemory::writeInstruction(WORD32 addr,
                                  WORD32 instr, 
				  const std::string &line,
                                  const std::string &assembly,
                                  const std::string &mnemonic) {
  WORD32 pos = addr / 4;
  
  this->code[pos] = instr;
  this->line[pos] = line;
  this->assembly[pos] = assembly;
  this->mnemonic[pos] = mnemonic;

  return TRUE;
}

BOOL CodeMemory::invalidateInstruction(WORD32 addr) {
  this->status[addr/4] |= CODE_INVALID;
  return TRUE;
}

BOOL CodeMemory::isValidAddress(WORD32 addr) {
	return (addr <= size);
}

BOOL CodeMemory::reset() {
  for (unsigned int i = 0; i < size/4; i++)
    this->status[i] = CODE_VALID;
  return TRUE;
}

BOOL CodeMemory::hasBreakpoint(WORD32 addr) const { 
	return (status[addr] & CODE_BREAKPOINT) == CODE_BREAKPOINT;
}

BOOL CodeMemory::setBreakpoint(WORD32 addr, BOOL state) {
  if (state)
  	status[addr] |= CODE_BREAKPOINT;
  else
  	status[addr] &= (0xff - CODE_BREAKPOINT);
  return TRUE;
}

BOOL CodeMemory::branchPredicted(WORD32 addr) const {
	return (status[addr] & CODE_BRANCHPRED) == CODE_BRANCHPRED;
}

BOOL CodeMemory::predictBranch(WORD32 addr, BOOL state) {
  if (state)
  	status[addr] |= CODE_BRANCHPRED;
  else
  	status[addr] &= (0xff - CODE_BRANCHPRED);
	
  return TRUE;
}

