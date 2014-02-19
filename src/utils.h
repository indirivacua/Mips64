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

#ifndef UTILS
#define UTILS

#include "mytypes.h"

extern int bits(int);
extern int sprintnbits(char *,WORD32,int);
extern WORD32 pack32(BYTE*);
extern void unpack32(WORD32,BYTE *);
extern SIGNED16 pack16(BYTE*);
extern void unpack16(SIGNED16,BYTE *);
extern WORD64 pack(BYTE*);
extern void unpack(WORD64,BYTE *);
extern void sprintword32(char *,WORD32);
extern void sprintdouble(char *,double);
extern void sprintword(char *,WORD64);
extern BOOL in_range(WORD32,WORD32);
extern BOOL delimiter(int);
extern int compare(const char*,const char *);
extern const char *skip(const char *,char);
extern const char *skipover(const char *,char);
extern int is_symbol(const char *);
extern const char *eatwhite(const char *);
extern int fgetreg(const char *&);
extern int alignment(int,int);
extern int getreg(const char *&);
extern WORD64 strtoint64(char *,char **,int);
extern BOOL getdouble(const char *&ptr,double *);
extern BOOL getfullnum(const char *&,WORD64 *);
extern BOOL getnum(const char *&,WORD32 *);
extern BOOL getsym(symbol_table *,int,const char *&,WORD32 *);
extern void init_processor(processor *,int,int);
extern void init_pipeline(pipeline *,int,int,int);
extern int clock_tick(pipeline *,processor *,BOOL,BOOL,BOOL,RESULT *);

#endif
