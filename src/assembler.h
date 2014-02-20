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

#endif
