#include "Processor.h"

#include "mytypes.h"

Processor::Processor() {
    code = NULL;
    cstat = NULL;
    data = NULL;
    dstat = NULL;
    screen = NULL;
}

Processor::~Processor() {
	delete code;
	delete cstat;

	delete data;
	delete dstat;
	delete screen;
}

void Processor::initialize(int codesize, int datasize) {
    int i;
    this->codesize = codesize;
    this->datasize = datasize;

    delete[] code;
    delete[] cstat;
    delete[] data;
    delete[] dstat;
    delete[] screen;

	this->code = new BYTE[codesize];
	this->cstat = new BYTE[codesize];

	this->data = new BYTE[datasize];
	this->dstat = new BYTE[datasize];

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


  for (i = 0; i < codesize; i++)
    this->cstat[i] = 0;

  if (full) {
	for (i = 0; i < datasize; i++) 
		this->data[i] = this->dstat[i] = 0;
  }

  this->terminal=""; this->drawit=FALSE; this->keyboard=0;

  clearScreen();

  fp_cc = FALSE;

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
	return (ptr <= datasize) || (ptr >= MMIO && ptr <= MMIO+16);
}

const BOOL Processor::isValidCodeMemoryAddress(WORD32 ptr) { 
	return ptr <= codesize;
}
