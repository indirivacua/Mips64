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


#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "simulator.h"
#include "language.h"
#include "utils.h"

int delimiter(int c) {
    if (c == ';' || c == '#')
      return COMMENT;
    if (c == 0 || c == '\n' || c == '\r')
      return ENDLINE;
    if (c == ' ' || c == 9)
      return SPACE;
    return 0;
}

const char *skipover(const char *ptr, char c) {
  // skip over anything to get to c

  int res;
  while (*ptr != c) {
    res = delimiter(*ptr);
    if (res == ENDLINE || res == COMMENT)
      return NULL;
    if (c == ',' && *ptr == '(')
      return NULL;
    ptr++;
  }
  ptr++;

  return ptr;
}

const char *skip(const char *ptr, char c) {
  // skip over next c

  ptr = eatwhite(ptr);
  if (ptr == NULL)
    return NULL;

  if (*ptr != c)
    return NULL;

  ptr++;
  return ptr;
}

const char *eatwhite(const char *ptr) {
  if (ptr == NULL)
    return NULL;

  while (*ptr == ' ' || *ptr == 9)
    ptr++;

  if (*ptr == 0    ||
      *ptr == ';'  ||
      *ptr == '#'  ||
      *ptr == '\n' ||
      *ptr == '\r')
    return NULL;

  return ptr;
}

BOOL getdouble(const char *&ptr, double *num) {
  double m;
  ptr = eatwhite(ptr);
  if (ptr == NULL)
    return FALSE;

  //  if (*ptr == '#') ptr++;   /* skip over # */
  if (!isdigit(*ptr) && *ptr != '.' && *ptr != '-' && *ptr != '+')
    return FALSE;

  char *ptr_2;
  m = strtod(ptr, &ptr_2);
  ptr = ptr_2;

  *num = m;
  return TRUE;
}

int is_symbol(const char *start) {
  int len = 0;
  const char *ptr = start;
  while (!delimiter(*ptr)) {
    ptr++;
    len++;
  }
  ptr--;
  len--;
  if (*ptr == ':')
    return len;
  return 0;
}

int getreg(const char *&ptr) {
  int n, ch, type;
  ptr = eatwhite(ptr);
  if (ptr == NULL)
    return -1;

  ch = tolower(*ptr);
  if (ch != 'r' && ch != '$')
    return -1;

  ptr++;
  if (ch == 'r' && !isdigit(*ptr))
    return -1;

  /* check for register pseudo-name */
  type = 0;
  if (strncmp(ptr,"zero", 4) == 0) {
    n = 0;
    ptr += 4;
    return n;
  }
  if (strncmp(ptr,"at", 2) == 0) {
    n = 1;
    ptr += 2;
    return n;
  }
  if (strncmp(ptr,"gp", 2) == 0) {
    n = 28;
    ptr += 2;
    return n;
  }
  if (strncmp(ptr,"sp", 2) == 0) {
    n = 29;
    ptr += 2;
    return n;
  }
  if (strncmp(ptr,"fp", 2) == 0) {
    n = 30;
    ptr += 2;
    return n;
  }
  if (strncmp(ptr,"ra", 2) == 0) {
    n = 31;
    ptr += 2;
    return n;
  }
  if (strncmp(ptr,"v", 1) == 0) {
    type = 1;
    ptr++;
  }
  if (strncmp(ptr,"a", 1) == 0) {
    type = 2; ptr++;
  }
  if (strncmp(ptr,"t", 1) == 0) {
    type = 3;
    ptr++;
  }
  if (strncmp(ptr,"s", 1) == 0) {
    type = 4;
    ptr++;
  }
  if (strncmp(ptr,"k", 1) == 0) {
    type = 5;
    ptr++;
  }

  n= (int) atoi(ptr);
  ptr++;
  while (isdigit(*ptr))
    ptr++;

  switch (type) {
  case 1:
    if (n < 0 || n > 1)
      return -1;
    n += 2;
    break;
  case 2:
    if (n < 0 || n > 3)
      return -1;
    n += 4;
    break;
  case 3:
    if (n < 0 || n > 9)
      return -1;
    if (n < 8)
      n += 8;
    else
      n += 16;
    break;
  case 4:
    if (n < 0 || n > 7)
      return -1;
    n += 16;
    break;
  case 5:
    if (n < 0 || n > 1)
      return -1;
    n += 26;
    break;

  default:
    break;
  }

  while (n < 0 || n > 31)
    return -1;

  return n;
}

