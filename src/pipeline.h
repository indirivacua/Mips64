#ifndef __PIPELINE_H
#define __PIPELINE_H

#define READ 0
#define WRITE 1
#define BOTH 2

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

typedef struct {
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
} pipeline;

void init_pipeline(pipeline *, int, int, int);
int clock_tick(pipeline *, Processor *, BOOL, BOOL, BOOL, RESULT *);

#endif
