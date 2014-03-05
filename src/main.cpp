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

#include <iostream>
#include <string.h>

#include "simulator.h"
#include "CPUConfig.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <term.h>

void ClearScreen() {
  if (!cur_term) {
    int result;
    setupterm( NULL, STDOUT_FILENO, &result );
    if (result <= 0) return;
  }

  char str[] = "clear";
  putp( tigetstr( str ) );
}

void show_stats(const CPUStats &stats) {
  std::cout << "----- Estadisticas -----" << std::endl;
  std::cout << "Ciclo(s)               : " << stats.cycles << std::endl;
  std::cout << "Instruccion(es)        : " << stats. instructions << std::endl;
  if (stats.instructions)
    std::cout << "CPI                    : " << (double) stats.cycles/stats.instructions << std::endl;
  std::cout << "Total Loads            : " << stats.loads << std::endl;
  std::cout << "Total Stores           : " << stats.stores << std::endl;
  std::cout << "Atasco(s) RAW          : " << stats.raw_stalls << std::endl;
  std::cout << "Atasco(s) WAW          : " << stats.waw_stalls << std::endl;
  std::cout << "Atasco(s) WAR          : " << stats.war_stalls << std::endl;
  std::cout << "Atasco(s) Estructural  : " << stats.structural_stalls << std::endl;
  std::cout << "Atasco(s) Branch Taken : " << stats.branch_taken_stalls << std::endl;
  std::cout << "Atasco(s) Branch Mispr.: " << stats.branch_misprediction_stalls << std::endl;
  //std::cout << "Tamanio del Codigo     : " << codeptr << std::endl;

}

int main(int argc, char **argv) {
  int grafico = 0;
  char programa[MAX_PATH + 1];

  if (argc >= 2) {
    strncpy(programa, argv[1], MAX_PATH);
  } else {
    std::cout << "Uso: " << argv[0] << " <programa.s> <modo>" << std::endl;
    std::cout << "     - modo = 0 texto, 1 grafico" << std::endl;
    return -2;
  }
  if (argc >= 3) {
    grafico = atoi(argv[2]);
  }

  CPUConfig config;
  Simulator *x = new Simulator(&config);
  x->openfile(programa);
  Assembler *assembler = new Assembler(x->getCodeMemory(), x->getDataMemory());
  int res = assembler->openit(programa);
  if (res == 1) {
  char txt[512];
  sprintf(txt, "No se pudo abrir el archivo %s", programa);
  //AfxMessageBox(txt, MB_OK | MB_ICONEXCLAMATION);
  std::cout << txt << std::endl;
  return res;
  }
  if (res == 2) {
    std::cout << "Errores detectados en 1ra Pasada" << std::endl;
    return res;
  }
  if (res == 3) {
    std::cout << "Errores detectados en 2da Pasada" << std::endl;
    return res;
  }
  //AfxGetMainWnd()->SetWindowText("WinMIPS64 - Simulador de Procesador MIPS64 - " + fname);

  delete assembler;

  //std::cout << "Res de apertura de archivo: " << res << "\n";
  if (res != 0) {
    std::cout << "Error al cargar " << programa << ", deteniendo." << std::endl;
    return -1;
  }
  std::cout << "Archivo " << programa << " abierto con exito\n";

  int i = 0;
  while (x->isRunning()) {
    x->OnExecuteSingle();
    if (grafico) {
      if (i % 32 == 0) {
  //system("/bin/bash -c clear");
  ClearScreen();
  x->show_screen();
      }
    } else {
      x->dump_Terminal();
    }
    ++i;
  }
  x->dump_reg();
  show_stats(x->getStats());
  return 0;
}
