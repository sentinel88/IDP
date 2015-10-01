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
int selection_scheme;
int cost_function_selector;

static int initialize(genetic_algo *ga, candidate **gen_children, candidate **cache, const char **argv) {
   int i = 0;
   int selector;

   printf("\nInside initialize function\n");

   ga->population_size = GA_POPULATION_SIZE;
   ga->iterations = GA_ITERATIONS;
   ga->population = (candidate *)malloc((ga->population_size) * sizeof(candidate));
   //memset(ga->population, 0, sizeof(ga->population_size) * sizeof(candidate));

   *gen_children = (candidate *)malloc((ga->population_size) * sizeof(candidate));
   //memset(gen_children, 0, sizeof(gen_children)); 
   //memset(gen_children, 0, ga->population_size * sizeof(candidate));
   *cache = (candidate *)malloc((2 * ga->population_size) * sizeof(candidate));
   //memset(cache, 0, sizeof(cache)); 
   //memset(cache, 0, 2 * ga->population_size * sizeof(candidate));

   Budget = atoi(argv[1]);
   if (Budget < 10 || (Budget % 10 != 0)) {
      printf("\nIncorrect budget specified\n");
      return -1;
   }

   selection_scheme = atoi(argv[2]);
   if (selection_scheme < 0 || selection_scheme > 2) {
      printf("\nIncorrect value of the selection_scheme\n");
      return -1;
   }

   selector = atoi(argv[3]);
   cost_function_selector = selector;
   if (cost_function_selector < 1 || cost_function_selector > 2) {
      printf("\nIncorrect value of the cost_function_selector\n");
      return -1;
   }

   printf("\nExiting initialize function\n");
   return 0;

}


/***********************************************************************/

