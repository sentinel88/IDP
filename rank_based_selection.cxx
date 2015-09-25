#include <defs.h>
#include <data.h>
#include <funcs.h>

int assign_selection_rb_prob(candidate *ga_cand, int size) {
   int i;
   int fitness = size;
   int total_fitness = (size * (size+1))/ 2;
   for (i=0; i<size; i++) {
      ga_cand[i].selection_prob = (float)(fitness)/(float)total_fitness;
      fitness--;
   }
   return 0;
}

int select_candidate_rb(candidate *ga_cand, int size) {
   int i;
   int count = 3;
   float sum = 0.0;
   double rand_elem = (double)rand()/(double)RAND_MAX;
   for (i=0; i<size; i++) {
      sum += ga_cand[i].selection_prob;
      if (sum >= rand_elem) 
         return i;
   }
   return -1;         
}   

int rank_based_selection(candidate *ga_cand, candidate *gen_children, network_data *netinfo, int size) {
   unsigned int i = 0;
   int index = -1;
   candidate *pool = (candidate *)malloc((size) * sizeof(candidate));
   memset(pool, 0, size * sizeof(candidate));
   //Elitism: Best candidate from current population goes to the next generation automatically as we do not touch the 0th index in population.
   // Fill the mating pool with chromosomes from the parent population to be used for crossover operation
   printf("\nSelected candidates for crossover operations are as below\n");
   while (i < size ) {
      index = select_candidate_rb(ga_cand, size);
      memcpy(&pool[i], &ga_cand[index], sizeof(candidate));
      print_candidate(&pool[i]);
      i++;
   }
   genetic_crossover(ga_cand, pool, gen_children, netinfo, size);
   genetic_mutation(ga_cand, gen_children, netinfo, size);
   free(pool);
   memcpy(&ga_cand[1], gen_children, (size-1) * sizeof(candidate));
   return 0;
}
