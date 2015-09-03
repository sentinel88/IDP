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

typedef enum { DEFAULT_SELECTION, RANK_BASED_SELECTION, TOURNAMENT_SELECTION } scheme;
extern scheme selection_scheme;

#ifdef ROULETTE_WHEEL_SELECTION
extern double total_fitness;
#endif

#define _DEBUG 1
//#define _PROTECT

#endif
