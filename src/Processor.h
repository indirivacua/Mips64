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

#ifndef __PROCESSOR_H
#define __PROCESSOR_H

#include "mytypes.h"
#include "CPUConfig.h"

#include "pipeline.h"

#include "DataMemory.h"
#include "CodeMemory.h"


#define RUNNING 0
#define STOPPED 1

// HACER: Descablear MMIO de todo el codigo
#define MMIO 0x10000

typedef struct {
    WORD64 val;
    SIGNED32 source;
} reg;


class Processor {

public:
  Processor(CodeMemory *code, DataMemory *data);
  virtual ~Processor();

  void initialize(CPUConfig *config);
  void reset(BOOL = false);

  int clock_tick(RESULT *);

  WORD32 getPC() const { return PC; }
  BOOL setPC(WORD32);

  int getStatus() const { return this->status; }
  BOOL setStatus(int state) { this->status = state; return TRUE;}

  pipeline *getPipeline() { return &pipe; }

  BOOL isValidDataMemoryAddress (WORD32 addr) const;
  BOOL isValidCodeMemoryAddress (WORD32 addr) const;
  WORD32 getCodeMemorySize() const { return code->getSize(); }
  WORD32 getDataMemorySize() const { return data->getSize(); }

  CodeMemory *getCodeMemory() { return code; }
  DataMemory *getDataMemory() { return data; }

  BOOL hasFPFlagOn() const { return fp_cc; } 
  BOOL hasFPFlagOff() const { return !fp_cc; } 
  BOOL setFPFlag(BOOL state) { fp_cc = state; return TRUE; } 

  BYTE   mm[16];

  reg    rreg[64];
  reg    wreg[64];

  void dump();

protected:
  WORD32 PC;
  int    status;

  // Flag FP 
  BOOL fp_cc;

  CodeMemory *code;
  DataMemory *data;
  CPUConfig *config;
  pipeline pipe;

};

#endif
