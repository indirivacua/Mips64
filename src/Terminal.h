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


#ifndef __TERMINAL_H
#define __TERMINAL_H

#include<iostream>

#include "mytypes.h"

class Terminal {

public:
  Terminal();
  BOOL write(const std::string &msg) { this->terminal += msg; return TRUE; }
  BOOL clear() { this->terminal = ""; return TRUE; }
  BOOL empty() { return this->terminal == ""; }
  const std::string &getContents() const { return this->terminal; }

  BOOL readNumber(WORD64 *result);
  BOOL readChar(BYTE *result);

  void dump();

protected:

  std::string terminal;

};

#endif