int main(int argc, const char **argv)
{
 printf("Enter main\n");

 genetic_algo ga;            /* Genetic algorithm instance */

 network_data netinfo;      /* Structure variable representing network information */

 candidate *gen_children;   /* New set of children created from crossover or mutation operations */
 candidate *cache;       /* Cache that currently stores 1 full previous generation and its fitness value to avoid unecessary recomputatio  */
 candidate *new_gen;

 double last_best_fitness = 99999999999.0000;
 int last_best_iter = 0;
 int last_best_index = -1;
 int index = -1;

 double total_fitness = 0;
 
 int i, j, k, l, m, n;           /* Iteration variables */
 float ta, xa, ba, ca, Constant;
 int r, s, flag = 0, count = 0, ret = 0, pool_size = 0;
 int orig, term;
 int ret_val = 0;
 float temp, best;
 //clock_t start, end;
 time_t start, end;
 double cpu_time_used;

 if (argc < 3) {
    printf("\nIncorrect number of command line arguments\n");
    exit(0);
 }

 printf("\nWARNING:  Please remember to change the settings for low budget problems to avoid getting stuck in an infinite loop. For low budget problems check_duplicate function is not a good idea as there are few feasible candidates in problems for low budget and one cannot avoid duplicates\n\n");

 selection_scheme = DEFAULT_SELECTION;
 cost_function_selector = 1;

 ret_val = initialize(&ga, &gen_children, &cache, argv);
 if (ret_val < 0) {
    printf("\nIncorrect command line arguments\n");
    exit(0);
 }

 init_net_data(&netinfo);

 ret_val = parse(&netinfo);
 if (ret_val < 0) {
    printf("\nExiting\n");
    cleanup_net_data(&netinfo);
    return 0;   
 }

//#ifdef DONT_EXECUTE_NOW
 ret = generate_rand(&ga, &netinfo);   
//#endif

 //start = clock();
 start = time(NULL);
 for (i=0; i<ga.iterations; i++) {
    #ifdef ROULETTE_WHEEL_SELECTION
       total_fitness = 0;
    #endif

    printf("\nGenetic algorithm: Iteration %d\n", i+1);
#ifdef _DEBUG
    printf("\n\n************************************************\n");
    printf("Generation %d\n", i+1);
    print_generation(ga.population, GA_POPULATION_SIZE, false);
#endif
    end = time(NULL);
    //cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    cpu_time_used = (end - start);
    printf("\nActual time: %lf\n", cpu_time_used);
    for (j=0; j<ga.population_size; j++) {
    //for (j=0; j<1; j++) {
       index = -1;
       model_data *dndp = new model_data;
       printf("\nIteration %d: Candidate %d\n", i+1, j+1);
       //start = clock();
       if (i) {
          if (cache_lookup(&ga.population[j], cache, &index, GA_POPULATION_SIZE + pool_size)) {
             printf("\nCache miss. Will have to solve the TAP for this candidate\n");
          } else {
             printf("\nCache hit. Do not need to solve the TAP for this candidate. Setting the fitness as computed before\n");
             ga.population[j].fitness_value = cache[index].fitness_value;
             continue;
          } 
       }
       model_problem(dndp, &netinfo, ga.population[j]);
    //end = clock();
    end = time(NULL);
    //cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    cpu_time_used = (end - start);
    printf("\nActual time: %lf\n", cpu_time_used);
       //end = clock();
       dndp->p.lpOptimize("");  
    end = time(NULL);
    //cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    cpu_time_used = (end - start);
    printf("\nActual time: %lf\n", cpu_time_used);
       //cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	       //printf("\nActual time: %lf\n", cpu_time_used);
#ifdef _DEBUG
       print(&ga, &netinfo, dndp, j);
#endif
       printf("\nObjective value: %f\n", dndp->p.getObjVal());
       candidate_fitness(dndp, &netinfo, &(ga.population[j]));
#ifdef _DEBUG
       printf("\n\n************************************************\n");
       printf("Iteration %d results:\n", i+1);
       print_candidate(&ga.population[j]);
       printf("Fitness: %f\n", ga.population[j].fitness_value);
       printf("************************************************\n");
#endif
    #ifdef ROULETTE_WHEEL_SELECTION
       total_fitness += ga.population[j].fitness_value;
    #endif

       if (selection_scheme == TOURNAMENT_SELECTION) {
          if (last_best_fitness >= ga.population[j].fitness_value) {
             last_best_fitness = ga.population[j].fitness_value;
             last_best_index = j;
          }
       }

       delete dndp;
    //end = clock();
    end = time(NULL);
    //cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    cpu_time_used = (end - start);
    printf("\nActual time: %lf\n", cpu_time_used);
    }
    //end = clock();
    end = time(NULL);
    //cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    cpu_time_used = (end - start);
    printf("\nActual time: %lf\n", cpu_time_used);
#ifdef _DEBUG
    printf("\n\n************************************************\n");
    printf("Generation %d(Before sorting)\n", i+1);
    print_generation(ga.population, GA_POPULATION_SIZE, true);
#endif
    //memcpy(&gen_children[0], &ga.population[last_best_index], sizeof(candidate));
//#endif
    if (selection_scheme == TOURNAMENT_SELECTION) {
        if (i < (ga.iterations - 1)) {
           memcpy(cache, ga.population, sizeof(candidate) * GA_POPULATION_SIZE);
           //memset(gen_children, 0, sizeof(candidate) * GA_POPULATION_SIZE);
           tournament_selection(ga.population, gen_children, &netinfo, GA_POPULATION_SIZE);
           memcpy(&ga.population[0], &cache[last_best_index], sizeof(candidate));
       	   last_best_index = 0;
    	}
    } else {
	candidates_sort(ga.population, GA_POPULATION_SIZE);

	memcpy(cache, ga.population, sizeof(candidate) * GA_POPULATION_SIZE);
    /* One of the other termination criterias for genetic algorithm is objective function value */ 
	if (i == 0) {
	   last_best_fitness = ga.population[0].fitness_value;
	} else {
	   if (last_best_fitness == ga.population[0].fitness_value) {
	      last_best_iter++;
	      if (last_best_iter == MAX_BEST_ITERATIONS) {
		 printf("\nGenetic algorithm terminates because the objective function value has remained the same for a long time\n");
		 printf("\nFinal objective value for DNDP is %f\n", ga.population[0].fitness_value);
		 return 0;
	      }
	   } else if (last_best_fitness > ga.population[0].fitness_value) {
		 last_best_fitness = ga.population[0].fitness_value;
		 last_best_iter = 0;
	   } else {
	   /* last_best_fitness < ga.population[0].fitness_value */
	   /* Do nothing currently because last_best_fitness cannot be less than the current best since while creating this generation the best
	      fitness will be on top and in such a case last_best_fitness should retain its top place */
	   }
	}
#ifdef _DEBUG
	printf("\n\n************************************************\n");
	printf("Generation %d(After sorting)\n", i+1);
	print_generation(ga.population, GA_POPULATION_SIZE, true);
#endif
	if (selection_scheme == RANK_BASED_SELECTION) {
	    if (i < (ga.iterations - 1)) {
	       assign_selection_rb_prob(ga.population, GA_POPULATION_SIZE);
	       memcpy(cache, ga.population, sizeof(candidate) * GA_POPULATION_SIZE);
	       rank_based_selection(ga.population, gen_children, &netinfo, GA_POPULATION_SIZE);
	    }
	} else {
	   //pool_size = default_selection(&ga, gen_children, &netinfo, &new_gen, cache);
	   //ret_val = default_selection(&ga, gen_children, &netinfo, cache);
	   if (i < (ga.iterations - 1)) {
	      ret_val = default_selection(ga.population, gen_children, &netinfo, GA_POPULATION_SIZE);
	      if (ret_val < 0) {
	         printf("\nError encountered in the execution of the default selection algorithm\n");
	         break;
	      }
           }
        }
    }   

   printf("\nFinished creating the next generation\n");
//#ifdef RANK_BASED_SELECTION
   /* Do nothing as the  next generation of population is already present in ga.population */
   if (selection_scheme != DEFAULT_SELECTION) {
   //ga.population = gen_children;*/
   } else {
      /*free(ga.population);
      printf("\nFreed ga population\n");
      ga.population = new_gen;*/
      new_gen = NULL;
   }
   //free(gen_children);
   //gen_children = (candidate *)malloc((ga.population_size) * sizeof(candidate));
//#endif
 }

 printf("\nFinal objective value for DNDP is %f\n", ga.population[0].fitness_value);
 //printf("\nFinal objective value for DNDP is %f\n", best);
 free(gen_children);
 free(ga.population);
 free(cache);
 gen_children = NULL;

 //clean(&netinfo);
 cleanup_net_data(&netinfo);
 /* Do the work. */
 //end = clock();
 //cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
 
 //printf("\nActual time: %lf\n", cpu_time_used);

 return 0;  
}
