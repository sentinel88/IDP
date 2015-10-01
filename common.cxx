#include <defs.h>
#include <data.h>
#include <funcs.h>


void print(genetic_algo *ga, network_data *netinfo, model_data *dndp, int j) {
    printf("\nInside print function\n");
    int orig, term;
    int r,s;
    printf("\nTravelers on all the links\n");
    /***Travelers on link a***/
    for (int m=0; m<EL; m++) {
      orig = netinfo->existing_links[m].orig;
      term = netinfo->existing_links[m].term;
      //printf("[%d, %d] = %lf\n", orig, term, (dndp->Xa[orig][term]).getSol());
      printf("[%d, %d] = %lf\n", orig, term, (dndp->Xa[m]).getSol());
    #ifdef _DEBUG1
      for (int n=1; n<=M; n++) {
         printf("%lf\t", dndp->x[m][n].getSol());
      }
      printf("\n\n");
    #endif
    }
    for (int m=0; m<NL; m++) {
       if (ga->population[j].binary_enc[m]) {
          orig = netinfo->new_links[m].orig;
          term = netinfo->new_links[m].term;
          //printf("[%d, %d] = %lf\n", orig, term, (dndp->Xa[orig][term]).getSol());
          printf("[%d, %d] = %lf\n", orig, term, (dndp->Xa[EL+m]).getSol());
          #ifdef _DEBUG
          for (int n=1; n<=M; n++) {
             printf("%lf\t", dndp->x[EL+m][n].getSol());
          }
          printf("\n\n");
          #endif
       }
    }
    printf("\nFlows on the links w.r.t OD pairs\n");
    for (int i=1; i<=OD; i++) {
       r = (i/ZONES);
       s = (i%ZONES);
       r = (r==0)?1 : (s==0?r : (r+1));
       s = (s==0)?ZONES : s;
       if (netinfo->Demand[r][s]) {
          for (int l=0; l<EL; l++) {
	     if ((dndp->Fa_rs[i][l]).getSol() != 0.0) {
	        printf("\nPair: [%d, %d]\n", r, s);
      	        orig = netinfo->existing_links[l].orig;
                term = netinfo->existing_links[l].term;
                printf("[%d, %d] = %lf\t", orig, term, (dndp->Fa_rs[i][l]).getSol());
	     }
          }
          printf("\n");
       }
    }
    printf("\nExiting print function\n");
}


// Get random number in a specific range using the library function rand() which generates pseudo random numbers in the interval [0, RANGE_MAX]
int get_random(int range, bool incl_zero) {
   unsigned int x = (RAND_MAX + 1u) / range;
   unsigned int y = x * range;
   unsigned int r;
   do {
      r = rand();
   } while(r >= y);
   incl_zero == true ? printf("%d\t", r / x) : printf("%d\t", r/x + 1);
   return ( incl_zero == true ? r/x: r/x + 1 );
}


int feasibility(candidate gen_cand, network_data *netinfo) {
 float budget_sat = 0.0;
 int i = 0, orig, term;
 int bin_val;
 printf("\nEntering feasibility\n");
 while (i < NL) {
    orig = netinfo->new_links[i].orig;
    term = netinfo->new_links[i].term;
    bin_val = gen_cand.binary_enc[i];
#ifdef _DEBUG
    printf("%d, orig: %d, term: %d\n ", gen_cand.binary_enc[i], orig, term);
#endif
    budget_sat += (bin_val * netinfo->ba[orig][term]);
    i++;
 }
 printf("\nBudget = %f\n", budget_sat);
 if (budget_sat > Budget) return -1;
 printf("\nExiting feasiblity\n");
 return 0;
}

int encode_ga_cand(candidate *ga_cand, int value) {
 int i = NL, k = 0;
 printf("\nEntering encode_ga_cand\n");
 while(i--) {
    ga_cand->binary_enc[k] = value & 1;
    value = value >> 1;
#ifdef _DEBUG
    printf("%d ", ga_cand->binary_enc[k]);
#endif
    k++;
 }
 printf("\nExisting encode_ga_cand\n");
 return 0;
}


