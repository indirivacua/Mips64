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
#include "mytypes.h"

#include "Screen.h"

Screen::Screen(unsigned int x, unsigned int y) {
  max_x = x;
  max_y = y;
  this->screen = new WORD32[max_x * max_y];
  empty = TRUE;
}

Screen::~Screen() {
  delete[] screen;
}

BOOL Screen::setPixel(unsigned int x, unsigned int y, WORD32 color) {
  if (x > max_x || y > max_y)
    return FALSE;

  this->screen[max_x * y + x] = color;
  empty = FALSE;
  return TRUE;
}

BOOL Screen::clear() {
  for (unsigned int i = 0; i < max_x * max_y; i++)
    this->screen[i] = WHITE;
  empty = TRUE;
  return TRUE;
}

void Screen::show() {
  if (empty)
   return;

  std::cout << "----- Pantalla -----" << std::endl;
  for (unsigned int y = max_y - 1; y > 0; --y) {
    for (unsigned int x = 0; x < max_x; ++x) {
      char car = '-';
        int pixel = screen[x + y * max_x];
        switch(pixel) {
        case BLACK:
          car = 'X';
          break;
        case WHITE:
          car = ' ';
          break;
      }
      std::cout << car;
    }
    std::cout << std::endl;
  }

}

