#ifndef _defs_h
#define _defs_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>  
#include <math.h>
#include <time.h>
#include <iostream>
#include <xprb_cpp.h>
#define OD 144               /* Number of O-D pairs */
#define M 80
#define N 12                 /* Number of nodes in the TAP */
#define R 1                  /* Number of origins */
#define S 1                  /* Number of destinations */ 
#define LC 18                /* Number of lines in the network data file */
#define EL 17                /* Number of existing links for the network */ 
#define NL 6                /* Number of new links for the network */ 
#define bigM 15000
#define step_size 0.2
#define CHOICE 1
#define B  20      /* Project budget */
#define GA_POPULATION_SIZE 5
#define GA_ITERATIONS 9
#define FILENAME_NET "IDP_Datasets/example_net.txt"    /* File with abosule path which has the information of the network for which you want
                                                      to solve the DNDP */
#define FILENAME_TRIPS "IDP_Datasets/example_trips.txt"   /* File with abosule path which has the information of the network for which you want
                                                      to solve the DNDP */
#define FILENAME_NEW_LINKS "IDP_Datasets/example_new_links.txt"     /* File with new links that are possible choices to be added to a traffic
                                                                       network under a given budget */

using namespace std;
using namespace ::dashoptimization;

#endif