int fgetreg(const char *&ptr) {
  int n;
  ptr = eatwhite(ptr);
  if (ptr == NULL)
    return -1;

  if (tolower(*ptr) != 'f')
    return -1;

  ptr++;
  if (!isdigit(*ptr))
    return -1;
  n = (int)atoi(ptr);
  ptr++;
  while (isdigit(*ptr))
    ptr++;

  if (n < 0 || n > 31)
    return -1;
  return n;
}

BOOL getnum(const char *&ptr, WORD32 *n) {
  int sign = 1;
  ptr = eatwhite(ptr);
  if (ptr == NULL)
    return FALSE;
  //if (*ptr == '#') ptr++;   // skip over #
  if (!isdigit(*ptr) && *ptr != '-' && *ptr != '+')
    return FALSE;

  if (*ptr == '-') {
    sign=-1;
    ptr++;
  }
  if (*ptr == '+')
    ptr++;

  char *ptr_2;
  *n = (int) strtoul(ptr, &ptr_2, 0);
  ptr = ptr_2;

  *n *= sign;

  return TRUE;
}

BOOL getfullnum(const char *&ptr, WORD64 *num) {
  WORD64 m;
  *num = 0;
  ptr = eatwhite(ptr);
  if (ptr == NULL)
    return FALSE;

   //if (*ptr == '#') ptr++;   // skip over #
  if (!isdigit(*ptr) && *ptr != '-' && *ptr != '+')
    return FALSE;

  m = strtoint64(ptr, &ptr, 0);
  *num = m;
  return TRUE;
}



/*
   get symbol from a symbol table
   This should use a hash table....
*/
BOOL getsym(symbol_table *table, int size, const char *&ptr, WORD32 *n) {
  int k, incr;
  WORD32 m;
  char *sptr;
  ptr = eatwhite(ptr);
  if (ptr == NULL)
    return FALSE;

  if (getnum(ptr, &m)) { // its a number!
    *n = m;
    return TRUE;
  }

  for (k = 0; k < size; k++) {
    sptr = table[k].symb;
    incr = compare(ptr, sptr);

    if (incr) {
      *n = table[k].value;
      // check for + or - an integer
      ptr += incr;
      if (eatwhite(ptr) == NULL)
        return TRUE; // nothing else on the line
      ptr = eatwhite(ptr);
      // move up to wahtever it is...
      if (*ptr == '-' || *ptr == '+') {
        if (getnum(ptr, &m))
          *n += m;
      }
      return TRUE;
    }
  }
  return FALSE;
}

Assembler::Assembler(CodeMemory *code, DataMemory *data) {
  this->code = code;
  this->data = data;
}

