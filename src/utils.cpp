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
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>

#include "utils.h"

/* Architectural globals */

int bits(int num) {
  int r = 0;
  while (num >>= 1) 
    r++;
  return r;
}

// print out nbits, rounded up to nearest byte

int sprintnbits(char *txt,WORD32 addr,int n) {
  int i,nbytes = n/8;
  if (n%8 != 0)
    nbytes++;
  BYTE a[4];
  unpack32(addr,a);
  for (i=0; i<nbytes; i++) 
    sprintf(&txt[2*i], "%02x", a[nbytes-i-1]);
  return nbytes;
}


SIGNED16 pack16(BYTE *b) {
  /* pack bytes into 16-bit half-word */
  SIGNED16 res = (SIGNED16) b[0];
  res |= (SIGNED16) (b[1] << 8);
  return res;
}

void unpack16(SIGNED16 a, BYTE *b) {
  /* unpack bytes from a half-word */
  b[0] = (BYTE) a;
  b[1] = (BYTE) (a>>8);
}


WORD32 pack32(BYTE *b) {
  /* pack bytes into a 32-bit Word */
  return 
    ((WORD32)b[3]<<24) | 
    ((WORD32)b[2]<<16) | 
    ((WORD32)b[1]<<8) | 
    (WORD32)b[0];
}

void unpack32(WORD32 a,BYTE *b) {
  /* unpack bytes from a word */
    b[0] = (BYTE) a;
    b[1] = (BYTE) (a>>8);
    b[2] = (BYTE) (a>>16);
    b[3] = (BYTE) (a>>24);
}

WORD64 pack(BYTE *b) {
  int i;
  WORD64 r = 0;
  for (i = STEP-1; i >= 0; i--) {
    r <<= 8;
    r |= b[i];
  }
  return r;
}

void unpack(WORD64 a, BYTE *b) {
  int i;
  for (i = 0; i < STEP; i++) {
    b[i] = (BYTE) a;
    a >>= 8;
  }
}

void sprintword32(char *txt, WORD32 addr) {
  sprintf(txt, "%08x", addr);
}

void sprintword(char *txt, WORD64 val) {
  int i;
  BYTE a[8];
  val &= MASK;
  unpack(val, a);
  for (i = 0; i < STEP; i++)
    sprintf(&txt[2*i], "%02x", a[STEP - i - 1]);
}

void sprintdouble(char *txt, double db) {
  sprintf(txt, "%016.8lf", db);
}

BOOL in_range(WORD32 num, WORD32 mask) {
  // check that num will fit into the number of bits in mask
  int n = num;
  if (n >= 0 && (num & mask) != num)
    return FALSE;
  if (n < 0  && (num | mask) != -1)
    return FALSE;
  return TRUE;
}

/* align objects on num-byte boundaries */
int alignment(int ptr,int num) {
    int t,r = ptr;
    t = r%num;
    if (t > 0)
      r += num-t;
    return r;
}

int compare(const char *ptr1, const char *ptr2) {
  // compare strings, up to 0 in the second with all of the first
  int incr = 0;
  if (ptr1 == NULL)
    return 0;

  while (*ptr2 == *ptr1) {
    if (*ptr2 == 0) 
      break;
    ptr1++; 
    ptr2++; 
    incr++;
  }    
  if (*ptr2 == 0 && !isalnum(*ptr1))
    return incr;

  return 0;      
}

WORD64 strtoint64(const char *ptr, const char **end, int b) {
  WORD64 n = 0;
    int ch,s = 0;
    int base = 10;

    while (*ptr == ' ' || *ptr == 9)
      ptr++;
    if (*ptr == '-') {
      s=1; 
      ptr++;
    } else if (*ptr=='+') 
      ptr++;
    while (*ptr == ' ' || *ptr == 9)
      ptr++;

    if (b > 0)
      base=b;
    else {
      if (*ptr == '0') {
	ptr++;
	if (*ptr == 'x' || *ptr == 'X') {
	  base = 16;
	  ptr++;
	} else
	  base = 8;
      }
    }
    for (;;) {
      ch=*ptr;
      if (base == 8) {
	if (ch<'0' || ch>'7')
	  break;
	n = n * base+(ch - '0'); 
      }
      if (base == 10) {
	if (ch < '0' || ch > '9')
	  break;
	n = n * base+(ch - '0');
      }
      if (base == 16) {
	if ((ch<'0' || ch>'9') && 
	    (ch < 'A' || ch > 'F') &&
	    (ch < 'a' || ch > 'f'))
	  break;
	if (ch >= '0' && ch <= '9') 
	  n = n * base+(ch - '0');
	if (ch >= 'A' && ch <= 'F') 
	  n = n * base + 10 + (ch - 'A');
	if (ch >= 'a' && ch <= 'f') 
	  n = n * base + 10 +(ch - 'a');
      }
      ptr++;
    }
    
    if (end != NULL)
      *end=ptr;
    
    if (s == 1)
      return (~n) + 1;
    
    return n;
}


