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
#include <stdlib.h>

#include "processor.h"

#include <unistd.h>
#include <term.h>

void ClearScreen() {
  if (!cur_term) {
    int result;
    setupterm( NULL, STDOUT_FILENO, &result );
    if (result <= 0) return;
  }
  
  putp( tigetstr( "clear" ) );
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
  CWinEVEDoc *x = new CWinEVEDoc();
  int res = x->openfile(programa);
  //std::cout << "Res de apertura de archivo: " << res << "\n";
  if (res != 0) {
    std::cout << "Error al cargar, deteniendo." << std::endl; 
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
  x->show_stats();
  return 0;
}
