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

// WinEVEDoc.cpp : implementation of the Simulator class
//

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>

#include "simulator.h"
#include "utils.h"
#include "pipeline.h"

/////////////////////////////////////////////////////////////////////////////
// Simulator

Simulator::Simulator() {
	unsigned int i, codebits, databits;
	char txt[12];
	int val;
	std::string fname;
	//CStdioFile file;
	codebits = 10;
	databits = 10;
	CODESIZE = 0x400; // defaults
	DATASIZE = 0x400;
	ADD_LATENCY = 4;
	MUL_LATENCY = 7;
	DIV_LATENCY = 24;
	forwarding = TRUE;
	delay_slot = FALSE;
	entries = 1;
	offset = 0;
	history[0].IR = 0;
	history[0].start_cycle = 0;
	history[0].status[0].stage = IFETCH;

/*
	GetCurrentDirectory(MAX_PATH,AppDir); // where I am running from?

 	fname=(std::string)AppDir+'\\'+"winmips64.pth";
	if (file.Open(fname,CFile::modeRead))
	{
		file.ReadString(AppDir,MAX_PATH);
		file.Close();
	}
	strcpy(LasDir,AppDir); 	

 	fname=(std::string)AppDir+'\\'+"winmips64.ini";

	txt[0]=txt[11]=0;
	if (file.Open(fname,CFile::modeRead))
	{
		file.ReadString(txt,10);
		val=atoi(txt);
		if (val<=MAX_CODEBITS && val>=MIN_CODEBITS) codebits=val;
		file.ReadString(txt,10);
		val=atoi(txt);
		if (val<=MAX_DATABITS && val>=MIN_DATABITS) databits=val;
		file.ReadString(txt,10);
		val=atoi(txt);
		if (val<=MAX_ADD_LATENCY && val>=MIN_ADD_LATENCY)	ADD_LATENCY=val;
		file.ReadString(txt,10);
		val=atoi(txt);
		if (val<=MAX_MUL_LATENCY && val>=MIN_MUL_LATENCY) MUL_LATENCY=val;
		file.ReadString(txt,10);
		val=atoi(txt);
		if (val<=MAX_DIV_LATENCY && val>=MIN_DIV_LATENCY) DIV_LATENCY=val;
		file.ReadString(txt,10);
		if (atoi(txt)==1)	delay_slot=TRUE;
		else				delay_slot=FALSE;
		file.ReadString(txt,10);
		if (atoi(txt)==1)	forwarding=TRUE;
		else				forwarding=FALSE;
		file.ReadString(txt,10);
		if (atoi(txt)==1 && !delay_slot)	branch_target_buffer=TRUE;
		else				                branch_target_buffer=FALSE;
		

		file.Close();
		CODESIZE=(1<<codebits);
		DATASIZE=(1<<databits);
	}

	lastfile="";
	fname=(std::string)AppDir+'\\'+"winmips64.las";

	if (file.Open(fname,CFile::modeRead))
	{
		file.ReadString(lastfile);
		file.Close();
	}
*/
	cpu.initialize(CODESIZE, DATASIZE);

	init_pipeline(&pipe, ADD_LATENCY, MUL_LATENCY, DIV_LATENCY);
	codelines = new std::string[CODESIZE/4];
	assembly = new std::string[CODESIZE/4];
	mnemonic = new std::string[CODESIZE/4];
	datalines = new std::string[DATASIZE/8];

	simulation_running = FALSE;
	restart = FALSE;

	clear();
	OnReload();

}

