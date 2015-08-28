#ifndef _defs_h
#define _defs_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>  
#include <math.h>
#include <time.h>
#include <iostream>
#include <xprb_cpp.h>
#include <tempconfig.h>


using namespace std;
using namespace ::dashoptimization;

extern int Budget;
extern uint8_t selection_scheme;

#ifdef ROULETTE_WHEEL_SELECTION
extern double total_fitness;
#endif

#endif
