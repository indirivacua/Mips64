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
#include "pipeline.h"
#include "assembler.h"


#define MIN_CODEBITS 8
#define MAX_CODEBITS 13
#define MIN_DATABITS 4
#define MAX_DATABITS 11
#define MIN_ADD_LATENCY 2
#define MAX_ADD_LATENCY 8
#define MIN_MUL_LATENCY 2
#define MAX_MUL_LATENCY 8
#define MIN_DIV_LATENCY 10
#define MAX_DIV_LATENCY 30

// Stages
#define IFETCH  1
#define IDECODE 2
#define INTEX   3
#define ADDEX   4
#define MULEX   5
#define DIVEX   6
#define MEMORY  7
#define WRITEB  8



class Simulator {
 public: // create from serialization only
  Simulator();

  // Attributes
 protected:
  char AppDir[MAX_PATH+1];
  char LasDir[MAX_PATH+1];
  std::string *codelines;
  std::string *datalines;
  std::string *assembly;
  std::string *mnemonic;
  std::string lastfile;
  
  unsigned int CODESIZE;
  unsigned int DATASIZE;
  
  Processor cpu;
  pipeline pipe;
  
  BOOL forwarding;
  BOOL delay_slot;
  BOOL branch_target_buffer;
  
  symbol_table code_table[SYMTABSIZE];
  symbol_table data_table[SYMTABSIZE];
  unsigned int codeptr;
  unsigned int dataptr;
  
  unsigned int code_symptr;
  unsigned int data_symptr;
  int CODEORDATA;
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
  unsigned int ADD_LATENCY;
  unsigned int MUL_LATENCY;
  unsigned int DIV_LATENCY;
  
  BOOL simulation_running;
  BOOL restart;
  int stall_type;
  int stalls;
  int amount;
  
  record history[50];
  WORD32 entries;
  WORD32 offset;
  
  
  // Implementation
 public:
  virtual ~Simulator();

  int mygets(char *, int, FILE *);
  BOOL openit(const std::string &);
  int openfile(const std::string &);
  
 protected:
  BOOL getcodesym(const char *&,WORD32 *);
  BOOL getdatasym(const char *&,WORD32 *);
  int instruction(const char *);
  BOOL directive(int, const char *, const char *);
  int first_pass(const char *,int);
  int second_pass(const char *,int);
  void process_result(RESULT *,BOOL);
  void clear();
  int one_cycle(pipeline *, Processor *,BOOL);
  void check_stalls(int,const char *,int, char *);
  void update_history(pipeline *, Processor *, RESULT *);
  int update_io(Processor *);
  
  // Generated message map functions
 protected:
 public:
  void OnFileReset();
  void OnFileOpen();
  void OnExecuteSingle();
  void OnFileMemory();
  void OnExecuteMulticycle();
  void OnFileMulti();
  void OnExecuteRunto();
  void OnExecuteStop();
  void OnUpdateExecuteStop();
  void OnUpdateExecuteRunto();
  void OnUpdateExecuteSingle();
  void OnUpdateExecuteMulticycle();
  void OnUpdateFileMulti();
  void OnUpdateFileMemory();
  void OnUpdateFileOpen();
  void OnUpdateFileReset();
  void OnUpdateExecuteInterrupt();
  void OnFullReset();
  void OnUpdateFullReset();
  int OnReload();
  void OnUpdateReload();
  void OnConfigureWordlength();
  void OnFileDelaySlot();
  void OnUpdateFileDelaySlot();
  void OnFileForwarding();
  void OnUpdateFileForwarding();
  void OnBtb();
  void OnUpdateBtb();
  
 public:
  void dump_mem();
  void dump_reg();
  void dump_Terminal();
  void show_stats();
  void show_screen();
  
  const int isRunning() { return !(cpu.getStatus() == HALTED); }

};

#endif
