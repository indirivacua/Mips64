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

#include "Terminal.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


Terminal::Terminal() {
}

void Terminal::dump() {
  if (empty()) 
    return;
  std::cout << "Terminal: ";
  std::cout << getContents() << std::endl;
  clear();
}

BOOL Terminal::readNumber(WORD64 *result) {
  char line[MAX_PATH+1];
  fgets(line, MAX_PATH, stdin); 
  DOUBLE64 number;
  if (strstr(line,"."))
    number.d = atof(line);
  else
    number.s = atoll(line);
  *result = number.u; 
  return TRUE;
}

BOOL Terminal::readChar(BYTE *result) {
  *result= getchar(); 
  return TRUE;
}
