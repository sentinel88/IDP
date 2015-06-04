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
//#define OD 576               /* Number of O-D pairs */
//#define M 80
//#define M 250
#define M 20
#define N 12                 /* Number of nodes in the TAP */
//#define N 24                 /* Number of nodes in the TAP */
#define R 1                  /* Number of origins */
#define S 1                  /* Number of destinations */ 
#define LC 18                /* Number of lines in the network data file */
//#define LC 84                /* Number of lines in the network data file */
#define EL 17                /* Number of existing links for the network */ 
//#define EL 76                /* Number of existing links for the network */ 
#define NL 6                /* Number of new links for the network */ 
//#define NL 10                /* Number of new links for the network */ 
#define bigM 15000
//#define step_size 0.2
#define step_size 2
//#define step_size 100
//#define step_size 125
#define mut_prob 0.45
#define cross_prob 0.85
#define CHOICE 1
//#define CHOICE 2
//#define B  10      /* Project budget */
#define GA_POPULATION_SIZE 10
#define GA_ITERATIONS 10
//#define GA_ITERATIONS 3
#define FILENAME_NET "IDP_Datasets/example_net.txt"    /* File with abosule path which has the information of the network for which you want
//#define FILENAME_NET "IDP_Datasets/SiouxFalls_net.txt"   * File with abosule path which has the information of the network for which you want
                                                      to solve the DNDP */
#define FILENAME_TRIPS "IDP_Datasets/example_trips.txt"   /* File with abosule path which has the information of the network for which you want
//#define FILENAME_TRIPS "IDP_Datasets/SiouxFalls_trips.txt"   * File with abosule path which has the information of the network for which you want
                                                      to solve the DNDP */
#define FILENAME_NEW_LINKS "IDP_Datasets/example_new_links.txt"     /* File with new links that are possible choices to be added to a traffic
//#define FILENAME_NEW_LINKS "IDP_Datasets/SiouxFalls_new_links.txt"    * File with new links that are possible choices to be added to a traffic
                                                                       network under a given budget */

using namespace std;
using namespace ::dashoptimization;

extern int Budget;

#endif
