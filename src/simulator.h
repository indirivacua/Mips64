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

#ifndef __SIMULATOR_H
#define __SIMULATOR_H

#include <string>

#include "mytypes.h"
#include "utils.h"
#include "Processor.h"
#include "Screen.h"
#include "assembler.h"

#include "Terminal.h"

#include "PipelineHistory.h"

class Simulator {
 public: // create from serialization only
  Simulator(CPUConfig *config);
  virtual ~Simulator();

  int openfile(const std::string &);
  int isRunning() const { return cpu.getStatus() != HALTED; }

  void toggleDelaySlot();
  void toggleForwarding();
  void toggleBtb();

  CodeMemory *getCodeMemory() { return &code; }
  DataMemory *getDataMemory() { return &data; }

 protected:
  
  Processor cpu;

  DataMemory data;
  CodeMemory code;

  CPUConfig *config;
  
  unsigned int cycles;
  unsigned int instructions;
  unsigned int loads;
  unsigned int stores;
  unsigned int branch_taken_stalls;
  unsigned int branch_misprediction_stalls;
  unsigned int raw_stalls;
  unsigned int waw_stalls;
  unsigned int war_stalls;
  unsigned int structural_stalls;
  
  int multi;
  
  BOOL simulation_running;
  BOOL restart;
  int stall_type;
  int stalls;
  int amount;
  
  Terminal terminal;
  Screen screen;
  PipelineHistory history;

 protected:
  void clear();
  int one_cycle(BOOL);
  void check_stalls(int,const char *,int, char *);
  void process_result(RESULT, BOOL);
  int update_io();

  // Generated message map functions
 public:
  void OnFileReset();
  void OnExecuteSingle();
  void OnFileMemory();
  void OnExecuteMulticycle();
  void OnExecuteRunto();
  void OnExecuteStop();
  void OnFullReset();
  
 public:
  void dump_mem();
  void dump_reg();
  void dump_Terminal();
  void show_stats();
  void show_screen();

};

#endif
