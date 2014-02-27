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

#ifndef __MY_TYPES_H
#define __MY_TYPES_H

#include <stdint.h>
#include <string>

//
// Useful types
//

#define MAX_PATH 200
#define MAX_LINE 200

#define FALSE 0
#define TRUE 1

#define RGB(R,G,B) (((R) << 16) + ((G) << 8) + (B))

// Code Memory Status Bits
// 0 => normal
// 1 => BreakPoint set
// 2 => Branch Prediction ?
// 4 => ERROR

#define WHITE  RGB(255,255,255)
#define BLACK  RGB(0,0,0)
#define GREY   RGB(128,128,128)
#define YELLOW RGB(255,255,0)

#define CYAN   RGB(0,255,255)
#define RED    RGB(255,0,0)
#define GREEN  RGB(0,255,0)
#define BLUE   RGB(0,0,255)
#define MAGENTA   RGB(255,0,255)

#define DGREEN   RGB(0,128,0)
#define DCYAN    RGB(0,128,128)
#define DYELLOW  RGB(128,128,0)

#define DBLUE   RGB(0,0,128)
#define DMAGENTA RGB(128,0,128)
#define DRED   RGB(128,0,0)
#define LGREY    RGB(192,192,192)

typedef uint8_t  BYTE;
typedef uint16_t WORD16;
typedef uint32_t WORD32;
typedef uint64_t WORD64;

typedef int16_t SIGNED16;
typedef int32_t SIGNED32;
typedef int64_t SIGNED64;

typedef int BOOL;

typedef union {
  WORD64 u;
  SIGNED64 s;
  double d;
} DOUBLE64;

#define OK      0


#define STEP 8
#define MASK -1

typedef struct {
  int IF;
  int ID;
  int EX;
  int MEM;
  int WB;
  int DIVIDER;
  int ADDER[10];
  int MULTIPLIER[10];
  int idrr;
  int exrr;
  int memrr;
  int addrr;
  int mulrr;
  int divrr;
} RESULT;

typedef struct {
  int type,function,opcode,tf,target;
  int rs,rt,rd;
  int src1,src2;
  SIGNED32 Imm;
} instruction;

#endif
