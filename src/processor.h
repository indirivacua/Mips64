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

#include <string>

#include "utils.h"
#include "mytypes.h"


#define MAX_PATH 200

#define afx_msg

typedef void *CCmdUI;

#define RGB(R,G,B) (((R) << 16) + ((G) << 8) + (B))

// From IOView.h
#define GSXY 50

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

#define SYMTABSIZE 1000

class CWinEVEDoc {
 public: // create from serialization only
  CWinEVEDoc();

  // Attributes
 protected:
  char AppDir[MAX_PATH+1];
  char LasDir[MAX_PATH+1];
  CString *codelines;
  CString *datalines;
  CString *assembly;
  CString *mnemonic;
  CString lastfile;
  
  unsigned int CODESIZE;
  unsigned int DATASIZE;
  
  processor cpu;
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
  
  // Operations
 public:
  
  // Overrides
 public:
  virtual BOOL OnNewDocument();
  
  // Implementation
 public:
  virtual ~CWinEVEDoc();

  int mygets(char *, int, FILE *);
  BOOL openit(CString);
  int openfile(CString);
  
 protected:
  BOOL getcodesym(const char *&,WORD32 *);
  BOOL getdatasym(const char *&,WORD32 *);
  int instruction(const char *);
  BOOL directive(int, const char *, const char *);
  int first_pass(const char *,int);
  int second_pass(const char *,int);
  void process_result(RESULT *,BOOL);
  void clear();
  int one_cycle(pipeline *,processor *,BOOL);
  void check_stalls(int,const char *,int,char *);
  void update_history(pipeline *,processor *,RESULT *);
  int update_io(processor *);
  
  // Generated message map functions
 protected:
 public:
  //{{AFX_MSG(CWinEVEDoc)
  afx_msg void OnFileReset();
  afx_msg void OnFileOpen();
  afx_msg void OnExecuteSingle();
  afx_msg void OnFileMemory();
  afx_msg void OnExecuteMulticycle();
  afx_msg void OnFileMulti();
  afx_msg void OnExecuteRunto();
  afx_msg void OnExecuteStop();
  afx_msg void OnUpdateExecuteStop(CCmdUI* pCmdUI);
  afx_msg void OnUpdateExecuteRunto(CCmdUI* pCmdUI);
  afx_msg void OnUpdateExecuteSingle(CCmdUI* pCmdUI);
  afx_msg void OnUpdateExecuteMulticycle(CCmdUI* pCmdUI);
  afx_msg void OnUpdateFileMulti(CCmdUI* pCmdUI);
  afx_msg void OnUpdateFileMemory(CCmdUI* pCmdUI);
  afx_msg void OnUpdateFileOpen(CCmdUI* pCmdUI);
  afx_msg void OnUpdateFileReset(CCmdUI* pCmdUI);
  afx_msg void OnUpdateExecuteInterrupt(CCmdUI* pCmdUI);
  afx_msg void OnFullReset();
  afx_msg void OnUpdateFullReset(CCmdUI* pCmdUI);
  afx_msg int OnReload();
  afx_msg void OnUpdateReload(CCmdUI* pCmdUI);
  afx_msg void OnConfigureWordlength();
  afx_msg void OnFileDelaySlot();
  afx_msg void OnUpdateFileDelaySlot(CCmdUI* pCmdUI);
  afx_msg void OnFileForwarding();
  afx_msg void OnUpdateFileForwarding(CCmdUI* pCmdUI);
  afx_msg void OnBtb();
  afx_msg void OnUpdateBtb(CCmdUI* pCmdUI);
  //}}AFX_MSG
  
 public:
  void dump_mem();
  void dump_reg();
  void dump_Terminal();
  void show_stats();
  void show_screen();
  
  const int isRunning() { return !(cpu.status == HALTED); }

};

#endif
