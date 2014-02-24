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

#ifndef __ASSEMBLER_H
#define __ASSEMBLER_H

#include "CodeMemory.h"

#define SYMTABSIZE 1000

#define ENDLINE 1
#define SPACE   2
#define COMMENT 3

typedef struct {
    char *symb;
    BYTE type;
    WORD32 value;
} symbol_table;

BOOL getsym(symbol_table *,int,const char *&,WORD32 *);
int fgetreg(const char *&);
int getreg(const char *&);
int is_symbol(const char *);
BOOL getfullnum(const char *&,WORD64 *);
BOOL getnum(const char *&,WORD32 *);
BOOL delimiter(int);
const char *skip(const char *,char);
const char *skipover(const char *,char);
const char *eatwhite(const char *);
BOOL getdouble(const char *&ptr,double *);

class Assembler {

  public:
    Assembler(CodeMemory *code, DataMemory *data);
    int openit(const std::string &fname);

  protected:
    int first_pass(const char *line,int lineptr);
    int second_pass(const char *line,int /* lineptr */);
    BOOL getcodesym(const char *&ptr, WORD32 *m);
    BOOL getdatasym(const char *&ptr, WORD32 *m);
    int instruction(const char *start);
    BOOL directive(int pass,const char *ptr, const char *line);
    int mygets(char *line, int max, FILE *fp);


  int CODEORDATA;
  unsigned int code_symptr;
  unsigned int data_symptr;
  symbol_table code_table[SYMTABSIZE];
  symbol_table data_table[SYMTABSIZE];

  unsigned int codeptr;
  unsigned int dataptr;

  CodeMemory *code;
  DataMemory *data;
};

#endif

