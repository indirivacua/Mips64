#include "mytypes.h"
#include "language.h"

const char *directives[]=
{".space",
 ".asciiz",
 ".align",
 ".word",
 ".byte",
 ".ascii",
 ".global",
 ".data",
 ".text",
 ".org",
 ".word32",
 ".word16",
 ".double",
 ".code",
 NULL};

op_code_info codes[]={
{"lb",     I_TYPE, LOAD,   SI(I_LB)},
{"lbu",    I_TYPE, LOAD,   SI(I_LBU)},
{"sb",     I_TYPE, STORE,  SI(I_SB)},
{"lh",     I_TYPE, LOAD,   SI(I_LH)},
{"lhu",    I_TYPE, LOAD,   SI(I_LHU)},
{"sh",     I_TYPE, STORE,  SI(I_SH)},
{"lw",     I_TYPE, LOAD,   SI(I_LW)},   
{"lwu",    I_TYPE, LOAD,   SI(I_LWU)},   
{"sw",     I_TYPE, STORE,  SI(I_SW)},
{"ld",     I_TYPE, LOAD,   SI(I_LD)},
{"sd",     I_TYPE, STORE,  SI(I_SD)},
{"l.d",    I_TYPE, FLOAD,  SI(I_L_D)},
{"s.d",    I_TYPE, FSTORE, SI(I_S_D)},
{"halt",   I_TYPE, HALT,   SI(I_HALT)},

{"daddi",  I_TYPE, REG2I,  SI(I_DADDI)},
{"daddui", I_TYPE, REG2I,  SI(I_DADDIU)},
{"andi",   I_TYPE, REG2I,  SI(I_ANDI)},
{"ori",    I_TYPE, REG2I,  SI(I_ORI)},
{"xori",   I_TYPE, REG2I,  SI(I_XORI)},
{"lui",    I_TYPE, REG1I,  SI(I_LUI)},

{"slti",   I_TYPE, REG2I,  SI(I_SLTI)},
{"sltiu",  I_TYPE, REG2I,  SI(I_SLTIU)},

{"beq",    I_TYPE, BRANCH,  SI(I_BEQ)},
{"bne",    I_TYPE, BRANCH,  SI(I_BNE)},
{"beqz",   I_TYPE, JREGN,   SI(I_BEQZ)},
{"bnez",   I_TYPE, JREGN,   SI(I_BNEZ)},


{"j",      J_TYPE, JUMP,   SI(I_J)},
{"jr",     R_TYPE, JREG,   SR(R_JR)},
{"jal",    J_TYPE, JUMP,   SI(I_JAL)},
{"jalr",   R_TYPE, JREG,   SR(R_JALR)},

{"dsll",   R_TYPE, REG2S,  SR(R_DSLL)},
{"dsrl",   R_TYPE, REG2S,  SR(R_DSRL)},
{"dsra",   R_TYPE, REG2S,  SR(R_DSRA)},
{"dsllv",  R_TYPE, REG3,   SR(R_DSLLV)},
{"dsrlv",  R_TYPE, REG3,   SR(R_DSRLV)},
{"dsrav",  R_TYPE, REG3,   SR(R_DSRAV)},
{"movz",   R_TYPE, REG3,   SR(R_MOVZ)},
{"movn",   R_TYPE, REG3,   SR(R_MOVN)},
{"nop",    R_TYPE, NOP,    SR(R_NOP)},
{"and",    R_TYPE, REG3,   SR(R_AND)},
{"or",     R_TYPE, REG3,   SR(R_OR)},
{"xor",    R_TYPE, REG3,   SR(R_XOR)},
{"slt",    R_TYPE, REG3,   SR(R_SLT)},
{"sltu",   R_TYPE, REG3,   SR(R_SLTU)},

{"dadd",   R_TYPE, REG3,   SR(R_DADD)},
{"daddu",  R_TYPE, REG3,   SR(R_DADDU)},
{"dsub",   R_TYPE, REG3,   SR(R_DSUB)},
{"dsubu",  R_TYPE, REG3,   SR(R_DSUBU)},

{"dmul",   R_TYPE, REG3,   SR(R_DMUL)},
{"dmulu",  R_TYPE, REG3,   SR(R_DMULU)},
{"ddiv",   R_TYPE, REG3,   SR(R_DDIV)},
{"ddivu",  R_TYPE, REG3,   SR(R_DDIVU)},

{"add.d",  F_TYPE, REG3F,   SF(F_ADD_D)},
{"sub.d",  F_TYPE, REG3F,   SF(F_SUB_D)},
{"mul.d",  F_TYPE, REG3F,   SF(F_MUL_D)},
{"div.d",  F_TYPE, REG3F,   SF(F_DIV_D)},
{"mov.d",  F_TYPE, REG2F,   SF(F_MOV_D)},
{"cvt.d.l",F_TYPE, REG2F,   SF(F_CVT_D_L)},
{"cvt.l.d",F_TYPE, REG2F,   SF(F_CVT_L_D)},
{"c.lt.d", F_TYPE, REG2C,   SF(F_C_LT_D)},
{"c.le.d", F_TYPE, REG2C,   SF(F_C_LE_D)},
{"c.eq.d", F_TYPE, REG2C,   SF(F_C_EQ_D)},

{"bc1f",   B_TYPE, BC,     SBC1F},
{"bc1t",   B_TYPE, BC,     SBC1T},
{"mtc1",   M_TYPE, REGID,  SMTC1},
{"mfc1",   M_TYPE, REGDI,  SMFC1},

{NULL,    0,      0,      0}
};

