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

#include "simulator.h"

#include "PipelineHistory.h"

PipelineHistory::PipelineHistory() {
  entries = 1;
  history[0].IR = 0;
  history[0].start_cycle = 0;
  history[0].status[0].stage = IFETCH;
}

BOOL PipelineHistory::initialize(CPUConfig *config) {
  entries = 1;
  history[0].IR = 0;
  history[0].start_cycle = 0;
  history[0].status[0].stage = IFETCH;

  ADD_LATENCY = config->ADD_LATENCY;
  MUL_LATENCY = config->MUL_LATENCY;
  DIV_LATENCY = config->DIV_LATENCY;
  return TRUE;
}

void PipelineHistory::update_history(unsigned int cycles, const RESULT &result, Processor &cpu) {
  int substage, stage;
  unsigned int i, cc;
  WORD32 previous;
  BOOL passed;
  pipeline *pipe = cpu.getPipeline();

  for (i = 0; i < entries; i++) {
    previous = history[i].IR;
    cc = cycles - history[i].start_cycle;
    stage = history[i].status[cc-1].stage; // previous stage
    substage = history[i].status[cc-1].substage;

    switch (stage) {

    case IFETCH:
      if (pipe->if_id.active)  {
        if (pipe->if_id.IR == previous) {

          history[i].status[cc].stage = IDECODE;
          history[i].status[cc].cause = 0;
        } else {
          history[i].status[cc].stage = IFETCH;
          history[i].status[cc].cause = (BYTE) result.IF;
        }
      } else {
        history[i].status[cc].stage = 0;
        history[i].status[cc].cause = 0;
      }
      break;
    case IDECODE:
      passed = FALSE;

      if (pipe->integer.active && pipe->integer.IR == previous && result.ID != STALLED) {
        passed = TRUE;
        history[i].status[cc].stage = INTEX;
        history[i].status[cc].cause = 0;
      }
      if (pipe->m[0].active && pipe->m[0].IR == previous && result.ID != STALLED) {
        passed = TRUE;
        history[i].status[cc].stage = MULEX;
        history[i].status[cc].substage = 0;
        history[i].status[cc].cause = 0;
      }
      if (pipe->a[0].active && pipe->a[0].IR == previous && result.ID != STALLED) {
        passed = TRUE;
        history[i].status[cc].stage = ADDEX;
        history[i].status[cc].substage = 0;
        history[i].status[cc].cause = 0;
      }
      if (pipe->div.active && pipe->div.IR == previous && result.ID != STALLED) {
        passed = TRUE;
        history[i].status[cc].stage = DIVEX;
        history[i].status[cc].cause = 0;
      }

      if (!passed) {
        history[i].status[cc].stage = IDECODE;
        history[i].status[cc].cause = (BYTE) result.ID;
      }
      break;
    case INTEX:
      if (pipe->ex_mem.active && pipe->ex_mem.IR == previous) {
        history[i].status[cc].stage = MEMORY;
        history[i].status[cc].cause = 0;
      } else {
        history[i].status[cc].stage = INTEX;
        history[i].status[cc].cause = (BYTE) result.EX;
      }
      break;

    case MULEX:
      if (substage == pipe->MUL_LATENCY - 1) {
        if (pipe->ex_mem.active && pipe->ex_mem.IR == previous) {
          history[i].status[cc].stage = MEMORY;
          history[i].status[cc].cause = 0;
        } else {
          history[i].status[cc].stage = MULEX;
          history[i].status[cc].substage = (BYTE) substage;
          history[i].status[cc].cause = (BYTE) result.MULTIPLIER[this->MUL_LATENCY - 1];
        }
      } else {
        if (pipe->m[substage+1].active && pipe->m[substage+1].IR == previous) {
          history[i].status[cc].stage = MULEX;
          history[i].status[cc].substage = (BYTE) (substage + 1);
          history[i].status[cc].cause = 0;
        } else {
          history[i].status[cc].stage = MULEX;
          history[i].status[cc].substage = (BYTE) substage;
          history[i].status[cc].cause = (BYTE) result.MULTIPLIER[substage];
        }
      }
      break;

    case ADDEX:
      if (substage == pipe->ADD_LATENCY - 1) {
        if (pipe->ex_mem.active && pipe->ex_mem.IR == previous) {
          history[i].status[cc].stage = MEMORY;
          history[i].status[cc].cause = 0;
        } else {
          history[i].status[cc].stage = ADDEX;
          history[i].status[cc].substage = (BYTE) substage;
          history[i].status[cc].cause = (BYTE) result.ADDER[this->ADD_LATENCY-1];
        }
      } else {
        if (pipe->a[substage+1].active && pipe->a[substage+1].IR == previous) {
          history[i].status[cc].stage = ADDEX;
          history[i].status[cc].substage = (BYTE) (substage + 1);
          history[i].status[cc].cause = 0;
        } else {
          history[i].status[cc].stage = ADDEX;
          history[i].status[cc].substage = (BYTE) substage;
          history[i].status[cc].cause = (BYTE) result.ADDER[substage];
        }
      }
      break;
    case DIVEX:
      if (pipe->ex_mem.active && pipe->ex_mem.IR == previous) {
        history[i].status[cc].stage = MEMORY;
        history[i].status[cc].cause = 0;
      } else {
        history[i].status[cc].stage = DIVEX;
        history[i].status[cc].cause = (BYTE) result.DIVIDER;
      }
      break;

    case MEMORY:
      if (pipe->mem_wb.active && pipe->mem_wb.IR == previous) {
        history[i].status[cc].stage = WRITEB;
        history[i].status[cc].cause = 0;
      } else {
        history[i].status[cc].stage = MEMORY;
        history[i].status[cc].cause = (BYTE) result.MEM;
      }
      break;

    case WRITEB:
      history[i].status[cc].stage = 0;
      history[i].status[cc].cause = 0;
      break;

    default:
      history[i].status[cc].stage = 0;
      history[i].status[cc].cause = 0;
    }
  }

// make a new entry
//  if (cpu->PC != history[entries-1].IR)
  if ((result.ID == OK || result.ID == EMPTY || cpu.getPC() != history[entries-1].IR) && pipe->active) {
    history[entries].IR = cpu.getPC();
    history[entries].status[0].stage = IFETCH;
    history[entries].status[0].cause = 0;
    history[entries].start_cycle = cycles;
    entries++;
  }
  if (entries == 50) {
    entries--;
    for (i = 0; i < entries; i++) {
      history[i] = history[i + 1];
    }
  }
}
