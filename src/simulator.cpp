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

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define __STDC_FORMAT_MACROS
#include <inttypes.h>

#include <string>
#include <iostream>

#include "mytypes.h"
#include "utils.h"
#include "Processor.h"
#include "simulator.h"

#include "assembler.h"

#define GSXY 50

/////////////////////////////////////////////////////////////////////////////
// Simulator

Simulator::Simulator(CPUConfig *config)
  : cpu(&code, &data),
    data(config->datasize),
    code(config->codesize)
 {
  Region *mmio = io.getMMRegion(); 
  data.registerRegion("io", mmio, 0x10000);

  this->config = config;
  data.reset();

  simulation_running = FALSE;
  restart = FALSE;

  cpu.initialize(config);
  history.initialize(config);
  clear();
}

Simulator::~Simulator() {
}

void Simulator::clear() {
  cycles = instructions = loads = stores = branch_taken_stalls = branch_misprediction_stalls = 0;
  raw_stalls = waw_stalls = war_stalls = structural_stalls = 0;
  cpu.setPC(0);
    multi = 5;
  stall_type = stalls = 0;

  code.reset();
}

/////////////////////////////////////////////////////////////////////////////
// Simulator commands


void Simulator::OnFileReset() {
   // Reset the processor
  cpu.reset();
  clear();
}

void Simulator::OnFullReset() {
  cpu.reset(TRUE);

  // Reset Data Memory as well
  data.reset();
  clear();
}

void Simulator::check_stalls(int status, const char *str, int rawreg, char *txt) {
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

void Simulator::process_result(RESULT result, BOOL show) {
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
    cpu.setStatus(HALTED);
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
  if (show) {
/*
    if (txt[0] == 0)
      pStatus->SetPaneText(0, "Listo");
   else
      pStatus->SetPaneText(0, txt);
*/
  }
}

int Simulator::one_cycle(BOOL show) {
  int status = 0;
  RESULT result;

  if (cpu.getStatus() == HALTED)
    return HALTED;

  status = cpu.clock_tick(&result);
  ++cycles;
  process_result(result, show);

  // used to be in update_history but it is not part
  // Move to process_result ??
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

  history.update_history(cycles, result, cpu);

  if (io.update(&data))
    return WAITING_FOR_INPUT;

  return status;
}

void Simulator::OnExecuteSingle() {
  int status = one_cycle(TRUE);
  if (status == WAITING_FOR_INPUT) {
  //  pStatus->SetPaneText(0,"Esperando Entrada");
  }
}

void Simulator::OnExecuteMulticycle() {
  int i, status;
  simulation_running = TRUE;
  for (i = 0; i < multi - 1; i++) {
    status = one_cycle(FALSE);
    if (status)
      break;
  }
  if (status == 0)
    status = one_cycle(TRUE); // show status after last one.

  if (status == WAITING_FOR_INPUT) {
    //pStatus->SetPaneText(0,"Esperando Entrada");
  }

  simulation_running = FALSE;
}

void Simulator::OnExecuteStop() {
  simulation_running = FALSE;
}

void Simulator::OnExecuteRunto() {
  //MSG message;
  char buf[80];
  int status, lapsed = 0;
  simulation_running = TRUE;
  //pStatus->SetPaneText(0,"Ejecutando Simulación");
  do {
/*
    if (::PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
    {
      ::TranslateMessage(&message);
      ::DispatchMessage(&message);
    }
*/
    lapsed++;
    status = one_cycle(FALSE);
    if (status)
      break;
  } while (stalls || ((!code.hasBreakpoint(cpu.getPC()) && cpu.getStatus() != HALTED && simulation_running)));
  simulation_running = FALSE;
  if (status == WAITING_FOR_INPUT) {
    sprintf(buf, "Simulacion Detenida luego de %d ciclos - Esperando Entrada", lapsed);
    restart = TRUE;
  } else {
    sprintf(buf, "Simulacion Detenida luego de %d ciclos", lapsed);
    restart = FALSE;
  }

  //pStatus->SetPaneText(0, buf);
}

int Simulator::openfile(const std::string &fname) {
  OnFileReset();
  data.reset(); // reset data memory
  io.clear();

  return 0;
}

void Simulator::OnFileMemory() {
  cpu.initialize(config);
  history.initialize(config);
  clear();
}

void Simulator::toggleDelaySlot() {
  if (config->delay_slot)
    config->delay_slot = FALSE;
  else
    config->delay_slot = TRUE;
  if (config->delay_slot)
    config->branch_target_buffer = FALSE;
  OnFileReset();
}

void Simulator::toggleForwarding() {
  if (config->forwarding)
    config->forwarding = FALSE;
  else
    config->forwarding = TRUE;
  OnFileReset();
}

void Simulator::toggleBtb() {
  if (config->branch_target_buffer)
    config->branch_target_buffer = FALSE;
  else
    config->branch_target_buffer = TRUE;
  if (config->branch_target_buffer)
    config->delay_slot = FALSE;
  OnFileReset();
}

/** Metodos para verificar el funcionamiento del Simulador */

void Simulator::dump_reg() {
  cpu.dump();
}

void Simulator::dump_Terminal() {
  io.terminal_dump();
}

void Simulator::show_stats() {

  std::cout << "----- Estadisticas -----" << std::endl;
  std::cout << "Ciclo(s)               : " << cycles << std::endl;
  std::cout << "Instruccion(es)        : " << instructions << std::endl;
  if (instructions)
    std::cout << "CPI                    : " << (double) cycles/instructions << std::endl;
  std::cout << "Total Loads            : " << loads << std::endl;
  std::cout << "Total Stores           : " << stores << std::endl;
  std::cout << "Atasco(s) RAW          : " << raw_stalls << std::endl;
  std::cout << "Atasco(s) WAW          : " << waw_stalls << std::endl;
  std::cout << "Atasco(s) WAR          : " << war_stalls << std::endl;
  std::cout << "Atasco(s) Estructural  : " << structural_stalls << std::endl;
  std::cout << "Atasco(s) Branch Taken : " << branch_taken_stalls << std::endl;
  std::cout << "Atasco(s) Branch Mispr.: " << branch_misprediction_stalls << std::endl;
  //std::cout << "Tamanio del Codigo     : " << codeptr << std::endl;

}

void Simulator::show_screen() {
  io.show_screen();
}

