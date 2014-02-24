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

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define __STDC_FORMAT_MACROS
#include <inttypes.h>

#include <iostream>

#include "mytypes.h"
#include "utils.h"
#include "Processor.h"
#include "pipeline.h"
#include "simulator.h"

#include "assembler.h"

/////////////////////////////////////////////////////////////////////////////
// Simulator

Simulator::Simulator(CPUConfig *config) : pipe(&cpu) {

  this->config = config;

  cpu.initialize(config);
  pipe.initialize(config);

  simulation_running = FALSE;
  restart = FALSE;
  clear();
}

Simulator::~Simulator() {
}

void Simulator::clear() {
	cycles = instructions = loads = stores = branch_taken_stalls = branch_misprediction_stalls = 0;
	raw_stalls = waw_stalls = war_stalls = structural_stalls = 0;
	cpu.setPC(0);
	entries = 1;
	offset = 0;
	history[0].IR = 0;
	history[0].start_cycle = 0;
	history[0].status[0].stage = IFETCH;
	multi = 5;
	stall_type = stalls = 0;
}

/////////////////////////////////////////////////////////////////////////////
// Simulator commands


void Simulator::OnFileReset() {
	 // Reset the processor
	cpu.reset();
	pipe.initialize(config);
}

void Simulator::OnFullReset() {
	 // Reset Data Memory as well

	pipe.initialize(config);

	cpu.reset(TRUE);
	clear();
}

void Simulator::check_stalls(int status, const char *str, int rawreg, char *txt) {
	char mess[100];
	if (status == RAW) {
		raw_stalls++;
		if (rawreg < 32)
			sprintf(mess,"  Atasco RAW en %s (R%d)",str,rawreg);
		else
			sprintf(mess,"  Atasco RAW en %s (F%d)",str,rawreg-32);
		strcat(txt,mess);
	}
	if (status == WAW) {
		waw_stalls++;
		if (rawreg < 32)
			sprintf(mess,"  Atasco WAW en %s (R%d)",str,rawreg);
		else
			sprintf(mess,"  Atasco WAW en %s (F%d)",str,rawreg-32);
		strcat(txt,mess);
	}
	if (status == WAR) {
		war_stalls++;
		if (rawreg < 32)
			sprintf(mess,"  Atasco WAR en %s (R%d)",str,rawreg);
		else
			sprintf(mess,"  Atasco WAR en %s (F%d)",str,rawreg-32);
		strcat(txt, mess);
	}
}

void Simulator::process_result(BOOL show)
{
	char txt[300];
	//CMainFrame* pFrame=(CMainFrame*) AfxGetApp()->m_pMainWnd;
	//CStatusBar* pStatus=&pFrame->m_wndStatusBar;
	BOOL something=FALSE;


	if (result.WB == OK || result.WB == HALTED)
  	instructions++;
	txt[0]=0;
	if (!config->delay_slot && result.ID == BRANCH_TAKEN_STALL) {
		something = TRUE;
		branch_taken_stalls++;
		strcat(txt,"  Atasco Branch Taken");
	}
	if (result.ID == BRANCH_MISPREDICTED_STALL) {
		something = TRUE;
		branch_misprediction_stalls++;
		strcat(txt,"  Atasco Branch Misprediction");
	}

	if (result.MEM == LOADS || result.MEM==DATA_ERR) 
		loads++;
	if (result.MEM==STORES)
		stores++;

	check_stalls(result.ID, "ID", result.idrr, txt);
	check_stalls(result.EX, "EX", result.exrr, txt);
	check_stalls(result.ADDER[0], "ADD", result.addrr, txt);
	check_stalls(result.MULTIPLIER[0], "MUL", result.mulrr, txt);
	check_stalls(result.DIVIDER, "DIV", result.divrr, txt);
	check_stalls(result.MEM, "MEM", result.memrr, txt);

	if (result.MEM!=RAW) {
		if (result.EX == STALLED) {
			structural_stalls++;
			strcat(txt,"  Atasco Estructural en EX");
		}
		if (result.DIVIDER == STALLED) {
			structural_stalls++;
			strcat(txt,"  Atasco Estructural en FP-DIV");
		}
		if (result.MULTIPLIER[config->MUL_LATENCY-1] == STALLED) {
			structural_stalls++;
			strcat(txt,"  Atasco Estructural en FP-MUL");
		}
		if (result.ADDER[config->ADD_LATENCY-1] == STALLED) {
			structural_stalls++;
			strcat(txt,"  Atasco Estructural en FP-ADD");
		}
	}
	if (result.IF == NO_SUCH_CODE_MEMORY) {
		strcat(txt,"  No existe esa direccion de codigo!");
		cpu.setStatus(HALTED);
	}
	if (result.EX == INTEGER_OVERFLOW) {
		strcat(txt,"  Desbordamiento de numero entero!");
	}
	if (result.DIVIDER == DIVIDE_BY_ZERO) {
		strcat(txt,"  Division por Cero en DIV!");
	}

	if (result.MEM == DATA_ERR) {
		strcat(txt,"  Memoria no inicializada en MEM!");
	}
	if (result.MEM==NO_SUCH_DATA_MEMORY) {
		strcat(txt,"  No existe esa direccion de datos!");
	}
	if (result.MEM==DATA_MISALIGNED) {
		strcat(txt, " Error Fatal - LOAD/StTORE de memoria mal alineado!");
	}
	if (show) {
/*
		if (txt[0]==0) pStatus->SetPaneText(0,"Listo");
		else pStatus->SetPaneText(0,txt);
*/
	}
}

