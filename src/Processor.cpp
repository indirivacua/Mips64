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

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "mytypes.h"
#include "Processor.h"
#include "pipeline.h"

Processor::Processor(CodeMemory *mcode, DataMemory *mdata) 
  : code(mcode),
    data(mdata),
    pipe(this)
{
}

Processor::~Processor() {
}

void Processor::initialize(CPUConfig *config)  {
    this->config = config;
    this->reset(TRUE);
}

void Processor::reset(BOOL full) {
  int i;
  this->PC = 0;
  for (i = 0; i < 16; i++)
    this->mm[i] = 0;

  this->status = RUNNING;

  for (i = 0; i < 64; i++) {
      rreg[i].val = wreg[i].val = 0;
      rreg[i].source = wreg[i].source = FROM_REGISTER;
  }

  fp_cc = FALSE;

  pipe.initialize(config);
}

BOOL Processor::setPC(WORD32 newPC) {
  this->PC = newPC;
  return TRUE;
}

BOOL Processor::isValidDataMemoryAddress(WORD32 ptr) const { 
  return (data->isValidAddress(ptr)) 
             || (ptr >= MMIO && ptr <= MMIO + 16);
}

BOOL Processor::isValidCodeMemoryAddress(WORD32 ptr) const { 
  return code->isValidAddress(ptr);
}

int Processor::clock_tick(RESULT *r) {
  int status = pipe.clock_tick(r);

  if (status == HALTED) 
    setStatus(HALTED);

  return status;
}

void Processor::dump() {
  printf("----------------------------------\n");
  for (int i = 0; i < 32; i++) {
   printf("R%02i = %016llx%s", i, rreg[i].val, ((i + 1) % 4)?"\t":"\n");
  }
  printf("----------------------------------\n");
  DOUBLE64 db;

  for (int i = 0; i < 32; i++) {
   db.s = rreg[i+32].val;
   printf("F%02i = %016.8f%s", i, db.d, ((i+1)%4)?"\t":"\n");
  }

}

