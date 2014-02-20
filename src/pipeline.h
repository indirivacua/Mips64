#ifndef __PIPELINE_H
#define __PIPELINE_H

#include "Processor.h"

void init_pipeline(pipeline *,int,int,int);
int clock_tick(pipeline *,Processor *,BOOL,BOOL,BOOL,RESULT *);

#endif
