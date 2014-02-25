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


#ifndef __SCREEN_H
#define __SCREEN_H

class Screen {

public:
  Screen(unsigned int, unsigned int);
  virtual ~Screen();

  WORD32 *getScreen() const { return screen; }
  BOOL setPixel(unsigned int x, unsigned int y, WORD32 color);
  BOOL clear();

  void show();

protected:
  unsigned int max_x;
  unsigned int max_y;
  WORD32 *screen;

  BOOL empty;
};

#endif
