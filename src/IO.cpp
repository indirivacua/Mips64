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
#include <stdio.h>

#define __STDC_FORMAT_MACROS
#include <inttypes.h>

#include "mytypes.h"

#include "IO.h" 
#include "DataMemory.h"

#define IO_CONTROL 0
#define IO_DATA   8 

#define GSXY 50

IO::IO(): mmio(16),  
    screen(GSXY, GSXY) {
  mmio.disableCheckReads();
  screen.clear();
}

Region *IO::getMMRegion() {
  return &mmio;
}

WORD64 IO::getData() {
  WORD64 data;
  mmio.readWord64(IO_DATA, data);
  return data;
}

BOOL IO::setData(WORD64 v) {
  return mmio.writeWord64(IO_DATA, v);
}

WORD64 IO::getControl() {
  WORD64 control;
  mmio.readWord64(IO_CONTROL, control);
/*
  if (control)
	std::cout << "getControl() = " <<  control << std::endl;
*/
  return control;
}

BOOL IO::clearControl() {
  return mmio.writeWord64(IO_CONTROL, 0);
}

int IO::update(DataMemory *data) {
  WORD32 func = getControl(); 
  int x, y, status = 0;

  BYTE az[256];
  if (!func)
    return status;

  char txt[30];
  DOUBLE64 fp;
  fp.u = getData(); 

  switch (func) {
  case 1:
    sprintf(txt,"%" PRIu64 "\n", fp.u);
    terminal.write(txt);
    break;
  case 2:
    sprintf(txt,"%" PRIi64 "\n", fp.s);
    terminal.write(txt);
    break;
  case 3:
    sprintf(txt,"%lf\n", fp.d);
    terminal.write(txt);
    break;
  case 4:
    // need to test here if fp.u is a legal address!
    data->getAsciiz(fp.u, az, 255);

    if (fp.u<data->getSize())
      terminal.write(std::string((const char *)az));
    break;

  case 5:
    y = (WORD32) ((fp.u >> 32) & 255);
    x = (WORD32) ((fp.u >> 40) & 255);
    screen.setPixel(x, y, (WORD32) fp.u);
    break;

  case 6:
    terminal.clear();
    break;

  case 7:
   screen.clear();
   break;

  case 8:
    WORD64 n;
    terminal.readNumber(&n);
    mmio.writeWord64(IO_DATA, n);
    break;

  case 9:
    BYTE c;
    terminal.readChar(&c);
    mmio.writeByte(IO_DATA, c);
    break;

  default:
    break;
  }

  clearControl(); //*(WORD32 *)&cpu.mm[0] = 0;
  return status;
}

void IO::show_screen() {
  screen.show();
}

void IO::clear() {
  screen.clear();
  terminal.clear();
}

void IO::terminal_dump() {
  terminal.dump();
}
