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

#ifndef __PIPELINE_H
#define __PIPELINE_H

#define READ   0
#define WRITE  1
#define BOTH   2

/* pipeline stage status */

#define NOT_A_BRANCH 0
#define BRANCH_TAKEN 1
#define BRANCH_NOT_TAKEN 2

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
    WORD32 IR;  /* pointer to instruction in memory */
    instruction ins;
    WORD32 NPC;
    BOOL active;
	BOOL predicted;
} if_id_reg;

typedef struct {
    WORD32 IR;
    instruction ins;
    int rA,rB;
    WORD32 NPC;
	WORD64 ALUOutput;
    SIGNED32 Imm;
    BOOL active;
    int cycles;
} id_ex_reg;

typedef struct {
    WORD32 IR;
    instruction ins;
    int rB;
    WORD64 ALUOutput;
    WORD32 NPC;
    BOOL active;
	BOOL condition;
} ex_mem_reg;

typedef struct {
    WORD32 IR;
    instruction ins;
    WORD64 ALUOutput,LMD;
    WORD32 NPC;
    BOOL active;
	BOOL condition;
} mem_wb_reg;

class pipeline {

public:

    pipeline(Processor *);

    BOOL active;
	BOOL halting;
    BOOL   branch;
    WORD32 destination;
	int ADD_LATENCY;
	int MUL_LATENCY;
	int DIV_LATENCY;

    if_id_reg  if_id;
    id_ex_reg integer;
    id_ex_reg m[10];
    id_ex_reg a[10];
    id_ex_reg div;
    ex_mem_reg ex_mem;
    mem_wb_reg  mem_wb;

    Processor *cpu;
    
    void initialize(int ADDS, int MULS, int DIVS, BOOL, BOOL, BOOL);
    int clock_tick(RESULT *);

protected:

    int IF();
    int ID(int *rawreg);

    void EX_MUL(int *rawreg,int *status);
    int EX_DIV(int *rawreg);
    void EX_ADD(int *rawreg, int *status);
    int EX_INT(int *rawreg);

    int MEM(int *rawreg);
    int WB();

    BOOL delay_slot;
    BOOL branch_target_buffer;
    BOOL forwarding;

};


#endif