int Simulator::update_io() {
	WORD32 func = *(WORD32 *)&cpu.mm[0];
	int x, y, status = 0;


	BYTE az[256];
	if (!func) 
		return status;

	char txt[30];
	DOUBLE64 fp;
	fp.u = *(WORD64 *)&cpu.mm[8]; 

	switch (func) {
	case (WORD32)1:
		sprintf(txt,"%" PRIu64 "\n",fp.u);
		cpu.writeTerminal(txt);
		//UpdateAllViews(NULL,2);
		break;
	case (WORD32)2:
		sprintf(txt,"%" PRIi64 "\n",fp.s);
		cpu.writeTerminal(txt);
		//UpdateAllViews(NULL,2);
		break;
	case (WORD32)3:
		sprintf(txt,"%lf\n",fp.d);
		cpu.writeTerminal(txt);
		//UpdateAllViews(NULL,2);
		break;
	case (WORD32)4:
		// need to test here if fp.u is a legal address!
		cpu.data->getAsciiz(fp.u, az, 255);

		if (fp.u<cpu.getDataMemorySize()) 
			cpu.writeTerminal(std::string((const char *)az));

		//UpdateAllViews(NULL,2);
		break;

	case (WORD32)5:

		y=(WORD32)((fp.u>>32)&255);
		x=(WORD32)((fp.u>>40)&255);
		cpu.drawit=TRUE;
//			char txt[80];
//			sprintf(txt,"%d %d",x,y);
//			AfxMessageBox(txt);

		if (x<GSXY && y<GSXY) {
			cpu.setScreenPixel(x,y, (WORD32)fp.u);
		}
		//UpdateAllViews(NULL,2);
		break;
	case (WORD32)6:
		cpu.clearTerminal();
		//UpdateAllViews(NULL,2);
		break;
	case (WORD32)7:
		cpu.clearScreen();
		cpu.drawit=FALSE;
		//UpdateAllViews(NULL,2);
		break;
	case (WORD32)8:
/*
		cpu.keyboard=1;
		status=1;
*/
		{
		  char line[MAX_PATH+1];
		  fgets(line, MAX_PATH, stdin); 
		  DOUBLE64 number;
		  if (strstr(line,"."))
				number.d=atof(line);
			else
				number.s=atoll(line);
	  	*(WORD64 *)&(cpu.mm[8]) = number.u; 
		}
		break;
	case (WORD32)9:
/*
		cpu.keyboard=2;
		status=1;
*/
{
		
	  	(cpu.mm[8]) = getchar(); 
}
		break;
	default:
		break;
	}

/*
	for (i=0;i<cpu->Terminal.GetLength();i++)
	{
		if (cpu->Terminal[i]=='\n')
		{
			ncols=0;
		}
		else
			ncols++;
	}
*/
	
//	UpdateAllViews(NULL,1L);

	*(WORD32 *)&cpu.mm[0]=0;
	return status;
}

