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

#ifndef __PIPELINEHISTORY_H
#define __PIPELINEHISTORY_H

// Stages
#define IFETCH  1
#define IDECODE 2
#define INTEX   3
#define ADDEX   4
#define MULEX   5
#define DIVEX   6
#define MEMORY  7
#define WRITEB  8

typedef struct {
	BYTE stage;
	BYTE substage;
	BYTE cause;
} entry;

typedef struct {
	WORD32 IR;
	WORD32 start_cycle;
	entry status[500];
} record;

class PipelineHistory {

public:
  PipelineHistory();

  void update_history(unsigned int cycles, const RESULT &result, Processor &cpu);
  BOOL initialize(CPUConfig *config);

protected:

  record history[50];
  WORD32 entries;

  unsigned int ADD_LATENCY;
  unsigned int MUL_LATENCY;
  unsigned int DIV_LATENCY;
};

#endif