int candidate_fitness(model_data *dndp, network_data *netinfo, candidate *ga_cand) {
   float summation = 0.0;
   int i = 0, orig, term;
   int ba = 1;

   printf("\nEntering candidate fitness function\n");

   for (i=0; i<EL; i++) {
      orig = netinfo->existing_links[i].orig;
      term = netinfo->existing_links[i].term;
      if (cost_function_selector == 2) {
         //summation += (netinfo->Ta[orig][term] * (1 + (netinfo->Ba[orig][term] * pow( (dndp->Xa[orig][term].getSol() / netinfo->Ca[orig][term]), 4))) );

         summation += (dndp->Xa[i].getSol()) * (netinfo->Ta[orig][term] * (1 + (0.15 * pow( (dndp->Xa[i].getSol() / netinfo->Ca[orig][term]), 4))) );   // Cost function for Sioux falls network

         //summation += (dndp->Xa[i].getSol()) * (netinfo->Ta[orig][term]  + (netinfo->Ba[orig][term] * pow( (dndp->Xa[i].getSol() / netinfo->Ca[orig][term]), 4)) );   // Cost function for berlin mitte center network

      } else {   // Cost function for the example network
         //summation += (netinfo->Ta[orig][term]) * (dndp->Xa[orig][term].getSol());
         summation += (dndp->Xa[i].getSol()) * (netinfo->Ta[orig][term] + (0.008 * pow(dndp->Xa[i].getSol(), 4)));
      }
   }

   for (i=0; i<NL; i++) {
   if (ga_cand->binary_enc[i]) {
      orig = netinfo->new_links[i].orig;
      term = netinfo->new_links[i].term;
//#ifdef _CODE   
   if (cost_function_selector == 2) {
         //summation += (netinfo->Ta[orig][term] * (1 + (0.15 * pow( (dndp->Xa[orig][term].getSol() / netinfo->Ca[orig][term]), 4))) );
        // summation += (dndp->Xa[EL+i].getSol()) * (netinfo->Ta[orig][term] * (1 + (0.15 * pow( (dndp->Xa[EL+i].getSol() / netinfo->Ca[orig][term]), 4))) );
         //summation += (dndp->Xa[EL+i].getSol()) * (netinfo->Ta[orig][term] + (netinfo->Ba[orig][term] * pow( (dndp->Xa[EL+i].getSol() / netinfo->Ca[orig][term]), 4)) );
         summation += (dndp->Xa[EL+i].getSol()) * (netinfo->Ta[orig][term] + (ba * pow( (dndp->Xa[EL+i].getSol() / netinfo->Ca[orig][term]), 4)) );
      } else {
//#endif
         //summation += (netinfo->Ta[orig][term]) * (dndp->Xa[orig][term].getSol());
         summation += (dndp->Xa[EL+i].getSol()) * (netinfo->Ta[orig][term] + (0.008 * pow(dndp->Xa[EL+i].getSol(), 4)));
      }
   } }

   ga_cand->fitness_value = summation;
   printf("\nCandidate fitness: %e\n", summation);
   printf("\nExiting candidate fitness function\n");
}

int candidates_sort(candidate *ga_cand, int size) {
   int i, j;
   candidate temp;

   memset(&temp, 0, sizeof(candidate));

   for (i=0; i<size; i++) {
      for (j=0; j<(size-i-1); j++) {
         if (ga_cand[j].fitness_value > ga_cand[j+1].fitness_value) {
            memcpy(&temp, &(ga_cand[j+1]), sizeof(candidate));
            memcpy(&(ga_cand[j+1]), &(ga_cand[j]), sizeof(candidate));
            memcpy(&(ga_cand[j]), &temp, sizeof(candidate));
         }
      }
   }
   return 0;
}

int print_generation(candidate *ga_cand, int pool_size, bool disp_fitness) {
   int i, j=0;
   printf("************************************************\n");
   if (disp_fitness) {
   for (i=0; i<pool_size; i++) {
      printf("Candidate %d:  ", i+1);
      /* Replace the below loop for printing candidate by calling the function print_candidate for the same */
      for (j=0; j<NL; j++) {
         printf("%d", ga_cand[i].binary_enc[j]);
      }
      printf("\tFitness value: %e", ga_cand[i].fitness_value);
#ifdef ROULETTE_WHEEL_SELECTION
      printf("\tSelection probability: %f", ga_cand[i].selection_prob);
#endif
      printf("\n");
   }
   } else {
   for (i=0; i<pool_size; i++) {
      printf("Candidate %d: ", i+1);
      for (j=0; j<NL; j++) {
         printf("%d", ga_cand[i].binary_enc[j]);
      }
      printf("\n");
   }
   }
   printf("************************************************\n");
   return 0;
}

int print_candidate(candidate *ga_cand) {
   int i;
   printf("************************************************\n");
   printf("Candidate: ");
   for (i=0; i<NL; i++) {
      printf("%d", ga_cand->binary_enc[i]);
   }
   printf("\n************************************************\n");
   return 0;
}         

int check_duplicate(candidate *ga_cand, candidate *population, int size) {
   int i, j;
   int match_found = 0;
   printf("\nEntering check_duplicate function\n");

   for (i=0; i<size; i++) {
      if (!memcmp(ga_cand->binary_enc, population[i].binary_enc, NL)) {
      //if (elem_compare(ga_cand, &population[i])) {
         match_found = 1;
         break;
      }
   }
   if (match_found) {
      printf("\nDuplicate found\n");
      return 1;
   }

   printf("\nExiting check_duplicate function\n");
   return 0;
}

int elem_compare(candidate *pattern, candidate *match) {
   int i;
   for (i=0; i<NL; i++) {
      if (pattern->binary_enc[i] != match->binary_enc[i])
         return 0;
   }
   return 1;
}

int count_set_bits(int value) {
   int count=0;
   while(value) {
      if (value & 1) count++;
      value = value>>1;
   }
   return count;
}

int check_if_zero(candidate gen_child) {
   int i=0, k=0;
   while (i < NL) {
      if (gen_child.binary_enc[k++] & 1) return 0;
      i++;
   }
   return 1;
}

int cache_lookup(candidate *pattern, candidate *cache, int *index, int size) {
   int i=0;
   printf("\nEntering cache lookup\n");
   while (i < size) {
      if (!memcmp(&(pattern->binary_enc), &cache[i].binary_enc, NL)) {
         printf("\nPattern:\n");
         print_candidate(pattern);
         printf("\nMatch:\n");
         print_candidate(&cache[i]);
         printf("\nCache hit. No need to re compute for this candidate again\n");
         print_candidate(pattern);
         *index = i;
         return 0;
      }
      i++;
   }
   printf("\nExiting cache lookup\n");
   return 1;
}