void Simulator::update_history() {
	int substage,stage;
	unsigned int i,cc;
	WORD32 previous;
	BOOL passed;

	if (result.MEM != RAW) {
		if (result.EX == STALLED) 
			result.EX = STRUCTURAL;
		if (result.DIVIDER == STALLED) 
			result.DIVIDER = STRUCTURAL;
		if (result.MULTIPLIER[config->MUL_LATENCY-1] == STALLED) 
			result.MULTIPLIER[config->MUL_LATENCY-1] = STRUCTURAL;
		if (result.ADDER[config->ADD_LATENCY-1] == STALLED) 
			result.ADDER[config->ADD_LATENCY-1] = STRUCTURAL;
	}	

	for (i = 0; i < entries; i++) {
		previous = history[i].IR;
		cc = cycles-history[i].start_cycle;
		stage = history[i].status[cc-1].stage; // previous stage
		substage = history[i].status[cc-1].substage;
	
		switch (stage) {

		case IFETCH:
			if (pipe.if_id.active)	{
				if (pipe.if_id.IR == previous) {

					history[i].status[cc].stage = IDECODE;
					history[i].status[cc].cause = 0;
				} else {
					history[i].status[cc].stage = IFETCH;
					history[i].status[cc].cause = (BYTE) result.IF;
				}
			} else {
				history[i].status[cc].stage = 0;
				history[i].status[cc].cause = 0;
			}
			break;
		case IDECODE:
			passed = FALSE;
			
			if (pipe.integer.active && pipe.integer.IR == previous && result.ID != STALLED) {
				passed = TRUE;
				history[i].status[cc].stage = INTEX;
				history[i].status[cc].cause = 0;
			}
			if (pipe.m[0].active && pipe.m[0].IR == previous && result.ID != STALLED) {
				passed = TRUE;
				history[i].status[cc].stage = MULEX;
				history[i].status[cc].substage = 0;
				history[i].status[cc].cause = 0;
			}
			if (pipe.a[0].active && pipe.a[0].IR == previous && result.ID != STALLED) {
				passed = TRUE;
				history[i].status[cc].stage = ADDEX;
				history[i].status[cc].substage = 0;
				history[i].status[cc].cause = 0;
			}
			if (pipe.div.active && pipe.div.IR == previous && result.ID != STALLED) {
				passed = TRUE;
				history[i].status[cc].stage = DIVEX;
				history[i].status[cc].cause = 0;
			}
			
			if (!passed) {
				history[i].status[cc].stage = IDECODE;
				history[i].status[cc].cause = (BYTE) result.ID;
			}
			break;
		case INTEX:
			if (pipe.ex_mem.active && pipe.ex_mem.IR == previous) {
				history[i].status[cc].stage = MEMORY;
				history[i].status[cc].cause = 0;
			} else {
				history[i].status[cc].stage = INTEX;
				history[i].status[cc].cause = (BYTE) result.EX;
			}
			break;

		case MULEX:
			if (substage == pipe.MUL_LATENCY - 1) {
				if (pipe.ex_mem.active && pipe.ex_mem.IR == previous) {
					history[i].status[cc].stage = MEMORY;
					history[i].status[cc].cause = 0;
				} else {
					history[i].status[cc].stage = MULEX;
					history[i].status[cc].substage = (BYTE) substage;
					history[i].status[cc].cause = (BYTE) result.MULTIPLIER[config->MUL_LATENCY - 1];
				}
			} else {
				if (pipe.m[substage+1].active && pipe.m[substage+1].IR == previous) {
					history[i].status[cc].stage = MULEX;
					history[i].status[cc].substage = (BYTE) (substage + 1);
					history[i].status[cc].cause = 0;
				} else {
					history[i].status[cc].stage = MULEX;
					history[i].status[cc].substage = (BYTE) substage;
					history[i].status[cc].cause = (BYTE) result.MULTIPLIER[substage];
				}
			}
			break;

		case ADDEX:
			if (substage == pipe.ADD_LATENCY - 1) {
				if (pipe.ex_mem.active && pipe.ex_mem.IR == previous) {
					history[i].status[cc].stage = MEMORY;
					history[i].status[cc].cause = 0;
				} else {
					history[i].status[cc].stage = ADDEX;
					history[i].status[cc].substage = (BYTE) substage;
					history[i].status[cc].cause = (BYTE) result.ADDER[config->ADD_LATENCY-1];
				}
			} else {
				if (pipe.a[substage+1].active && pipe.a[substage+1].IR == previous) {
					history[i].status[cc].stage = ADDEX;
					history[i].status[cc].substage = (BYTE) (substage + 1);
					history[i].status[cc].cause = 0;
				} else {
					history[i].status[cc].stage = ADDEX;
					history[i].status[cc].substage = (BYTE) substage;
					history[i].status[cc].cause = (BYTE) result.ADDER[substage];
				}
			}
			break;
		case DIVEX:
			if (pipe.ex_mem.active && pipe.ex_mem.IR == previous) {
				history[i].status[cc].stage = MEMORY;
				history[i].status[cc].cause = 0;
			} else {
				history[i].status[cc].stage = DIVEX;
				history[i].status[cc].cause = (BYTE) result.DIVIDER;
			}
			break;

		case MEMORY:
			if (pipe.mem_wb.active && pipe.mem_wb.IR == previous) {
				history[i].status[cc].stage = WRITEB;
				history[i].status[cc].cause = 0;
			} else {
				history[i].status[cc].stage = MEMORY;
				history[i].status[cc].cause = (BYTE) result.MEM;
			}
			break;

		case WRITEB:
			history[i].status[cc].stage = 0;
			history[i].status[cc].cause = 0;
			break;

		default:
			history[i].status[cc].stage = 0;
			history[i].status[cc].cause = 0;
		}
	}

// make a new entry
//	if (cpu->PC!=history[entries-1].IR)
	if ((result.ID == OK || result.ID == EMPTY || cpu.getPC() != history[entries-1].IR) && pipe.active) {
		history[entries].IR = cpu.getPC();
		history[entries].status[0].stage = IFETCH;
		history[entries].status[0].cause = 0;
		history[entries].start_cycle = cycles;
		entries++;
	}
	if (entries == 50) {
		entries--;
		for (i = 0; i < entries; i++) {
			history[i] = history[i + 1];
		}
	}

}

