#ifndef _data_h
#define _data_h

#include <defs.h>

/***Data structure definitions***/

typedef struct {
    int orig;
    int term;
} link_set;

typedef struct {
    float **Ca;
    float **Ba;
    float **Ta;                    /* Pointer to memory area holding the information of Free flow travel time on link a */
    float **Demand;                /* Points to memory location holding data about OD trips */
    float **ba;
    int **EdgeMatrix;
    float budget;                  /* Budget available to make this infrastructure decision */
    link_set *existing_links;
    link_set *new_links;
}network_data;

class model_data {
 public:
       XPRBvar Ya[NL];                /* Binary decision variables representing new links to be taken up/not for leader */
       XPRBvar x[EL+NL][M+1];           /* Decision variables for linear approximation = Number of arcs * Number of approximation points*/
       XPRBvar Xa[EL+NL];                 /* Decision variables representing travelers on link a */
       XPRBvar Fa_rs[OD+1][EL+NL];          /* Decision variables representing travelers of O-D pair on link a */
       XPRBprob p;                          /* Initialize a new problem in BCL */
};

typedef struct {
    unsigned char binary_enc[NL];
    float fitness_value;
    float selection_prob;
} candidate;

typedef struct {
    int population_size;
    int iterations;
    candidate *population;
} genetic_algo;

#endif
