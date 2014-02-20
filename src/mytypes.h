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

//
// Useful types
//

#include <stdint.h>

#include <string>

#define MAX_PATH 200
#define MAX_LINE 200

#define FALSE 0
#define TRUE 1

#define READ 0
#define WRITE 1
#define BOTH 2

#define ENDLINE 1
#define SPACE   2
#define COMMENT 3

// Data Memory Status
#define VACANT  0
#define WRITTEN 1

// Code Memory Status Bits
// 0 => normal
// 1 => BreakPoint set
// 2 => Branch Prediction ?
// 4 => ERROR 

#define RUNNING 0
#define STOPPED 1

#define INT 0
#define ADD 1
#define MUL 2
#define DIV 3

// Stages

#define IFETCH  1
#define IDECODE 2
#define INTEX   3
#define ADDEX   4
#define MULEX   5
#define DIVEX   6
#define MEMORY  7
#define WRITEB  8

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

#define DBLUE	 RGB(0,0,128)
#define DMAGENTA RGB(128,0,128)
#define DRED	 RGB(128,0,0)
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

/* pipeline stage status */

#define NOT_A_BRANCH 0
#define BRANCH_TAKEN 1
#define BRANCH_NOT_TAKEN 2

#define OK			0

// stalls...

#define RAW			1
#define WAW			2 
#define STALLED		3
#define HALTED		4
#define STRUCTURAL	5
#define WAR         6
#define BRANCH_TAKEN_STALL 7
#define BRANCH_MISPREDICTED_STALL 8

// advisories....

#define DATA_ERR	9
#define EMPTY		10
#define DIVIDE_BY_ZERO   11
#define INTEGER_OVERFLOW 12
#define NO_SUCH_DATA_MEMORY   13
#define LOADS		14
#define STORES		15
#define NO_SUCH_CODE_MEMORY		16
#define DATA_MISALIGNED 17
#define WAITING_FOR_INPUT 18

/* register status */

#define NOT_AVAILABLE  0
#define FROM_REGISTER  1
#define FROM_MEM       2
#define FROM_EX        3
#define FROM_ID        4
#define FROM_ADD       5
#define FROM_MUL       6
#define FROM_DIV       7

typedef struct {
    const char *name;
    BYTE type;
    BYTE subtype;
    WORD32 op_code;
} op_code_info;

#define CODE 1
#define DATA 2

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

typedef struct {
    WORD64 val;
    SIGNED32 source;
} reg;

typedef struct {
	BYTE stage;
	BYTE substage;
	BYTE cause;
} entry;

typedef struct {
	WORD32 IR;
	WORD32 start_cycle;
	entry status[500];
} record;

#define GSXY 50
#define RGB(R,G,B) (((R) << 16) + ((G) << 8) + (B))

#endif
