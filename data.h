#ifndef _data_h
#define _data_h

#include <defs.h>


/***Data structure definitions***/

struct link_set {
 int orig;
 int term;
};

struct network_data {
 //float Ca[N+1][N+1];            /* Capacity of link a */
 float **Ca;
 //float Ba[N+1][N+1];            /* Congestion factor for link a */
 float **Ba;
 float **Ta;                    /* Pointer to memory area holding the information of Free flow travel time on link a */
 float **Demand;                /* Points to memory location holding data about OD trips */
 //float ba[N+1][N+1];            /* Cost for building new links in the network */
 float **ba;
 //int EdgeMatrix[N+1][N+1];      /* Matrix that specifies whether the edge is an existing or a new link. -1 says its a new link and 1 says
                                   //its an existing link */
 int **EdgeMatrix;
 float budget;                  /* Budget available to make this infrastructure decision */
 //link_set existing_links[EL];     /* Set of existing links where every element is a pair containing orig and term node */
 link_set *existing_links;
 //link_set new_links[NL];         /* Set of new links to be considered for construction containing pairs just like mentioned in the prev stmt */
 link_set *new_links;
};

//struct model_data {
// XPRBvar x[N+1][N+1][M+1];           /* Decision variables for linear approximation = Number of arcs * Number of approximation points*/
// XPRBvar Xa[N+1][N+1];                 /* Decision variables representing travelers on link a */
// XPRBvar Fa_rs[OD+1][N+1][N+1];          /* Decision variables representing travelers of O-D pair on link a */
// XPRBvar Ya[N+1][N+1];                /* Binary decision variables representing new links to be taken up/not for leader */
// XPRBprob p;                          /* Initialize a new problem in BCL */
// XPRBexpr incr;              /* Increment to be added after x[1] */
// XPRBexpr summation;         /* Auxiliary summation expression */
// XPRBexpr increment[N+1][N+1];   /* Summation expressions for each arc that is used to replace the non linear term in TAP objective */
//};

class model_data {
 public:
       XPRBvar Ya[N+1][N+1];                /* Binary decision variables representing new links to be taken up/not for leader */
       XPRBvar x[N+1][N+1][M+1];           /* Decision variables for linear approximation = Number of arcs * Number of approximation points*/
       //XPRBvar ****x;
       XPRBvar Xa[N+1][N+1];                 /* Decision variables representing travelers on link a */
       //XPRBvar ***Xa;
       XPRBvar Fa_rs[OD+1][N+1][N+1];          /* Decision variables representing travelers of O-D pair on link a */
       //XPRBvar ****Fa_rs;
       //XPRBvar ***Ya;
       //XPRBexpr incr;              /* Increment to be added after x[1] */
       //XPRBexpr summation;         /* Auxiliary summation expression */
       //XPRBexpr increment[N+1][N+1];   /* Summation expressions for each arc that is used to replace the non linear term in TAP objective */
       XPRBprob p;                          /* Initialize a new problem in BCL */
       
       int init_model_data(); /* Allocate and initialize(may not be needed as the BCL library may itself be doing it already while creating
                                 these XPRB variables) memory for all these XPRB variables */
       int cleanup_model_data(); /* Cleanup the above allocated memory */
};

struct candidate {
 //unsigned int binary_enc : NL;
 unsigned char binary_enc[NL];
 float fitness_value;
#ifdef ROULETTE_WHEEL_SELECTION
 float selection_prob;
#endif
};

struct genetic_algo {
 int population_size;
 int iterations;
 candidate *population;
};
#endif