Simulator::~Simulator() {
/*
	CStdioFile file;
	std::string fname;
	char txt[10];
	fname=(std::string)AppDir+'\\'+"winmips64.ini";

	if (file.Open(fname,CFile::modeCreate | CFile::modeWrite))
	{
		sprintf(txt,"%d\n",bits(CODESIZE));
		file.WriteString(txt);
		sprintf(txt,"%d\n",bits(DATASIZE));
		file.WriteString(txt);
		sprintf(txt,"%d\n",ADD_LATENCY);
		file.WriteString(txt);
		sprintf(txt,"%d\n",MUL_LATENCY);
		file.WriteString(txt);
		sprintf(txt,"%d\n",DIV_LATENCY);
		file.WriteString(txt);
		if (delay_slot) sprintf(txt,"1\n");
		else			sprintf(txt,"0\n");
		file.WriteString(txt);
		if (forwarding) sprintf(txt,"1\n");
		else			sprintf(txt,"0\n");
		file.WriteString(txt);
		if (branch_target_buffer) sprintf(txt,"1\n");
		else			          sprintf(txt,"0\n");
		file.WriteString(txt);

	
		file.Close();
	}
*/
	delete [] codelines;
	delete [] assembly;
	delete [] mnemonic;
	delete [] datalines;
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
	unsigned int i;

	init_pipeline(&pipe,ADD_LATENCY,MUL_LATENCY,DIV_LATENCY);
	cpu.reset();

	//UpdateAllViews(NULL);
}

void Simulator::OnFullReset() {
	 // Reset Data Memory as well
	unsigned i;
	for (i=0;i<DATASIZE/8;i++) datalines[i]="";

	init_pipeline(&pipe,ADD_LATENCY,MUL_LATENCY,DIV_LATENCY);
	cpu.reset(TRUE);
	clear();
	//UpdateAllViews(NULL);
}