int Simulator::one_cycle(BOOL show) {
	int status = 0;

	if (cpu.getStatus() == HALTED) 
		return HALTED;

	status = pipe.clock_tick(&result);
	++cycles;
	process_result(show);
	update_history();
	if (update_io()) 
		return WAITING_FOR_INPUT;

	if (status == HALTED) {
		cpu.setStatus(HALTED);
		return HALTED;
	}

	return status;
}

void Simulator::OnExecuteSingle() {
	//CMainFrame* pFrame=(CMainFrame*) AfxGetApp()->m_pMainWnd;
	//CStatusBar* pStatus=&pFrame->m_wndStatusBar;
	int status = one_cycle(TRUE);
	if (status == WAITING_FOR_INPUT) {
	//	pStatus->SetPaneText(0,"Esperando Entrada");
	}
	//UpdateAllViews(NULL,1L);	// send hint that								
					// code window should be scrolled
}

void Simulator::OnExecuteMulticycle() {
	//CMainFrame* pFrame=(CMainFrame*) AfxGetApp()->m_pMainWnd;
	//CStatusBar* pStatus=&pFrame->m_wndStatusBar;
	int i,status;
	simulation_running=TRUE;
	for (i = 0; i < multi - 1; i++) {
		status = one_cycle(FALSE);
		if (status)
			 break;
	}
	if (status == 0)
		 status = one_cycle(TRUE); // show status after last one.

	if (status == WAITING_FOR_INPUT) {
		//pStatus->SetPaneText(0,"Esperando Entrada");
	}

	simulation_running = FALSE;
	//UpdateAllViews(NULL,1L);	// send hint that	
					// code window should be scrolled
}

void Simulator::OnExecuteStop() 
{
	simulation_running = FALSE;
}

void Simulator::OnExecuteRunto() 
{
	//MSG message;
	char buf[80];
	int status, lapsed = 0;
	simulation_running = TRUE;
	//CMainFrame* pFrame=(CMainFrame*) AfxGetApp()->m_pMainWnd;
	//CStatusBar* pStatus=&pFrame->m_wndStatusBar;
	//pStatus->SetPaneText(0,"Ejecutando Simulación");
	do {
/*
		if (::PeekMessage(&message,NULL,0,0,PM_REMOVE))
		{
			::TranslateMessage(&message);
			::DispatchMessage(&message);
		}
*/
		lapsed++;
		status = one_cycle(FALSE);
		if (status) 
			break;
	} while (stalls || ((!cpu.code->hasBreakpoint(cpu.getPC()) && cpu.getStatus() != HALTED && simulation_running)));
	simulation_running = FALSE;
	if (status == WAITING_FOR_INPUT) {
		sprintf(buf, "Simulacion Detenida luego de %d ciclos - Esperando Entrada", lapsed);
		restart = TRUE;
	} else {
		sprintf(buf, "Simulacion Detenida luego de %d ciclos", lapsed);
		restart = FALSE;
	}

	//pStatus->SetPaneText(0,buf);
	//UpdateAllViews(NULL,1L);	// send hint that		
								// code window should be scrolled
}

