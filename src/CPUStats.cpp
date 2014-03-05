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

#include <stdio.h>
#include <iostream>


#include "mytypes.h"
#include "CPUConfig.h"
#include "pipeline.h"
#include "CPUStats.h"

CPUStats::CPUStats(CPUConfig *cfg) : config(cfg) {
  reset();
}

void CPUStats::reset() {
  cycles = 0;
  instructions = 0;
  loads = 0;
  stores = 0;
  branch_taken_stalls = 0;
  branch_misprediction_stalls = 0;
  raw_stalls = 0;
  waw_stalls = 0;
  war_stalls = 0;
  structural_stalls = 0;
}

void CPUStats::check_stalls(int status, const char *str, int rawreg, char *txt) {
  char mess[100] = "";
  if (status == RAW) {
    raw_stalls++;
    if (rawreg < 32)
      sprintf(mess,"  Atasco RAW en %s (R%d)", str, rawreg);
    else
      sprintf(mess,"  Atasco RAW en %s (F%d)", str, rawreg - 32);
    strcat(txt, mess);
  }
  if (status == WAW) {
    waw_stalls++;
    if (rawreg < 32)
      sprintf(mess,"  Atasco WAW en %s (R%d)", str, rawreg);
    else
      sprintf(mess,"  Atasco WAW en %s (F%d)", str, rawreg-32);
    strcat(txt, mess);
  }
  if (status == WAR) {
    war_stalls++;
    if (rawreg < 32)
      sprintf(mess,"  Atasco WAR en %s (R%d)", str, rawreg);
    else
      sprintf(mess,"  Atasco WAR en %s (F%d)", str, rawreg-32);
    strcat(txt, mess);
  }
  if (strcmp(mess, "") != 0)
    std::cout << "Stall Msg: " << mess << std::endl;
}

BOOL CPUStats::process_result(RESULT result, BOOL show) {
  BOOL res = TRUE;
  char txt[300];
  //BOOL something = FALSE;
  if (result.WB == OK || result.WB == HALTED)
    instructions++;
  txt[0] = 0;

  if (!config->delay_slot && result.ID == BRANCH_TAKEN_STALL) {
    //something = TRUE;
    branch_taken_stalls++;
    strcat(txt,"  Atasco Branch Taken");
  }
  if (result.ID == BRANCH_MISPREDICTED_STALL) {
    //something = TRUE;
    branch_misprediction_stalls++;
    strcat(txt,"  Atasco Branch Misprediction");
  }

  if (result.MEM == LOADS || result.MEM == DATA_ERR)
    loads++;
  if (result.MEM == STORES)
    stores++;

  check_stalls(result.ID, "ID", result.idrr, txt);
  check_stalls(result.EX, "EX", result.exrr, txt);
  check_stalls(result.ADDER[0], "ADD", result.addrr, txt);
  check_stalls(result.MULTIPLIER[0], "MUL", result.mulrr, txt);
  check_stalls(result.DIVIDER, "DIV", result.divrr, txt);
  check_stalls(result.MEM, "MEM", result.memrr, txt);

  if (result.MEM != RAW) {
    if (result.EX == STALLED) {
      structural_stalls++;
      strcat(txt,"  Atasco Estructural en EX");
    }
    if (result.DIVIDER == STALLED) {
      structural_stalls++;
      strcat(txt,"  Atasco Estructural en FP-DIV");
    }
    if (result.MULTIPLIER[config->MUL_LATENCY-1] == STALLED) {
      structural_stalls++;
      strcat(txt,"  Atasco Estructural en FP-MUL");
    }
    if (result.ADDER[config->ADD_LATENCY-1] == STALLED) {
      structural_stalls++;
      strcat(txt,"  Atasco Estructural en FP-ADD");
    }
  }
  if (result.IF == NO_SUCH_CODE_MEMORY) {
    strcat(txt,"  No existe esa direccion de codigo!");
    res = FALSE;
  }
  if (result.EX == INTEGER_OVERFLOW) {
    strcat(txt,"  Desbordamiento de numero entero!");
  }
  if (result.DIVIDER == DIVIDE_BY_ZERO) {
    strcat(txt,"  Division por Cero en DIV!");
  }
  if (result.MEM == DATA_ERR) {
    strcat(txt,"  Memoria no inicializada en MEM!");
  }
  if (result.MEM == NO_SUCH_DATA_MEMORY) {
    strcat(txt,"  No existe esa direccion de datos!");
  }
  if (result.MEM == DATA_MISALIGNED) {
    strcat(txt, " Error Fatal - LOAD/StTORE de memoria mal alineado!");
  }

  // used to be in update_history but it is not part
  if (result.MEM != RAW) {
    if (result.EX == STALLED)
      result.EX = STRUCTURAL;
    if (result.DIVIDER == STALLED)
      result.DIVIDER = STRUCTURAL;
    if (result.MULTIPLIER[config->MUL_LATENCY-1] == STALLED)
      result.MULTIPLIER[config->MUL_LATENCY-1] = STRUCTURAL;
    if (result.ADDER[config->ADD_LATENCY-1] == STALLED)
      result.ADDER[config->ADD_LATENCY-1] = STRUCTURAL;
  }

  if (show) {
/*
    if (txt[0] == 0)
      pStatus->SetPaneText(0, "Listo");
   else
      pStatus->SetPaneText(0, txt);
*/
  }
  return res;
}