int Assembler::openit(const std::string &fname) {
  int i, j, k, gotline, lineptr, errors;
  char preline[MAX_LINE + 1];
  char line[MAX_LINE + 1];
  FILE *asmfile;

  errors = 0;
  CODEORDATA = 0;
  codeptr = 0;
  dataptr = 0;

  code_symptr = 0;
  data_symptr = 0;

  asmfile = fopen(fname.c_str(), "rb");
  if (asmfile == NULL)
    return 1;

  for (lineptr = 1; ; lineptr++) {
    gotline = mygets(line, MAX_LINE, asmfile);
    if (gotline == 1)
      break;
    if (gotline == -1) { // something got chopped
      errors++;
    }
    errors += first_pass(line, lineptr);
    if (errors)
      break;
    dataptr = alignment(dataptr, STEP);
  }
  if (errors) {
    fclose(asmfile);
    return 2; // errors on pass 1
  }

  CODEORDATA = 0;
  codeptr = 0;
  dataptr = 0;
  rewind(asmfile);
  for (lineptr = 1; ; lineptr++) {
    if (mygets(preline, MAX_LINE, asmfile) == 1) // ***
      break;
    preline[strlen(preline) - 1] = 0; // remove CR/LF

    for (i = j = 0; ; i++) {
      // replace Tabs with 2 spaces
      if (preline[i] == '\t')
        for (k = 0; k < 2; k++)
          line[j++] = ' ';
        else
          line[j++] = preline[i];
      if (preline[i] == 0) {
        // pad it out with spaces...
        line[j - 1] = ' ';
        for ( ; j < MAX_LINE; j++)
          line[j] = ' ';
        line[MAX_LINE] = 0;
        break;
      }
    }
    errors += second_pass(line, lineptr);
    dataptr = alignment(dataptr, STEP);
  }
  fclose(asmfile);

  if (errors)
    return 3; // errors on pass 2
  return 0;
}

// fill in symbol tables and check for syntax errors

int Assembler::first_pass(const char *line, int lineptr) {
  int i, len;
  const char *ptr = line;
  char txt[100];

  ptr = eatwhite(ptr);
  if (ptr == NULL)
    return 0;

  if (delimiter(*ptr))  /* blank line */
    return 0;

  while ((len = is_symbol(ptr)) > 0) {
    //  copy symbol into symbol table
    //  advance over symbol
    if (CODEORDATA == 0) {
      sprintf(txt, "Pasada 1 - Error en linea %d\n", lineptr);
      std::cout << txt << "\n";
      return 1;
    }

    if (CODEORDATA == CODE) {
      if (code_symptr >= SYMTABSIZE) {
        sprintf(txt,"Pasada 1 - Error en linea %d\nLa tabla de Simbolos de Codigo esta llena.", lineptr);
        std::cout << txt << "\n";
        return 1;
      }
      code_table[code_symptr].symb = (char *) malloc(len + 1);
      for (i = 0; i < len; i++)
        code_table[code_symptr].symb[i] = *ptr++;
      code_table[code_symptr].symb[len] = 0;
      code_table[code_symptr].value = codeptr;
      ptr++;   // skip over the ":"
      code_symptr++;
    }
    if (CODEORDATA == DATA) {
      if (data_symptr >= SYMTABSIZE) {
        sprintf(txt,"Pasada 1 - Error en linea %d\nLa tabla de Simbolos de Datos esta llena.", lineptr);
        std::cout << txt << "\n";
        return 1;
      }
      data_table[data_symptr].symb = (char *) malloc(len + 1);
      for (i = 0; i < len; i++)
        data_table[data_symptr].symb[i] = *ptr++;
      data_table[data_symptr].symb[len] = 0;
      data_table[data_symptr].value = dataptr;
      ptr++;           // skip over the ":"
      data_symptr++;
    }
    ptr = eatwhite(ptr);
    if (ptr == NULL)
      return 0;
  }

  if (instruction(ptr) >= 0) {
    // instruction found
    // increase instruction pointer

    if (CODEORDATA != CODE) {
      sprintf(txt,"Pasada 1 - Error en linea %d\n", lineptr);
      std::cout << txt << "\n";
      return 1;
    }
    codeptr = alignment(codeptr, 4);
    codeptr += 4;
    if (!code->isValidAddress(codeptr)) {
      sprintf(txt,"Pasada 1 - Error en linea %d\nNo existe esa ubicacion de memoria", lineptr);
      std::cout << txt << "\n";
      return 1;
    }
    return 0;
  }

  if (directive(1, ptr, line))
    return 0;

  sprintf(txt,"Pasada 1 - Error en linea %d\n", lineptr);
  std::cout << txt << "\n";
  return 1;
}