int Simulator::openfile(const std::string &fname) {
	unsigned int i;
	OnFileReset();
        cpu.data->reset(); // reset data memory
	for (i = 0; i < 16; i++) 
          cpu.mm[i] = 0;

	cpu.clearScreen();
	cpu.clearTerminal(); 
        cpu.drawit = FALSE; 
        cpu.keyboard = 0;
/*	
	if (res > 0) {
		remove("winmips64.ini");
		remove("winmips64.las"); 
		//AfxGetMainWnd()->SetWindowText("WinMIPS64 - Simulador de Procesador MIPS64");
		lastfile = "";
	}
*/
	return 0;
}


void Simulator::OnFileMulti() {
/*
	CMultiDialog dlg;

	dlg.m_cycles=multi;
	dlg.DoModal();
    multi=dlg.m_cycles;	
*/
}

void Simulator::OnFileMemory() 
{
	cpu.initialize(config);
	pipe.initialize(config);

	clear();
	//UpdateAllViews(NULL);

}

void Simulator::toggleDelaySlot() {
	if (config->delay_slot) 
		config->delay_slot = FALSE;
	else
		config->delay_slot = TRUE;

	if (config->delay_slot)
		config->branch_target_buffer = FALSE;
	OnFileReset();
}

void Simulator::toggleForwarding() {
	if (config->forwarding) 
		config->forwarding = FALSE;
	else
		config->forwarding = TRUE;
	OnFileReset();
}

void Simulator::toggleBtb() {
	if (config->branch_target_buffer) 
		config->branch_target_buffer=FALSE;
	else	
		config->branch_target_buffer=TRUE;
	if (config->branch_target_buffer)
		config->delay_slot = FALSE;
	OnFileReset();
}

/** Metodos para verificar el funcionamiento del Simulador */ 

void Simulator::dump_reg() {
  printf("----------------------------------\n");
  for (int i = 0; i < 32; i++) {
   printf("R%02i = %016llx%s",i,cpu.rreg[i].val, ((i+1)%4)?"\t":"\n");
  }
  printf("----------------------------------\n");
  DOUBLE64 db;

  for (int i = 0; i < 32; i++) {
   db.s = cpu.rreg[i+32].val;
   printf("F%02i = %016.8f%s", i, db.d, ((i+1)%4)?"\t":"\n");
  }
}

void Simulator::dump_Terminal() {
	if (cpu.emptyTerminal()) 
		return;
	std::cout << "Terminal: ";
	std::cout << cpu.getTerminal() << std::endl;
	cpu.clearTerminal();
}

void Simulator::show_stats() {

	std::cout << "----- Estadisticas -----" << std::endl;
	std::cout << "Ciclo(s)               : " << cycles << std::endl;
	std::cout << "Instruccion(es)        : " << instructions << std::endl;
	if (instructions)
		std::cout << "CPI                    : " << (double) cycles/instructions << std::endl;
	std::cout << "Total Loads            : " << loads << std::endl;
	std::cout << "Total Stores           : " << stores << std::endl;
	std::cout << "Atasco(s) RAW          : " << raw_stalls << std::endl;
	std::cout << "Atasco(s) WAW          : " << waw_stalls << std::endl;
	std::cout << "Atasco(s) WAR          : " << war_stalls << std::endl;
	std::cout << "Atasco(s) Estructural  : " << structural_stalls << std::endl;
	std::cout << "Atasco(s) Branch Taken : " << branch_taken_stalls << std::endl;
	std::cout << "Atasco(s) Branch Mispr.: " << branch_misprediction_stalls << std::endl;
	//std::cout << "Tamanio del Codigo     : " << codeptr << std::endl;

}

void Simulator::show_screen() {
	std::cout << "----- Pantalla -----" << std::endl;
	const WORD32 *pantalla = cpu.getScreen();	

	for (int y = GSXY - 1; y > 0; --y) {
	  for (int x = 0; x < GSXY; ++x) {
		char car = '-';
		int pixel = pantalla[x+y*GSXY];
		switch(pixel) {
		case BLACK: 
			car = 'X';
			break;
		case WHITE:
			car = ' ';
			break;
		}
		std::cout << car;
          }
	  std::cout << std::endl;
        }
}

