#ifndef __PROCESSOR_H
#define __PROCESSOR_H

#include "mytypes.h"

#define RUNNING 0
#define STOPPED 1

// HACER: Descablear MMIO de todo el codigo
#define MMIO 0x10000

class Processor {

public:
  Processor();
  ~Processor();

  void initialize(int, int);
  void reset(BOOL = false);

  const WORD32 getPC() { return PC; }
  BOOL setPC(WORD32);

  const int getStatus() { return this->status; }
  BOOL setStatus(int state) { this->status = state; return TRUE;}

  BYTE   *code;
  BYTE   *cstat;

  BYTE   *data;
  BYTE   *dstat;

  BYTE   mm[16];

  reg    rreg[64];
  reg    wreg[64];
  BOOL fp_cc;

  BOOL drawit;
  WORD32 keyboard;

  const WORD32 *getScreen() { return screen; }
  BOOL setScreenPixel(int x, int y, WORD32 color);
  BOOL clearScreen();

  BOOL writeTerminal(const std::string &msg) { this->terminal += msg; return TRUE; }
  BOOL clearTerminal() { this->terminal = ""; return TRUE; }  
  BOOL emptyTerminal() { return this->terminal == ""; }
  const std::string &getTerminal() { return this->terminal; }

  const BOOL isValidDataMemoryAddress(WORD32 addr);
  const BOOL isValidCodeMemoryAddress(WORD32 addr);
  const WORD32 getCodeMemorySize() { return codesize; }
  const WORD32 getDataMemorySize() { return datasize; }

protected:
  WORD32 PC;
  int    status;

  // A una clase de memoria
  WORD32 codesize;
  WORD32 datasize;

  // A una clase Screen
  WORD32 *screen;

  // A una clase Terminal
  std::string terminal;
};

#endif