int Assembler::second_pass(const char *line, int /* lineptr */) {
  WORD32 w/*, byte*/;
  WORD32 op, code_word = 0;
  WORD32 flags = 0;
  int instruct;
  const char *start, *end, *fin;
  int rs, rt, rd, sub, type;
  BOOL /*sign, */error = TRUE;

  const char *ptr = line;
  ptr = eatwhite(ptr);
  if (ptr == NULL) {
    return 0;  /* blank line */
  }

  // skip over any symbols on the line
  while (is_symbol(ptr)) {
    while (*ptr != ':')
      ptr++;
    ptr++;
    ptr = eatwhite(ptr);
    if (ptr == NULL)
      break;
  }

  instruct = instruction(ptr);
  if (instruct < 0) {
    // no instruction on the line, perhaps a directive?
    if (!directive(2, ptr, line)) {
      if (ptr != NULL && CODEORDATA == DATA)
        return 1;
    }
    return 0;
  }

  // instruction found - parse it out
  start = ptr;
  op = codes[instruct].op_code;
  sub = codes[instruct].subtype;
  type = codes[instruct].type;

  while (!delimiter(*ptr)) // advance over instruction
    ptr++;
  //ptr = eatwhite(ptr);
  fin = ptr;
  rs = rt = rd = 0;
  w = 0; /*byte = 0;*/ flags = 0;
  /*sign = TRUE;*/

  switch (sub) {
  case NOP:
  case HALT:
    if (eatwhite(ptr) != NULL)
      break;
    error = FALSE;
    break;

  case STORE:
  case LOAD:
    rt = getreg(ptr);
    if (rt < 0)
      break;
    ptr = skip(ptr, ','); if (eatwhite(ptr) == NULL) break;  /* skip over ,  */
    ptr = eatwhite(ptr);
    if (*ptr == '(') w = 0;
    else if (!getdatasym(ptr, &w)) break;
    ptr = skip(ptr, '('); if (eatwhite(ptr) == NULL) break;  /* skip over (  */
    ptr = eatwhite(ptr);
    rs = getreg(ptr);    if (rs < 0) break;

    ptr = skip(ptr, ')');                                  /* skip over ) */
    if (ptr == NULL) break;
    if (eatwhite(ptr) != NULL) break;

    error = FALSE;
    break;

  case FSTORE:
  case FLOAD:
    rt = fgetreg(ptr);    if (rt < 0) break;
    ptr = skip(ptr, ','); if (eatwhite(ptr) == NULL) break;  /* skip over , */
    ptr = eatwhite(ptr);
    if (*ptr == '(') w = 0;
    else if (!getdatasym(ptr, &w)) break;
    ptr = skip(ptr, '('); if (eatwhite(ptr) == NULL) break;  /* skip over ( */
    ptr = eatwhite(ptr);
    rs = getreg(ptr);    if (rs < 0) break;
    ptr = skip(ptr, ')');                                  /* skip over ) */
    if (ptr == NULL) break;
    if (eatwhite(ptr) != NULL) break;
    error = FALSE;
    break;

  case REG2I:
    rt = getreg(ptr); if (rt < 0) break;
    ptr = skip(ptr, ','); if (eatwhite(ptr) == NULL) break;  /* skip over , */
    ptr = eatwhite(ptr);
    rs = getreg(ptr); if (rs < 0) break;
    ptr = skip(ptr, ','); if (eatwhite(ptr) == NULL) break;  /* skip over , */
    ptr = eatwhite(ptr);
    if (!getdatasym(ptr, &w)) break;
    if (eatwhite(ptr) != NULL) break;
    error = FALSE;
    break;

  case REG1I:
    rt = getreg(ptr); if (rt < 0) break;
    ptr = skip(ptr, ','); if (eatwhite(ptr) == NULL) break;  /* skip over , */
    ptr = eatwhite(ptr);
    if (!getdatasym(ptr, &w)) break;
    if (eatwhite(ptr) != NULL) break;
    error = FALSE;
    break;

  case JREG:
    rt = getreg(ptr); if (rt < 0) break;
    if (eatwhite(ptr) != NULL) break;
    error = FALSE;
    break;

  case REG3:
    rd = getreg(ptr); if (rd < 0) break;
    ptr = skip(ptr, ','); if (eatwhite(ptr) == NULL) break;  /* skip over ,  */
    ptr = eatwhite(ptr);
    rs = getreg(ptr); if (rs < 0) break;
    ptr = skip(ptr, ','); if (eatwhite(ptr) == NULL) break;  /* skip over ,  */
    ptr = eatwhite(ptr);
    rt = getreg(ptr); if (rt < 0) break;
    if (eatwhite(ptr) != NULL) break;
    error = FALSE;
    break;

  case REG3F:
    rd = fgetreg(ptr); if (rd < 0) break;
    ptr = skip(ptr, ','); if (eatwhite(ptr) == NULL) break;  /* skip over ,  */
    ptr = eatwhite(ptr);
    rs = fgetreg(ptr); if (rs < 0) break;
    ptr = skip(ptr, ','); if (eatwhite(ptr) == NULL) break;  /* skip over ,  */
    ptr = eatwhite(ptr);
    rt = fgetreg(ptr); if (rt < 0) break;
    if (eatwhite(ptr) != NULL) break;
    error = FALSE;
    break;

  case REG2F:
    rd = fgetreg(ptr); if (rd < 0) break;
    ptr = skip(ptr, ','); if (eatwhite(ptr) == NULL) break;  /* skip over , */
    ptr = eatwhite(ptr);
    rs = fgetreg(ptr); if (rs < 0) break;
    if (eatwhite(ptr) != NULL) break;
    error = FALSE;
    break;

  case REG2C:
    rs = fgetreg(ptr); if (rs < 0) break;
    ptr = skip(ptr, ','); if (eatwhite(ptr) == NULL) break;  /* skip over , */
    ptr = eatwhite(ptr);
    rt = fgetreg(ptr); if (rt < 0) break;
    if (eatwhite(ptr) != NULL) break;
    error = FALSE;
    break;

  case REGID:
  case REGDI:
    rt = getreg(ptr); if (rt < 0) break;
    ptr = skip(ptr, ','); if (eatwhite(ptr) == NULL) break;  /* skip over , */
    ptr = eatwhite(ptr);
    rd = fgetreg(ptr); if (rd < 0) break;
    if (eatwhite(ptr) != NULL) break;
    error = FALSE;
    break;

  case REG2S:
    rd = getreg(ptr); if (rd < 0) break;
    ptr = skip(ptr, ','); if (eatwhite(ptr) == NULL) break;  /* skip over , */
    ptr = eatwhite(ptr);
    rs = getreg(ptr); if (rs < 0) break;
    ptr = skip(ptr, ','); if (eatwhite(ptr) == NULL) break;  /* skip over , */
    ptr = eatwhite(ptr);
    if (!getdatasym(ptr, &flags)) break;
    if (eatwhite(ptr) != NULL) break;
    error = FALSE;
    break;

  case JUMP:
  case BC:
    if (!getcodesym(ptr, &w)) break;
    w -= (codeptr + 4);   /* relative jump */
    w  = (SIGNED32) w/4;
    if (eatwhite(ptr) != NULL) break;
    error = FALSE;
    break;

  case BRANCH:
    rt = getreg(ptr); if (rt < 0) break;
    ptr = skip(ptr, ','); if (eatwhite(ptr) == NULL) break;
    ptr = eatwhite(ptr);
    rs = getreg(ptr); if (rs < 0) break;
    ptr = skip(ptr, ','); if (eatwhite(ptr) == NULL) break;
    ptr = eatwhite(ptr);
    if (!getcodesym(ptr, &w)) break;
    w -= (codeptr + 4);
    w  = (SIGNED32) w/4;
    if (eatwhite(ptr) != NULL) break;
    error = FALSE;
    break;

  case JREGN:
    rt = getreg(ptr); if (rt < 0) break;
    ptr = skip(ptr, ','); if (eatwhite(ptr) == NULL) break;
    ptr = eatwhite(ptr);
    if (!getcodesym(ptr, &w)) break;
    w -= (codeptr + 4);
    w  = (SIGNED32) w/4;
    if (eatwhite(ptr) != NULL) break;
    error = FALSE;
    break;

  default:
    error = FALSE;
    break;
  }

  if (!error)
    switch (type) {
    case I_TYPE:
      if (in_range(w, 0xffff))
        code_word = op | rs << 21 | rt << 16 | (w & 0xffff);
      else error = TRUE;
      break;
    case R_TYPE:
      if (in_range(flags, 0x1F))
        code_word = (op | rs << 21 | rt << 16 | rd << 11 | flags << 6);
      else error = TRUE;
      break;
    case J_TYPE:
      if (in_range(w, 0x3ffffff)) code_word = (op | (w & 0x3ffffff));
      else error = TRUE;
      break;
    case F_TYPE:
      code_word = (op | rs << 11 | rt << 16 | rd << 6);
      break;
    case M_TYPE:
      code_word = (op | rt << 16 | rd << 11);
      break;
    case B_TYPE:
      if (in_range(w, 0xffff)) code_word = (op | (w&0xffff));
      else error = TRUE;
      break;
    }

  codeptr = alignment(codeptr, 4);

  std::string assembly;
  std::string mnemonic;
  int ret_val = 0;
  if (error) {
    code_word = 0;
    ret_val = 1;
  }

  if (ptr != NULL) { // its crap...
    end = ptr;

    int len = end - start;
    if (len > 25)
      len = 25;

    std::string str(start, len);
    assembly = str;

    len = fin - start;

    if (len > 7)
      len = 7;

    std::string str1(start, len);
    std::transform(str1.begin(), str1.end(), str1.begin(), tolower);

    mnemonic = str1;
  }

  code->writeInstruction(codeptr, code_word, line, assembly, mnemonic);
  if (error)
    code->invalidateInstruction(codeptr);
  codeptr += 4;

  return ret_val;
}

