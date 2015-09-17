#include <defs.h>
#include <data.h>
#include <funcs.h>


static void merge_sort(candidate *new_gen, genetic_algo *ga, candidate *gen_children) {
   printf("\nInside merge_sort function\n");

   int l = 0, k = 0, j = 0;

   while(1) {
      if (l == GA_POPULATION_SIZE || j == GA_POPULATION_SIZE || k == pool_size) break;
      if (gen_children[k].fitness_value < ga.population[j].fitness_value) {
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
            if (l && check_duplicate(&ga.population[j], new_gen, l)) {
               printf("\nIgnoring this child\n");
               j++;
               continue;
            }
#endif
         memcpy(&new_gen[l], &(ga.population[j]), sizeof(candidate));
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
         if (l && check_duplicate(&ga.population[j], new_gen, l)) {
            printf("\nIgnoring this child\n");
            j++;
            continue;
         }
#endif
         memcpy(&new_gen[l], &(ga.population[j]), sizeof(candidate));
         l++; j++;
      }
#ifdef _PROTECT
      if (l < GA_POPULATION_SIZE) {
         printf("\nInside if: Merge of parent and child populations could not happen to create a new generation hence keeping the old parent generation for next iteration as well\n");
         int i = 0;
         while (i < GA_POPULATION_SIZE) {
            memcpy(&new_gen[i], &ga.population[i], sizeof(candidate));
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
            memcpy(&new_gen[i], &ga.population[i], sizeof(candidate));
            i++;
         }
      }
#endif
   }
   printf("\nExiting merge_sort function\n");
}


int default_selection(genetic_algo *ga, candidate *gen_children, network_data *netinfo) {
    pool_size = genetic_sp_crossover(&ga, gen_children, netinfo);
#ifdef _DEBUG
    printf("\n\n************************************************\n");
    printf("Children generated after crossover for Generation %d\n", i+1);
    print_generation(gen_children, pool_size, false);
#endif
    genetic_mutation(gen_children, netinfo, pool_size);
#ifdef _DEBUG
    printf("\n\n************************************************\n");
    printf("Children generated after mutation for Generation %d\n", i+1);
    print_generation(gen_children, pool_size, false);
#endif
    for (k=0; k<pool_size; k++) {
       model_data dndp;
       printf("Iteration(Generation): %d, Child no: %d\n", i+1, k+1);
       model_problem(dndp, netinfo, gen_children[k]);
       dndp.p.lpOptimize("");
       printf("\nObjective value: %f\n", dndp.p.getObjVal());
       candidate_fitness(dndp, netinfo, &gen_children[k]);
#ifdef _DEBUG
       printf("\n\n************************************************\n");
       printf("Iteration %d results:\n", i+1);
       print_candidate(&gen_children[k]);
#endif
       printf("Fitness: %f\n", gen_children[k].fitness_value);
       printf("************************************************\n");
    }

    candidates_sort(gen_children, pool_size);
#ifdef _DEBUG
    printf("\n\n************************************************\n");
    printf("Children sorted after crossover and mutation for Generation %d\n", i+1);
    print_generation(gen_children, pool_size, true);
#endif
    memcpy((candidate *)cache + GA_POPULATION_SIZE, gen_children, sizeof(candidate) * pool_size);

/***Select the candidates for the next generation from the pool of children and current population***/
   candidate *new_gen = (candidate *)(malloc(GA_POPULATION_SIZE * sizeof(candidate)));
   memset(new_gen, 0, GA_POPULATION_SIZE * sizeof(candidate));

   merge_sort(new_gen, ga, gen_children);
   return 0;
}

