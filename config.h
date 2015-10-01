#ifndef _config_h
#define _config_h

//#define OD 144               /* Number of O-D pairs */
#define OD 1296              /* Number of O-D pairs */
//#define OD 576               /* Number of O-D pairs */
//#define M 80
//#define M 250
#define M 20
//#define N 12                 /* Number of nodes in the TAP */
//#define N 24                 /* Number of nodes in the TAP */
#define N 398
#define R 1                  /* Number of origins */
#define S 1                  /* Number of destinations */ 
//#define LC 18                /* Number of lines in the network data file */
//#define LC 84                /* Number of lines in the network data file */
#define LC 880
//#define EL 76                /* Number of existing links for the network */ 
//#define EL 17                /* Number of existing links for the network */ 
#define EL 871                /* Number of existing links for the network */ 
//#define NL 6                /* Number of new links for the network */ 
#define NL 10                /* Number of new links for the network */ 
//#define NL 0
#define bigM 150000000
//#define step_size 0.2
//#define step_size 2
//#define step_size 100
//#define step_size 80
//#define step_size 1250
//#define step_size 313
#define step_size 75
#define mut_prob 0.25
#define cross_prob 0.85
//#define _PROTECT 1
#define CHOICE 1
#define MAX_CROSSOVER_ATTEMPTS 5
//#define ZONES 1
#define ZONES 36
//#define B  10      /* Project budget */
#define GA_POPULATION_SIZE 10
//#define GA_POPULATION_SIZE 1
#define GA_ITERATIONS 7
#define MAX_BEST_ITERATIONS 4
#define MAX_RETRY 3
#define TWO_POINT_CROSSOVER 1
//#define GA_ITERATIONS 3
//#define ROULETTE_WHEEL_SELECTION 1

#define FILENAME_NET "IDP_Datasets/Berlin_Network/TNTP_Format/mitte_center/mitte_center_net.txt"   /* File with abosule path which has the information of the n                                                                                      etwork for which you want to solve the DNDP */

#define FILENAME_TRIPS "IDP_Datasets/Berlin_Network/TNTP_Format/mitte_center/mitte_center_ODflows.txt"   /* File with abosule path which has the information of the network for which you w                                                                 ant to solve the DNDP */


#define FILENAME_NEW_LINKS "IDP_Datasets/Berlin_Network/TNTP_Format/mitte_center/mitte_center_new_links.txt"    /* File with new links that are possible choices to be added to a traffic                                                                       network under a given budget */

#endif