BOOL Assembler::getcodesym(const char *&ptr, WORD32 *m) {
  return getsym(code_table, code_symptr, ptr, m);
}

BOOL Assembler::getdatasym(const char *&ptr, WORD32 *m) {
  return getsym(data_table, data_symptr, ptr, m);
}

int Assembler::instruction(const char *start) {
  int i = 0;
  char text[10];    /* all instructions are 6 chars or less */
  const char *ptr = start;
  if (ptr == NULL)
    return -1;
  while (i < 10 && !delimiter(*ptr))  {
    text[i++]= (unsigned char) tolower(*ptr);
    ptr++;
  }
  if (i > 9)
    return -1;

  text[i] = 0;    /* terminate it */

  for (i = 0; ; i++) {
    if (codes[i].name == NULL)
      return -1;
    if (strcmp(text, codes[i].name) == 0)
      break;
  }
  return i;
}

BOOL Assembler::directive(int pass, const char *ptr, const char *line) {
  // process assembler directives. return number of bytes consumed

  BOOL zero, bs;
  WORD32 num, m;
  WORD64 fw;
  DOUBLE64 db;
  int k;
  int sc, ch;
  const char *iptr;

  if (ptr == NULL || *ptr != '.')
    return FALSE;

  for (k = 0; ; k++) {
    if (directives[k] == NULL)
      return FALSE;
    if (!compare(ptr, directives[k]))
      continue;
    break;
  }
  while (!delimiter(*ptr))
    ptr++;
  zero = TRUE;
  switch (k) {
  case 0:     // .space
    if (CODEORDATA == CODE) return FALSE;
    if (!getnum(ptr, &num)) return FALSE;
    if (num == 0) return FALSE;
    if (CODEORDATA == DATA) {
      if (pass == 2 && data->isValidAddress(dataptr))
        data->setAddressDescription(dataptr, line);
      dataptr += num;
      //        if (dataptr>DATASIZE) return FALSE;
    }

    return TRUE;
    break;

  case 5:     // .ascii
    zero = FALSE;
  case 1:     // .asciiz
    if (CODEORDATA == CODE)
      return FALSE;
    ptr = eatwhite(ptr);
    if (ptr == NULL)
      return -1;
    if (*ptr != '"' && *ptr != '\'')
      return -1;
    sc = *ptr;    // character to indicate end of string

    ++ptr;

    num = 0;
    iptr = ptr;
    bs = FALSE;
    while (*iptr != sc) {
      if (delimiter(*iptr) == ENDLINE)
        return FALSE;

      if (bs) {
        num++;
        bs = FALSE;
      } else {
        if (*ptr == '\\')
          bs = TRUE;
        else
         num++;
      }
      iptr++;
    }
    if (CODEORDATA == DATA) {
      if (pass == 1)  {
        dataptr += num;
        if (!data->isValidAddress(dataptr))
          return FALSE;
      }
      if (pass == 2) {
        data->setAddressDescription(dataptr, line);

        m = 0;
        bs = FALSE;
        while (m < num) {
          m++;
          if (bs) {
            if (*ptr == 'n')
              ch = '\n';
            else
              ch = *ptr;
            data->writeByte(dataptr, ch);
            ++dataptr;
            bs = FALSE;
          } else {
            if (*ptr == '\\')
              bs = TRUE;
            else {
              data->writeByte(dataptr, *ptr);
              ++dataptr;
            }
          }
          ptr++;
        }
        if (zero) {
          data->writeByte(dataptr, 0);
          ++dataptr;
        }
      }
    }
    return TRUE;

  case 2:  // .align
    if (!getnum(ptr, &num))
      return FALSE;
    if (num < 2 || num > 16)
      return FALSE;
    if (CODEORDATA == CODE)
      return FALSE;
    if (CODEORDATA == DATA) {
      dataptr = alignment(dataptr, num);
      if (pass == 2)
        data->setAddressDescription(dataptr, line);
    }
    return TRUE;

  case 3:  // .word
    if (CODEORDATA == CODE)
      return FALSE;
    if (pass == 1) {
      if (CODEORDATA == DATA) {
        do {
          dataptr += STEP;
        } while ((ptr = skipover(ptr, ',')) != NULL);

        if (!data->isValidAddress(dataptr))
          return FALSE;
      }
    }
    if (pass == 2) {
      if (!getfullnum(ptr, &fw))
        return FALSE;
      if (CODEORDATA == DATA) {
        data->setAddressDescription(dataptr, line);
        data->writeWord64(dataptr, fw);
        dataptr += 8;
        while ((ptr = skip(ptr, ',')) != NULL) {
          if (!getfullnum(ptr, &fw))
            return FALSE;
          //printf("         %08x\n", num);
          data->writeWord64(dataptr, fw);
          dataptr += 8;
        }
      }
    }
    return TRUE;

  case 10: // .word32
    if (CODEORDATA == CODE)
      return FALSE;
    if (pass == 1) {
      if (CODEORDATA == DATA) {
        do {
          dataptr += 4;
        } while ((ptr = skipover(ptr, ',')) != NULL);
        if (!data->isValidAddress(dataptr))
          return FALSE;
      }
    }
    if (pass == 2) {
      if (!getdatasym(ptr, &num))
        return FALSE;
      if (CODEORDATA == DATA) {
        data->setAddressDescription(dataptr, line);
        data->writeWord32(dataptr, num);
        dataptr += 4;
        while ((ptr = skip(ptr, ',')) != NULL) {
          if (!getdatasym(ptr, &num))
            return FALSE;
          data->writeWord32(dataptr, num);
          dataptr += 4;
        }
      }
    }
    return TRUE;

  case 11: // .word16
    if (CODEORDATA == CODE)
      return FALSE;
    if (pass == 1) {
      if (CODEORDATA == DATA) {
        do {
          dataptr += 2;
        } while ((ptr = skipover(ptr, ',')) != NULL);
        if (!data->isValidAddress(dataptr))
          return FALSE;
      }
    }
    if (pass == 2) {
      if (!getnum(ptr, &num))
        return FALSE;
      if (!in_range(num, 0xffff))
        return FALSE;
      if (CODEORDATA == DATA) {
        data->setAddressDescription(dataptr, line);
        data->writeHalf(dataptr, num);
        dataptr += 2;
        while ((ptr = skip(ptr, ',')) != NULL) {
          if (!getnum(ptr, &num))
            return FALSE;
          if (!in_range(num, 0xffff))
            return FALSE;
          data->writeHalf(dataptr, num);
          dataptr += 2;
        }
      }
    }
    return TRUE;

  case 4:            // .byte
    if (CODEORDATA == CODE)
      return FALSE;
    if (pass == 1) {
        if (CODEORDATA == DATA) {
          do {
            dataptr += 1;
          } while ((ptr = skipover(ptr, ',')) != NULL);
          if (!data->isValidAddress(dataptr))
            return FALSE;
        }
    }
    if (pass == 2) {
      if (!getnum(ptr, &num))
        return FALSE;
      if (!in_range(num, 0xff))
        return FALSE;
      if (CODEORDATA == DATA) {
        data->setAddressDescription(dataptr, line);
        data->writeByte(dataptr, (unsigned char) num);
        ++dataptr;
        while ((ptr = skip(ptr, ',')) != NULL) {
          if (!getnum(ptr, &num))
            return FALSE;
          if (!in_range(num, 0xff))
            return FALSE;
          data->writeByte(dataptr, (unsigned char) num);
          ++dataptr;
        }
      }
    }
    return TRUE;

  case 6:      // .global
  case 7:      // .data
    CODEORDATA = DATA;
    if (pass == 1) {
        if (eatwhite(ptr) != NULL)
         return FALSE;
    }
    return TRUE;

  case 12:        // double (64 bits)
    if (CODEORDATA == CODE)
      return FALSE;
    if (pass == 1) {
      if (CODEORDATA == DATA) {
        do {
          dataptr += STEP;
        } while ((ptr = skipover(ptr, ',')) != NULL);
        if (!data->isValidAddress(dataptr))
          return FALSE;
      }
    }
    if (pass == 2) {
      if (!getdouble(ptr, &db.d))
        return FALSE;
      if (CODEORDATA == DATA) {
        data->setAddressDescription(dataptr, line);
        //    printf("%08x %08x %s", dataptr, num, line);
        data->writeWord64(dataptr, db.u);
        dataptr += 8;
            while ((ptr = skip(ptr, ',')) != NULL) {
          if (!getdouble(ptr, &db.d)) return FALSE;
          //   printf("         %08x\n", num);
                data->writeWord64(dataptr, db.u);
                dataptr += 8;
              }
          }

      }
    return TRUE;

  case 13:
  case 8:      // .text
    CODEORDATA = CODE;
    if (pass == 1) {
        if (eatwhite(ptr) != NULL)
           return FALSE;
    }
    return TRUE;

  case 9:    // .org
    if (CODEORDATA == DATA) {
        if (!getnum(ptr, &num))
          return FALSE;
        if (num < dataptr /* || num>DATASIZE*/ )
          return FALSE;

        dataptr = alignment(num, STEP);
        //if (pass == 2)
        //  printf("%08x          %s", dataptr, line);
        return TRUE;
    }
    if (CODEORDATA == CODE) {
      if (!getnum(ptr, &num))
        return FALSE;
      if (num < codeptr || code->isValidAddress(num))
        return FALSE;
      codeptr = alignment(num, 4);
      //if (pass == 2)
      //  printf("%08x          %s", codeptr, line);
      return TRUE;
    }
    return FALSE;

  default:
    break;
  }

  return FALSE;
}

//
// This function reads a line even if not termimated by CR
//
int Assembler::mygets(char *line, int max, FILE *fp) {
  if (feof(fp)) {
    return 1;
  }
  fgets(line, max, fp);
  return 0;
}