void Simulator::OnFileOpen() {	
/*
	CStdioFile last;
	std::string fname,path;

	fname=(std::string)LasDir+'\\'+"winmips64.las";

	if (last.Open(fname,CFile::modeRead))
	{
		last.ReadString(path);
		dlg.m_ofn.lpstrInitialDir=path.Left(path.ReverseFind('\\'));
		last.Close();
	}

	if (dlg.DoModal()!=IDOK) return;
	path=dlg.GetPathName();

	int res=openfile(path);

	if (res==0)
	{

// AfxMessageBox(fname,MB_OK);
		if (last.Open(fname,CFile::modeCreate | CFile::modeWrite))
		{
// create .las file in directory from which application was run
			lastfile=path;
			last.WriteString(lastfile);
			last.Close();
		}
		else
		{
// if unable to write it there -  write it here instead
			int pathstart=path.ReverseFind('\\');
			if (pathstart>=0 && pathstart<MAX_PATH)
				strcpy(LasDir,path.Left(pathstart));
			else
				LasDir[0]=0;
			fname=(std::string)LasDir+'\\'+"winmips64.las";
			if (last.Open(fname,CFile::modeCreate | CFile::modeWrite))
			{
// create .las file in user directory
				lastfile=path;
				last.WriteString(lastfile);
				last.Close();
			}
		}
	}

	UpdateAllViews(NULL,1L);
*/
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

void Simulator::process_result(RESULT *result,BOOL show)
{
	char txt[300];
	//CMainFrame* pFrame=(CMainFrame*) AfxGetApp()->m_pMainWnd;
	//CStatusBar* pStatus=&pFrame->m_wndStatusBar;
	BOOL something=FALSE;


	if (result->WB==OK || result->WB==HALTED) instructions++;
	txt[0]=0;
	if (!delay_slot && result->ID==BRANCH_TAKEN_STALL) {
		something=TRUE;
		branch_taken_stalls++;
		strcat(txt,"  Atasco Branch Taken");
	}
	if (result->ID==BRANCH_MISPREDICTED_STALL) {
		something=TRUE;
		branch_misprediction_stalls++;
		strcat(txt,"  Atasco Branch Misprediction");
	}

	if (result->MEM==LOADS || result->MEM==DATA_ERR) 
		loads++;
	if (result->MEM==STORES)
		stores++;

	check_stalls(result->ID, "ID", result->idrr, txt);
	check_stalls(result->EX, "EX", result->exrr, txt);
	check_stalls(result->ADDER[0], "ADD", result->addrr, txt);
	check_stalls(result->MULTIPLIER[0], "MUL", result->mulrr, txt);
	check_stalls(result->DIVIDER, "DIV", result->divrr, txt);
	check_stalls(result->MEM, "MEM", result->memrr, txt);

	if (result->MEM!=RAW) {
		if (result->EX == STALLED) {
			structural_stalls++;
			strcat(txt,"  Atasco Estructural en EX");
		}
		if (result->DIVIDER == STALLED) {
			structural_stalls++;
			strcat(txt,"  Atasco Estructural en FP-DIV");
		}
		if (result->MULTIPLIER[MUL_LATENCY-1] == STALLED) {
			structural_stalls++;
			strcat(txt,"  Atasco Estructural en FP-MUL");
		}
		if (result->ADDER[ADD_LATENCY-1] == STALLED) {
			structural_stalls++;
			strcat(txt,"  Atasco Estructural en FP-ADD");
		}
	}
	if (result->IF == NO_SUCH_CODE_MEMORY) {
		strcat(txt,"  No existe esa dirección de código!");
		cpu.setStatus(HALTED);
	}
	if (result->EX == INTEGER_OVERFLOW) {
		strcat(txt,"  Desbordamiento de número entero!");
	}
	if (result->DIVIDER == DIVIDE_BY_ZERO) {
		strcat(txt,"  División por Cero en DIV!");
	}

	if (result->MEM == DATA_ERR) {
		strcat(txt,"  Memoria no inicializada en MEM!");
	}
	if (result->MEM==NO_SUCH_DATA_MEMORY) {
		strcat(txt,"  No existe esa dirección de datos!");
	}
	if (result->MEM==DATA_MISALIGNED) {
		strcat(txt, " Error Fatal - LOAD/StTORE de memoria mal alineado!");
	}
	if (show)
	{
/*
		if (txt[0]==0) pStatus->SetPaneText(0,"Listo");
		else pStatus->SetPaneText(0,txt);
*/
	}
}

int Simulator::update_io(Processor *cpu) {
	WORD32 func = *(WORD32 *)&cpu->mm[0];
	int i, x, y, status = 0;

	BYTE *az;
	if (!func) 
		return status;

	char txt[30];
	DOUBLE64 fp;
	fp.u = *(WORD64 *)&cpu->mm[8]; 

	switch (func) {
	case (WORD32)1:
		sprintf(txt,"%I64u\n",fp.u);
		cpu->writeTerminal(txt);
		//UpdateAllViews(NULL,2);
		break;
	case (WORD32)2:
		sprintf(txt,"%I64d\n",fp.s);
		cpu->writeTerminal(txt);
		//UpdateAllViews(NULL,2);
		break;
	case (WORD32)3:
		sprintf(txt,"%lf\n",fp.d);
		cpu->writeTerminal(txt);
		//UpdateAllViews(NULL,2);
		break;
	case (WORD32)4:
		// need to test here if fp.u is a legal address!

		// ARREGLAR!! Feo, feo!!!
		az = &cpu->data[fp.u];

		if (fp.u<cpu->getDataMemorySize()) 
		cpu->writeTerminal(std::string((const char *)az));

		//UpdateAllViews(NULL,2);
		break;

	case (WORD32)5:

		y=(WORD32)((fp.u>>32)&255);
		x=(WORD32)((fp.u>>40)&255);
		cpu->drawit=TRUE;
//			char txt[80];
//			sprintf(txt,"%d %d",x,y);
//			AfxMessageBox(txt);

		if (x<GSXY && y<GSXY) {
			cpu->setScreenPixel(x,y, (WORD32)fp.u);
		}
		//UpdateAllViews(NULL,2);
		break;
	case (WORD32)6:
		cpu->clearTerminal();
		//UpdateAllViews(NULL,2);
		break;
	case (WORD32)7:
		cpu->clearScreen();
		cpu->drawit=FALSE;
		//UpdateAllViews(NULL,2);
		break;
	case (WORD32)8:
/*
		cpu->keyboard=1;
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
	  	*(WORD64 *)&(cpu->mm[8]) = number.u; 
		}
		break;
	case (WORD32)9:
/*
		cpu->keyboard=2;
		status=1;
*/
{
		
	  	(cpu->mm[8]) = getchar(); 
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

	*(WORD32 *)&cpu->mm[0]=0;
	return status;
}

void Simulator::update_history(pipeline *pipe, Processor *cpu, RESULT *result) {
	int substage,stage;
	unsigned int i,cc;
	WORD32 previous;
	BOOL passed;

	if (result->MEM!=RAW) {
		if (result->EX==STALLED) 
			result->EX=STRUCTURAL;
		if (result->DIVIDER==STALLED) 
			result->DIVIDER=STRUCTURAL;
		if (result->MULTIPLIER[MUL_LATENCY-1]==STALLED) 
			result->MULTIPLIER[MUL_LATENCY-1]=STRUCTURAL;
		if (result->ADDER[ADD_LATENCY-1]==STALLED) 
			result->ADDER[ADD_LATENCY-1]=STRUCTURAL;
	}	

	for (i=0;i<entries;i++)	{
		previous = history[i].IR;
		cc = cycles-history[i].start_cycle;
		stage = history[i].status[cc-1].stage; // previous stage
		substage = history[i].status[cc-1].substage;
	
		switch (stage) {

		case IFETCH:
			if (pipe->if_id.active)	{
				if (pipe->if_id.IR==previous) {

					history[i].status[cc].stage=IDECODE;
					history[i].status[cc].cause=0;
				} else {
					history[i].status[cc].stage=IFETCH;
					history[i].status[cc].cause= (BYTE) result->IF;
				}
			} else {
				history[i].status[cc].stage=0;
				history[i].status[cc].cause=0;
			}
			break;
		case IDECODE:
			passed=FALSE;
			
			if (pipe->integer.active && pipe->integer.IR==previous && result->ID!=STALLED) {
				passed=TRUE;
				history[i].status[cc].stage=INTEX;
				history[i].status[cc].cause=0;
			}
			if (pipe->m[0].active && pipe->m[0].IR==previous && result->ID!=STALLED) {
				passed=TRUE;
				history[i].status[cc].stage=MULEX;
				history[i].status[cc].substage=0;
				history[i].status[cc].cause=0;
			}
			if (pipe->a[0].active && pipe->a[0].IR==previous && result->ID!=STALLED) {
				passed=TRUE;
				history[i].status[cc].stage=ADDEX;
				history[i].status[cc].substage=0;
				history[i].status[cc].cause=0;
			}
			if (pipe->div.active && pipe->div.IR==previous && result->ID!=STALLED) {
				passed=TRUE;
				history[i].status[cc].stage=DIVEX;
				history[i].status[cc].cause=0;
			}
			
			if (!passed) {
				history[i].status[cc].stage=IDECODE;
				history[i].status[cc].cause=(BYTE) result->ID;
			}
			break;
		case INTEX:
			if (pipe->ex_mem.active && pipe->ex_mem.IR==previous) {
				history[i].status[cc].stage=MEMORY;
				history[i].status[cc].cause=0;
			} else {
				history[i].status[cc].stage=INTEX;
				history[i].status[cc].cause=(BYTE) result->EX;
			}
			break;

		case MULEX:
			if (substage==pipe->MUL_LATENCY-1) {
				if (pipe->ex_mem.active && pipe->ex_mem.IR==previous) {
					history[i].status[cc].stage=MEMORY;
					history[i].status[cc].cause=0;
				} else {
					history[i].status[cc].stage=MULEX;
					history[i].status[cc].substage= (BYTE) substage;
					history[i].status[cc].cause= (BYTE) result->MULTIPLIER[MUL_LATENCY-1];
				}
			} else {
				if (pipe->m[substage+1].active && pipe->m[substage+1].IR==previous) {
					history[i].status[cc].stage=MULEX;
					history[i].status[cc].substage= (BYTE) (substage + 1);
					history[i].status[cc].cause=0;
				} else {
					history[i].status[cc].stage=MULEX;
					history[i].status[cc].substage=(BYTE) substage;
					history[i].status[cc].cause=(BYTE) result->MULTIPLIER[substage];
				}
			}
			break;

		case ADDEX:
			if (substage == pipe->ADD_LATENCY - 1) {
				if (pipe->ex_mem.active && pipe->ex_mem.IR==previous) {
					history[i].status[cc].stage=MEMORY;
					history[i].status[cc].cause=0;
				} else {
					history[i].status[cc].stage=ADDEX;
					history[i].status[cc].substage=(BYTE) substage;
					history[i].status[cc].cause=(BYTE) result->ADDER[ADD_LATENCY-1];
				}
			} else {
				if (pipe->a[substage+1].active && pipe->a[substage+1].IR==previous) {
					history[i].status[cc].stage=ADDEX;
					history[i].status[cc].substage=(BYTE) (substage + 1);
					history[i].status[cc].cause=0;
				} else {
					history[i].status[cc].stage=ADDEX;
					history[i].status[cc].substage=(BYTE) substage;
					history[i].status[cc].cause= (BYTE) result->ADDER[substage];
				}
			}
			break;
		case DIVEX:
			if (pipe->ex_mem.active && pipe->ex_mem.IR==previous) {
				history[i].status[cc].stage=MEMORY;
				history[i].status[cc].cause=0;
			} else {
				history[i].status[cc].stage=DIVEX;
				history[i].status[cc].cause=(BYTE) result->DIVIDER;
			}
			break;

		case MEMORY:
			if (pipe->mem_wb.active && pipe->mem_wb.IR==previous) {
				history[i].status[cc].stage=WRITEB;
				history[i].status[cc].cause=0;
			} else {
				history[i].status[cc].stage=MEMORY;
				history[i].status[cc].cause=(BYTE) result->MEM;
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
	if ((result->ID == OK || result->ID == EMPTY || cpu->getPC() != history[entries-1].IR) && pipe->active) {
		history[entries].IR = cpu->getPC();
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

int Simulator::one_cycle(pipeline *pipe, Processor *cpu, BOOL show) {
	int status=0;
	RESULT result;

	if (cpu->getStatus() == HALTED) 
		return HALTED;

	status = clock_tick(pipe, cpu, forwarding, delay_slot, branch_target_buffer, &result);

	cycles++;
	process_result(&result,show);
	update_history(pipe, cpu, &result);
	if (update_io(cpu)) 
		return WAITING_FOR_INPUT;

	if (status == HALTED) {
		cpu->setStatus(HALTED);
		return HALTED;
	}

	return status;
}

void Simulator::OnExecuteSingle() {
	//CMainFrame* pFrame=(CMainFrame*) AfxGetApp()->m_pMainWnd;
	//CStatusBar* pStatus=&pFrame->m_wndStatusBar;
	int status = one_cycle(&pipe, &cpu, TRUE);
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
		status = one_cycle(&pipe, &cpu, FALSE);
		if (status)
			 break;
	}
	if (status == 0)
		 status = one_cycle(&pipe, &cpu, TRUE); // show status after last one.

	if (status == WAITING_FOR_INPUT) {
		//pStatus->SetPaneText(0,"Esperando Entrada");
	}

	simulation_running = FALSE;
	//UpdateAllViews(NULL,1L);	// send hint that	
					// code window should be scrolled
}

void Simulator::OnExecuteStop() 
{
	simulation_running=FALSE;
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
		status = one_cycle(&pipe,&cpu,FALSE);
		if (status) 
			break;
	} while (stalls || ((cpu.cstat[cpu.getPC()] & 1) == 0 && cpu.getStatus() != HALTED && simulation_running));
	simulation_running = FALSE;
	if (status == WAITING_FOR_INPUT) {
		sprintf(buf, "Simulación Detenida luego de %d ciclos - Esperando Entrada", lapsed);
		restart = TRUE;
	} else {
		sprintf(buf, "Simulación Detenida luego de %d ciclos", lapsed);
		restart = FALSE;
	}

	//pStatus->SetPaneText(0,buf);
	//UpdateAllViews(NULL,1L);	// send hint that		
								// code window should be scrolled
}

int Simulator::openfile(const std::string &fname) {
	unsigned int i;
	int res;
	OnFileReset();
	for (i = 0; i < DATASIZE; i++) 
          cpu.data[i] = cpu.dstat[i]  = 0; // reset memory
	for (i = 0; i < 16; i++) 
          cpu.mm[i] = 0;
	cpu.clearScreen();
	cpu.clearTerminal(); 
        cpu.drawit = FALSE; 
        cpu.keyboard = 0;
	
	for (i = 0; i < CODESIZE/4; i++) {
		codelines[i] = "";
		assembly[i] = "";
		mnemonic[i] = "";
	}
	for (i = 0; i < DATASIZE/STEP; i++) 
          datalines[i] = "";

	if (fname == "")
          return 1;

	res = openit(fname);
	if (res > 0) {
		remove("winmips64.ini");
		remove("winmips64.las"); 
		//AfxGetMainWnd()->SetWindowText("WinMIPS64 - Simulador de Procesador MIPS64");
		lastfile = "";
	}
	if (res == 1) {
		char txt[512];
		sprintf(txt, "No se pudo abrir el archivo %s", fname.c_str());
		//AfxMessageBox(txt,MB_OK|MB_ICONEXCLAMATION);
		std::cout << txt << std::endl;
		return res;
	}
	if (res == 2) {
		//AfxMessageBox("Errores detectados en 1ra Pasada");
		std::cout << "Errores detectados en 1ra Pasada" << std::endl;
		return res;
	}
	if (res == 3) {
		//AfxMessageBox("Errores detectados en 2da Pasada");
		std::cout << "Errores detectados en 2da Pasada" << std::endl;
		return res;
	}
	//AfxGetMainWnd()->SetWindowText("WinMIPS64 - Simulador de Procesador MIPS64 - " + fname);

	return res;
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
	unsigned int i;
	unsigned int codesize, datasize;
/*
	CMemDialog dlg;

	dlg.m_code=bits(CODESIZE);
	dlg.m_data=bits(DATASIZE);
	dlg.m_fpal=ADD_LATENCY;
	dlg.m_fpml=MUL_LATENCY;
	dlg.m_fpdl=DIV_LATENCY;

	if (dlg.DoModal()!=IDOK) return;

	codesize=(1<<dlg.m_code);
	datasize=(1<<dlg.m_data);
						// full system reset
	CODESIZE=codesize;
	DATASIZE=datasize;
	ADD_LATENCY=dlg.m_fpal;
	MUL_LATENCY=dlg.m_fpml;
	DIV_LATENCY=dlg.m_fpdl;
*/

	delete [] codelines;
	delete [] datalines;
	delete [] assembly;
	delete [] mnemonic;

	cpu.initialize(CODESIZE, DATASIZE);

	init_pipeline(&pipe,ADD_LATENCY,MUL_LATENCY,DIV_LATENCY);

	forwarding = TRUE;
	delay_slot = FALSE;
	branch_target_buffer = FALSE;

	codelines = new std::string[CODESIZE/4];
	assembly = new std::string[CODESIZE/4];
	mnemonic = new std::string[CODESIZE/4];
	datalines = new std::string[DATASIZE/STEP];
	
	clear();
	//UpdateAllViews(NULL);

	OnReload();
}

// Disable everything while simulation is running

void Simulator::OnUpdateExecuteMulticycle() 
{
	//pCmdUI->Enable(!simulation_running && !cpu.keyboard);	
}

void Simulator::OnUpdateExecuteStop() 
{
	//pCmdUI->Enable(simulation_running);	
}

void Simulator::OnUpdateExecuteRunto() 
{
	//pCmdUI->Enable(!simulation_running && !cpu.keyboard);
}

void Simulator::OnUpdateExecuteSingle() 
{
	//pCmdUI->Enable(!simulation_running && !cpu.keyboard);
}

void Simulator::OnUpdateFileMulti() 
{
	//pCmdUI->Enable(!simulation_running);	
}

void Simulator::OnUpdateFileMemory() 
{
	//pCmdUI->Enable(!simulation_running);
}

void Simulator::OnUpdateFileOpen() 
{
	//pCmdUI->Enable(!simulation_running);
}

void Simulator::OnUpdateFileReset() 
{
	//pCmdUI->Enable(!simulation_running);	
}

void Simulator::OnUpdateFullReset() 
{
	//pCmdUI->Enable(!simulation_running);	
}

int Simulator::OnReload() 
{ // reload last file
	int res;
	char txt[512];
	//CMainFrame* pFrame=(CMainFrame*) AfxGetApp()->m_pMainWnd;
	//CStatusBar* pStatus=&pFrame->m_wndStatusBar;
	res = openfile(lastfile);
	if (res == 0) {
	//	AfxGetMainWnd()->SetWindowText(lastfile);
		snprintf(txt, 512, "Archivo cargado - %s",lastfile.c_str());
	//	pStatus->SetPaneText(0,txt);
	}

	return res;
}

void Simulator::OnUpdateReload() {
	//pCmdUI->Enable(!simulation_running);
	
}

void Simulator::OnFileDelaySlot() {
	if (delay_slot) 
		delay_slot = FALSE;
	else
		delay_slot = TRUE;
	OnFileReset();
}

void Simulator::OnUpdateFileDelaySlot() {
	//pCmdUI->SetCheck(delay_slot);
	//pCmdUI->Enable(!branch_target_buffer);
}

void Simulator::OnFileForwarding() {
	if (forwarding) 
		forwarding=FALSE;
	else
		forwarding=TRUE;
	OnFileReset();
}

void Simulator::OnUpdateFileForwarding() {
	//pCmdUI->SetCheck(forwarding);	
}

void Simulator::OnBtb() {
	if (branch_target_buffer) 
		branch_target_buffer=FALSE;
	else	
		branch_target_buffer=TRUE;
	OnFileReset();
}

void Simulator::OnUpdateBtb() {
	//pCmdUI->SetCheck(branch_target_buffer);
	//pCmdUI->Enable(!delay_slot);
}


/** Metodos para verificar el funcionamiento del Simulador */ 
void Simulator::dump_mem() {
	BYTE *mem = cpu.code;
	std::cout << "Code Size: " << cpu.getCodeMemorySize() << std::endl;
	std::cout << "Data Size: " << cpu.getDataMemorySize() << std::endl;

	for (int i = 0; i < cpu.getCodeMemorySize(); ++i) {
		std::cout << " " << (int) mem[i];
	}
	std::cout << std::endl;
}

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
	if (!cpu.emptyTerminal()) 
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
	std::cout << "Tamanio del Codigo     : " << codeptr << std::endl;

}

void Simulator::show_screen() {
	std::cout << "----- Pantalla -----" << std::endl;
	const WORD32 *pantalla = cpu.getScreen();	

	for (int y = 0; y < GSXY; ++y) {
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

