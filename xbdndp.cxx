/********************************************************
  file xbdndp.cxx
  ````````````````
  Implementing genetic algorithm as a metaheuristic for a
  bilevel formulation of the discrete network design problem.

  (c) 2015 Private
      author: Nishanth Nagendra, Mar. 2015
********************************************************/

#include <defs.h>
#include <data.h>
#include <funcs.h>
#include <time.h>

int Budget;

/***********************************************************************/

int main(int argc, char **argv)
{

/***Declarations***/

 //model_data dndp;            /* Structure variable representing all modeling data */

 genetic_algo ga;            /* Genetic algorithm instance */

 network_data netinfo;      /* Structure variable representing network information */

 candidate *gen_children;   /* New set of children created from crossover or mutation operations */
 
 int i, j, k, l, m;           /* Iteration variables */
 float ta, xa, ba, ca, Constant;
 int r, s, flag = 0, count = 0, ret = 0, pool_size;
 float temp, best;
 clock_t start, end;
 double cpu_time_used;

 printf("\nWARNING:  Please remember to change the settings for low budget problems to avoid getting stuck in an infinite loop. For low budget problems check_duplicate function is not a good idea as there are few feasible candidates in problems for low budget and one cannot avoid duplicates\n\n");

 ga.population_size = GA_POPULATION_SIZE;
 ga.iterations = GA_ITERATIONS;
 ga.population = (candidate *)malloc( (ga.population_size) * sizeof(candidate));
 memset(ga.population, 0, sizeof(ga.population_size) * sizeof(candidate));
 
 gen_children = (candidate *)(malloc(ga.population_size * sizeof(candidate)));
 memset(gen_children, 0, sizeof(gen_children)); 

 if (argc < 2) {
    printf("\nIncorrect number of command line arguments\n");
    exit(0);
 }

 Budget = atoi(argv[1]);

 //gen_children = NULL;

/***Initial population generated by a set of random candidates***/
 parse(&netinfo);

 //model_problem(&dndp, &netinfo, &ga);

 //i = 0;
 //while (i <= ga.population_size) {
    ret = generate_rand(&ga, netinfo);
  //  i++;
 //}

 i = 0;
 for (i=0; i<ga.iterations; i++) {
    printf("\nGenetic algorithm: Iteration %d\n", i+1);
    printf("\n\n************************************************\n");
    printf("Generation %d\n", i+1);
    print_generation(ga.population, GA_POPULATION_SIZE, false);
       //model_data dndp;
    for (j=0; j<ga.population_size; j++) {
       //dndp.p = XPRBnewprob("TAP");
       model_data dndp;
       //dndp = (model_data *)(malloc(sizeof(model_data)));
       //model_data dndp;
       //memset(dndp, 0, sizeof(model_data));
       printf("\nIteration %d: Candidate %d\n", i+1, j+1);
    /***Construction of new links***/
      /* for (k=0; k<NL; k++) {
          r = netinfo.new_links[k].orig;
          s = netinfo.new_links[k].term;
          dndp.Ya[r][s] = dndp.p.newVar("New link", XPRB_BV);
          printf("\tbin value: %d", ga.population[j].binary_enc[k]);
          dndp.Ya[r][s].fix(ga.population[j].binary_enc[k]);
          printf("\nValue fixed for Ya is: %f", dndp.Ya[r][s].getSol());
       }*/
       //start = clock();
       remodel_problem(&dndp, &netinfo, ga.population[j]);
       //end = clock();
       dndp.p.lpOptimize("");  
       //cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
       //printf("\nActual time: %lf\n", cpu_time_used);
       printf("\nObjective value: %f\n", dndp.p.getObjVal());
       candidate_fitness(&dndp, &netinfo, &(ga.population[j]));
       printf("\n\n************************************************\n");
       printf("Iteration %d results:\n", i+1);
       print_candidate(&ga.population[j]);
       printf("Fitness: %f\n", ga.population[j].fitness_value);
       printf("************************************************\n");
       
       /*for (k=0; k<NL; k++) {
          dndp->Ya[r][s].fix(0.0);
       }*/
       //free(dndp);
       //dndp.p.reset();
       //XPRBdelprob(&(dndp.p));
    }

    printf("\n\n************************************************\n");
    printf("Generation %d(Before sorting)\n", i+1);
    print_generation(ga.population, GA_POPULATION_SIZE, true);

    candidates_sort(ga.population, GA_POPULATION_SIZE);

    printf("\n\n************************************************\n");
    printf("Generation %d(After sorting)\n", i+1);
    print_generation(ga.population, GA_POPULATION_SIZE, true);

    pool_size = genetic_sp_crossover(&ga, gen_children, netinfo);
    printf("\n\n************************************************\n");
    printf("Children generated after crossover for Generation %d\n", i+1);
    print_generation(gen_children, pool_size, false);

    genetic_mutation(gen_children, netinfo, pool_size);
    printf("\n\n************************************************\n");
    printf("Children generated after mutation for Generation %d\n", i+1);
    print_generation(gen_children, pool_size, false);

    k = 0; j = 0;
    //pool_size = sizeof(gen_children)/sizeof(candidate);
    for (k=0; k<pool_size; k++) {
       model_data dndp; 
       printf("Iteration(Generation): %d, Child no: %d\n", i+1, k+1);
       /*for (j=0; j<NL; j++) {
          r = netinfo.new_links[j].orig;
          s = netinfo.new_links[j].term;
          dndp.Ya[r][s] = dndp.p.newVar("New link", XPRB_BV);
          dndp.Ya[r][s].fix(gen_children[k].binary_enc[j]);
          printf("\nValue fixed for Ya is: %f", dndp.Ya[r][s].getSol());
       }*/
       remodel_problem(&dndp, &netinfo, gen_children[k]); 
       dndp.p.lpOptimize("");
       printf("\nObjective value: %f\n", dndp.p.getObjVal());
       candidate_fitness(&dndp, &netinfo, &gen_children[k]);
       printf("\n\n************************************************\n");
       printf("Iteration %d results:\n", i+1);
       print_candidate(&gen_children[k]);
       printf("Fitness: %f\n", gen_children[k].fitness_value);
       printf("************************************************\n");
       //XPRBdelprob(dndp.p);
    } 

    candidates_sort(gen_children, pool_size);
    printf("\n\n************************************************\n");
    printf("Children sorted after crossover and mutation for Generation %d\n", i+1);
    print_generation(gen_children, pool_size, true);

    //best = ga.population[0].fitness_value;
    //if (best < gen_children[0].fitness_value) best = gen_children[0].fitness_value;

/***Select the candidates for the next generation from the pool of children and current population***/
   candidate *new_gen = (candidate *)(malloc(GA_POPULATION_SIZE * sizeof(candidate)));
   memset(new_gen, 0, sizeof(new_gen));

   l = 0; k = 0; j = 0;
   //memcpy(new_gen, ga.population, sizeof(candidate) * GA_POPULATION_SIZE);
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

   if (k == pool_size) {
      while(l < GA_POPULATION_SIZE && j < GA_POPULATION_SIZE) {
         if (l && check_duplicate(&ga.population[j], new_gen, l)) {
            printf("\nIgnoring this child\n");
            j++;
            continue;
         }
         memcpy(&new_gen[l], &(ga.population[j]), sizeof(candidate));
         l++; j++;
      }  
      if (l < GA_POPULATION_SIZE) { 
         printf("\nMerge of parent and child populations could not happen to create a new generation hence keeping the old parent generation for next iteration as well\n");
         memcpy(new_gen, ga.population, sizeof(candidate) * GA_POPULATION_SIZE);
      }
   } else {
      if (l < GA_POPULATION_SIZE) {  
         printf("\nMerge of parent and child populations could not happen to create a new generation hence keeping the old parent generation for next iteration as well\n");
         memcpy(new_gen, ga.population, sizeof(candidate) * GA_POPULATION_SIZE);
      }
   }

   free(ga.population);
   ga.population = new_gen;
   new_gen = NULL;

 }

/*

 k = 0;

 while(1) {
    i = (rand() % NL);
    printf("\n%d\t", i);
    //i = ga.population[count].binary_enc = (rand() % NL);
    if (i == 0) continue;
    while(i) {
       ga.population[count].binary_enc[k++] = i&1;
       i = i>>1;
       printf("%d ", ga.population[count].binary_enc[k-1]);
    }
    //printf("\n%d\n", ga.population[count].binary_enc);
    count++;
    if (count == 5) break;
 }*/

 printf("\nFinal objective value for DNDP is %f\n", ga.population[0].fitness_value);
 //printf("\nFinal objective value for DNDP is %f\n", best);

 free(gen_children);
 free(ga.population);
 gen_children = NULL;

 clean(&netinfo);
 /* Do the work. */
 //end = clock();
 //cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
 
 printf("\nActual time: %lf\n", cpu_time_used);

 return 0;  
}
