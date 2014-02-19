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

#include "mytypes.h"

int bits(int);
int sprintnbits(char *,WORD32,int);
WORD32 pack32(BYTE*);
void unpack32(WORD32,BYTE *);
SIGNED16 pack16(BYTE*);
void unpack16(SIGNED16,BYTE *);
WORD64 pack(BYTE*);
void unpack(WORD64,BYTE *);
void sprintword32(char *,WORD32);
void sprintdouble(char *,double);
void sprintword(char *,WORD64);
BOOL in_range(WORD32,WORD32);
BOOL delimiter(int);
int compare(const char*,const char *);
const char *skip(const char *,char);
const char *skipover(const char *,char);
int is_symbol(const char *);
const char *eatwhite(const char *);
int fgetreg(const char *&);
int alignment(int,int);
int getreg(const char *&);
WORD64 strtoint64(char *,char **,int);
BOOL getdouble(const char *&ptr,double *);
BOOL getfullnum(const char *&,WORD64 *);
BOOL getnum(const char *&,WORD32 *);
BOOL getsym(symbol_table *,int,const char *&,WORD32 *);
void init_processor(processor *,int,int);
void init_pipeline(pipeline *,int,int,int);
int clock_tick(pipeline *,processor *,BOOL,BOOL,BOOL,RESULT *);

#endif
