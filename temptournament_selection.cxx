#include <defs.h>
#include <data.h>
#include <funcs.h>

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

// The below selection procedure for tournament selection is binary/ternary which means it selects 2/3 candidates at random and returns the fittest one.
int select_candidates_k(candidate *ga_cand, int size) {
   int index[3] = {-1, -1, -1};
   //int index[2] = {-1, -1};
   int i = 0;
   double fittest = 999999999;
   int fittest_index = 0;
   while (i < 3) {
      index[i] = get_random(size, true);
      if (i && (index[i] == index[i -1])) { // Wrong check. index[i] needs to be compared with all the previous indices
         continue;
      } 
      if ((i-1) && (index[i] == index[i-2])) {
         continue;
      } else {
         if (fittest > ga_cand[index[i]].fitness_value) {
            fittest = ga_cand[index[i]].fitness_value;
            fittest_index = i;
         }
         i++;
      }

   }      
   /*index_arr[0] = get_random(size, true);
   index_arr[1] = get_random(size, true);
   index_arr[2] = get_random(size, true);
   if (ga_cand[index_arr[0]].fitness_value < ga_cand[index_arr[1]].fitness_value) 
      return index_arr[0];
   return index_arr[1];         */
   return fittest_index;
}   


int tournament_selection(candidate *ga_cand, candidate *gen_children, network_data netinfo, int size) {
   unsigned int i = 0;
   int index = -1;
   candidate *pool = (candidate *)malloc((size) * sizeof(candidate));
   memset(pool, 0, size * sizeof(candidate));
   printf("\nSelect candidates  for crossover operation as below\n");
   while (i < size) {
      index = select_candidates_k(ga_cand, size);
      memcpy(&pool[i], &ga_cand[index], sizeof(candidate));
      printf("\n");
      print_candidate(&pool[i]);
      //memcpy(gen_children[i].binary_enc, ga_cand[index].binary_enc, NL * sizeof(unsigned char));
      i++;
   }
   print_generation(pool, size, false);
   // Remaining half of the next generation will be filled by doing crossover and mutation operations on the first half of the mating pool
   //genetic_ts_crossover(ga_cand, gen_children, netinfo, size);
   genetic_rb_crossover(ga_cand, pool, gen_children, netinfo, size);
   genetic_mutation(gen_children, netinfo, size);
   free(pool);
   memcpy(&ga_cand[1], gen_children, (size-1) * sizeof(candidate));
   return 0;
}
