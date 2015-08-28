#ifndef _data_h
#define _data_h

#include <defs.h>

/***Data structure definitions***/

typedef struct {
 int orig;
 int term;
} link_set;

typedef struct {
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
}network_data;

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
       XPRBvar Ya[NL];                /* Binary decision variables representing new links to be taken up/not for leader */
       XPRBvar x[EL+NL][M+1];           /* Decision variables for linear approximation = Number of arcs * Number of approximation points*/
       XPRBvar Xa[EL+NL];                 /* Decision variables representing travelers on link a */
       XPRBvar Fa_rs[OD+1][EL+NL];          /* Decision variables representing travelers of O-D pair on link a */
       XPRBprob p;                          /* Initialize a new problem in BCL */
       
       int init_model_data(); /* Allocate and initialize(may not be needed as the BCL library may itself be doing it already while creating
                                 these XPRB variables) memory for all these XPRB variables */
       int cleanup_model_data(); /* Cleanup the above allocated memory */
};

typedef struct {
 //unsigned int binary_enc : NL;
 unsigned char binary_enc[NL];
 float fitness_value;
//#ifdef ROULETTE_WHEEL_SELECTION
#ifdef RANK_BASED_SELECTION
 float selection_prob;
#endif
} candidate;

typedef struct {
 int population_size;
 int iterations;
 candidate *population;
} genetic_algo;

#endif
