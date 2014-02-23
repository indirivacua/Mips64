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

#ifndef __UTILS_H
#define __UTILS_H

// Used in class Assembler
void unpack32(WORD32,BYTE *);
void unpack16(SIGNED16,BYTE *);
void unpack(WORD64, BYTE *);
BOOL in_range(WORD32, WORD32);
int compare(const char*, const char *);
int alignment(int, int);
WORD64 strtoint64(const char *ptr, const char **end, int b);

// Deberiamos tener estas macros?? 
// En principio me parecio que guardaba bigendian, pero no esta conviertiendo nada

#define swap16(_val) \
	((WORD16)(((((WORD16)_val) & 0xff00) >> 8) | \
	       ((((WORD16)_val) & 0x00ff) << 8)))

#define swap32(_val) \
	((WORD32)(((((WORD32)_val) & 0xff000000) >> 24) | \
	       ((((WORD32)_val) & 0x00ff0000) >> 8 ) | \
	       ((((WORD32)_val) & 0x0000ff00) << 8 ) | \
	       ((((WORD32)_val) & 0x000000ff) << 24)))

#define swap64(_val) \
	((WORD64)(((((WORD64)_val) & 0xff00000000000000ull) >> 56) | \
	       ((((WORD64)_val) & 0x00ff000000000000ull) >> 40) | \
	       ((((WORD64)_val) & 0x0000ff0000000000ull) >> 24) | \
	       ((((WORD64)_val) & 0x000000ff00000000ull) >> 8 ) | \
	       ((((WORD64)_val) & 0x00000000ff000000ull) << 8 ) | \
	       ((((WORD64)_val) & 0x0000000000ff0000ull) << 24) | \
	       ((((WORD64)_val) & 0x000000000000ff00ull) << 40) | \
	       ((((WORD64)_val) & 0x00000000000000ffull) << 56)))

#endif
