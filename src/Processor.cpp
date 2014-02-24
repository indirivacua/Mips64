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
#include "Processor.h"
#include "pipeline.h"

Processor::Processor() : pipe(this) {
  code = NULL;
  data = NULL;
  screen = NULL;
}

Processor::~Processor() {
  delete code;
  delete data;
  delete[] screen;
}

void Processor::initialize(CPUConfig *config) {
    this->config = config;

    delete code;
    delete data;
    delete[] screen;

    this->code = new CodeMemory(config->codesize);
    this->data = new DataMemory(config->datasize);
    this->screen = new WORD32[GSXY*GSXY];

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

  code->reset();
  if (full) {
	data->reset();
  }

  this->terminal = "";
  this->drawit = FALSE; 
  this->keyboard = 0;

  clearScreen();

  fp_cc = FALSE;

  pipe.initialize(config);
}

BOOL Processor::setPC(WORD32 newPC) {
  this->PC = newPC;
  return TRUE;
}


BOOL Processor::setScreenPixel(int x, int y, WORD32 color) {
  this->screen[GSXY*y+x] = color;
  return TRUE;
}

BOOL Processor::clearScreen() {
  for (int i = 0; i < GSXY * GSXY; i++) 
    this->screen[i] = WHITE;
  return TRUE;
}

const BOOL Processor::isValidDataMemoryAddress(WORD32 ptr) { 
  return (ptr <= config->datasize) || (ptr >= MMIO && ptr <= MMIO+16);
}

const BOOL Processor::isValidCodeMemoryAddress(WORD32 ptr) { 
  return ptr <= config->codesize;
}

int Processor::clock_tick(RESULT *r) {
  int status = pipe.clock_tick(r);
  if (status == HALTED) 
	setStatus(HALTED);

  return status;
}
