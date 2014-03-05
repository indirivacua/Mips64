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

#define MMIO 0x10000


/////////////////////////////////////////////////////////////////////////////
// Simulator

Simulator::Simulator(CPUConfig *config)
  : cpu(&code, &data),
    data(config->datasize),
    code(config->codesize),
    stats(config)
 {
  Region *mmio = io.getMMRegion(); 
  data.registerRegion("io", mmio, MMIO);

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
  stats.reset();
  cpu.setPC(0);
  multi = 5;

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


int Simulator::one_cycle(BOOL show) {
  int status = 0;
  RESULT result;

  if (cpu.getStatus() == HALTED)
    return HALTED;

  status = cpu.clock_tick(&result);
  ++stats.cycles;
  BOOL res = stats.process_result(result, show);
  if (!res)
    cpu.setStatus(HALTED);

  history.update_history(stats.cycles, result, cpu);

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
  } while (((!code.hasBreakpoint(cpu.getPC()) && cpu.getStatus() != HALTED && simulation_running)));
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

void Simulator::show_screen() {
  io.show_screen();
}

