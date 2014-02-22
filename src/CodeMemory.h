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

#ifndef __CODEMEMORY_H
#define __CODEMEMORY_H

#include "Memory.h"

#define CODE_VALID        0
#define CODE_BREAKPOINT   1
#define CODE_BRANCHPRED   2
#define CODE_INVALID      4

class CodeMemory {
public:
  CodeMemory(WORD32);
  virtual ~CodeMemory();

  BOOL reset();

  WORD32 readInstruction(WORD32 addr) const;

  BOOL writeInstruction(WORD32, WORD32 instr, const std::string &, const std::string &, const std::string &);
  BOOL invalidateInstruction(WORD32 addr);

  BOOL isValidAddress(WORD32 addr);
  WORD32 getMemorySize() const { return size; };

  BOOL setBreakpoint(WORD32 addr, BOOL); 
  BOOL hasBreakpoint(WORD32 addr) const; 

  BOOL branchPredicted(WORD32 addr) const; 
  BOOL predictBranch(WORD32 addr, BOOL); 

protected:
  WORD32 size;
  WORD32 *code;
  BYTE *status;
  std::string *line;
  std::string *assembly;
  std::string *mnemonic;

};

#endif
