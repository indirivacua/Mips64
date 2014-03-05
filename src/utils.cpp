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

#include "mytypes.h"
#include "utils.h"

BOOL in_range(WORD32 num, WORD32 mask) {
  // check that num will fit into the number of bits in mask
  int n = num;
  if (n >= 0 && (num & mask) != num)
    return FALSE;
  if (n < 0  && (num | mask) != 0xFFFFFFFF)
    return FALSE;
  return TRUE;
}

/* align objects on num-byte boundaries */
int alignment(int ptr, int num) {
    int t, r = ptr;
    t = r % num;
    if (t > 0)
      r += num - t;
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
    int ch, s = 0;
    int base = 10;

    while (*ptr == ' ' || *ptr == 9)
      ptr++;
    if (*ptr == '-') {
      s = 1;
      ptr++;
    } else if (*ptr == '+')
      ptr++;
    while (*ptr == ' ' || *ptr == 9)
      ptr++;

    if (b > 0)
      base = b;
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
        if (ch < '0' || ch > '7')
          break;
        n = n * base + (ch - '0');
      }
      if (base == 10) {
        if (ch < '0' || ch > '9')
          break;
        n = n * base + (ch - '0');
      }
      if (base == 16) {
        if ((ch < '0' || ch > '9') &&
            (ch < 'A' || ch > 'F') &&
            (ch < 'a' || ch > 'f'))
          break;
        if (ch >= '0' && ch <= '9')
          n = n * base + (ch - '0');
        if (ch >= 'A' && ch <= 'F')
          n = n * base + 10 + (ch - 'A');
        if (ch >= 'a' && ch <= 'f')
          n = n * base + 10 + (ch - 'a');
      }
      ptr++;
    }

    if (end != NULL)
      *end=ptr;

    if (s == 1)
      return (~n) + 1;

    return n;
}


