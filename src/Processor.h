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
  Processor();
  virtual ~Processor();

  void initialize(CPUConfig *config);
  void reset(BOOL = false);

  int clock_tick(RESULT *);

  WORD32 getPC() const { return PC; }
  BOOL setPC(WORD32);

  int getStatus() const { return this->status; }
  BOOL setStatus(int state) { this->status = state; return TRUE;}

  pipeline *getPipeline() { return &pipe; }

  WORD32 *getScreen() const { return screen; }
  BOOL setScreenPixel(int x, int y, WORD32 color);
  BOOL clearScreen();

  BOOL writeTerminal(const std::string &msg) { this->terminal += msg; return TRUE; }
  BOOL clearTerminal() { this->terminal = ""; return TRUE; }  
  BOOL emptyTerminal() { return this->terminal == ""; }
  const std::string &getTerminal() { return this->terminal; }

  const BOOL isValidDataMemoryAddress(WORD32 addr);
  const BOOL isValidCodeMemoryAddress(WORD32 addr);
  const WORD32 getCodeMemorySize() { return config->codesize; }
  const WORD32 getDataMemorySize() { return config->datasize; }

  // Esto deberia ir o protegido o eliminado (otra clase o algo)
  CodeMemory *code;
  DataMemory *data;

  BYTE   mm[16];

  reg    rreg[64];
  reg    wreg[64];
  BOOL fp_cc;

  BOOL drawit;
  WORD32 keyboard;

protected:
  WORD32 PC;
  int    status;

  pipeline pipe;
  CPUConfig *config;

  // A una clase Screen
  WORD32 *screen;

  // A una clase Terminal
  std::string terminal;
};

#endif
