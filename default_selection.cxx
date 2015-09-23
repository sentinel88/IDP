#include <defs.h>
#include <data.h>
#include <funcs.h>


static void merge_sort(candidate *new_gen, genetic_algo *ga, candidate *gen_children, int pool_size) {
   printf("\nInside merge_sort function\n");

   int l = 0, k = 0, j = 0;

   while(1) {
      if (l == GA_POPULATION_SIZE || j == GA_POPULATION_SIZE || k == pool_size) break;
      if (gen_children[k].fitness_value < ga->population[j].fitness_value) {
#ifdef _PROTECT
         //if (k && check_duplicate(&gen_children[k], ga.population, GA_POPULATION_SIZE)) {
         if (l && check_duplicate(&gen_children[k], new_gen, l)) {
            printf("\nIgnoring this child\n");
            k++;
            continue;
         }
#endif
         memcpy(&new_gen[l], &gen_children[k], sizeof(candidate));
         k++;
      } else {
#ifdef _PROTECT
            //if (k && check_duplicate(&gen_children[k], ga.population, GA_POPULATION_SIZE)) {
            if (l && check_duplicate(&ga->population[j], new_gen, l)) {
               printf("\nIgnoring this child\n");
               j++;
               continue;
            }
#endif
         memcpy(&new_gen[l], &(ga->population[j]), sizeof(candidate));
         j++;
      }
      l++;
      //k++; j++;
      if (l == GA_POPULATION_SIZE || j == GA_POPULATION_SIZE || k == pool_size) break;
   }

   printf("\nStatus:  l = %d, k = %d, j = %d, pool_size = %d\n\n", l, k, j, pool_size);

   if (k == pool_size) {
      while(l < GA_POPULATION_SIZE && j < GA_POPULATION_SIZE) {
#ifdef _PROTECT
         if (l && check_duplicate(&ga->population[j], new_gen, l)) {
            printf("\nIgnoring this child\n");
            j++;
            continue;
         }
#endif
         memcpy(&new_gen[l], &(ga->population[j]), sizeof(candidate));
         l++; j++;
      }
#ifdef _PROTECT
      if (l < GA_POPULATION_SIZE) {
         printf("\nInside if: Merge of parent and child populations could not happen to create a new generation hence keeping the old parent generation for next iteration as well\n");
         int i = 0;
         while (i < GA_POPULATION_SIZE) {
            memcpy(&new_gen[i], &ga->population[i], sizeof(candidate));
            printf("\nCopied candidate no. %d of parent population to next generation\n", i+1);
            i++;
         }
      }
#endif
   } else {
#ifdef _PROTECT
      if (l < GA_POPULATION_SIZE) {
         printf("\nInside else: Merge of parent and child populations could not happen to create a new generation hence keeping the old parent generation for next iteration as well\n");
         int i = 0;
         while (i < GA_POPULATION_SIZE) {
            memcpy(&new_gen[i], &ga->population[i], sizeof(candidate));
            i++;
         }
      }
#endif
   }
   printf("\nExiting merge_sort function\n");
}


int default_selection(candidate *ga_cand, candidate *gen_children, network_data *netinfo, int size) {
   genetic_crossover(ga_cand, ga_cand, gen_children, netinfo, size);
   genetic_mutation(gen_children, netinfo, size);
   memcpy(&ga_cand[1], gen_children, (size - 1) * sizeof(candidate));
   return 0;
}

