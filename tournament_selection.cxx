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

#ifdef TOURNAMENT_SELECTION

// The below selection procedure for tournament selection is binary which means it selects 2 candidates at random and returns the fitter one.
int select_candidates_k(candidate *ga_cand, int size) {
   int index_arr[2] = {-1, -1};
   index_arr[0] = get_random(size, true);
   index_arr[1] = get_random(size, true);
   if (ga_cand[index_arr[0]].fitness_value < ga_cand[index_arr[1]].fitness_value) 
      return index_arr[0];
   return index_arr[1];         
}   


int tournament_selection(candidate *ga_cand, candidate *gen_children, network_data netinfo, int size) {
   unsigned int i = 0;
   int index = -1;
   candidate *pool = (candidate *)malloc((size) * sizeof(candidate));
   memset(pool, 0, size * sizeof(candidate));
   printf("\nSelect candidates  for crossover operation as below\n");
   while (i < size ){
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


int genetic_ts_crossover(candidate* ga_cand, candidate *gen_children, network_data netinfo, int size) {
 int i, j, k=1, l, i1, i2, m, rand_elem, bits;
 int intervals, tempval, srand, value;
 int index[2] = {-1, -1};
 int retry=0;
 double rand_val;
 candidate temp, temp1;
 candidate *temp_gen;
 int dup_parent_gen = 0;
 int dup_child_gen = 0;
 int dup_temp_gen = 0;
 int retry_attempts = 0;
 int error_flag = 0;

 printf("\nEntering crossover function\n");

 //gen_children = (candidate *)(malloc(sizeof(candidate)));
 //memset(gen_children, 0, sizeof(gen_children));
 temp_gen = (candidate *)(malloc(GA_POPULATION_SIZE * sizeof(candidate)));
 memset(temp_gen, 0, sizeof(candidate) * GA_POPULATION_SIZE);
 memset(&temp, 0, sizeof(candidate));
 memcpy(&temp_gen[0].binary_enc, &gen_children[0].binary_enc, sizeof(candidate));

 //bits = ceil(log2((double)NL));
 //intervals = (round((log2((double)RAND_MAX)))) / bits;
 if (NL) {
    intervals = (round((log2((double)RAND_MAX)))) / NL;
    tempval = pow(2, NL) - 1;
 }
 //tempval = !(1<<(NL-1)) + 1;
 //srand(time(NULL));
 i = rand_elem = rand();
 //i = rand_elem;
 l = intervals;
 rand_val = (double)rand() / (double)RAND_MAX;

while(1) {
    if (error_flag) {
          retry = 0;
          retry_attempts = 0;
          rand_val = (double)rand() / (double)RAND_MAX;
    } 
    if (k >= size) break;
    if (rand_val > cross_prob) {
       rand_val = (double)rand() / (double)RAND_MAX;
       continue;
    }
    if (l==0) {
       rand_elem = rand();
       l = intervals;
    }

    value = rand_elem & tempval;
    l--;
    rand_elem = rand_elem >> NL;
    if (value == 0) continue;

    value = get_random(NL, false);
    printf("\nCrossover point is %d\n", value);
    i1 = get_random(size/2, true);
    i2 = get_random(size/2, true);
    if (i1 == i2) continue;
/* Following memory copy operations do not reference the binary_enc member inside structure candidate for every gen_children because it is 
   just a shortcut since the first member in the candidate data structure is binary_enc and the memcpy will work correctly without specfiying
   the binary_enc */
//    memcpy(&gen_children[k], &(ga->population[j].binary_enc), i);
    memcpy(&temp_gen[k].binary_enc, &(gen_children[i1].binary_enc), value);
//    memcpy((char *)(&gen_children[k]) + i, (char *)(&ga->population[j+1]) + i, sizeof(temp) - i);
    memcpy((char *)(&temp_gen[k]) + value, (char *)(&gen_children[i2]) + value, sizeof(temp) - value);

    if (compare(temp_gen[k])) {
       memcpy(&temp_gen[k], &(gen_children[i2].binary_enc), value);
       memcpy((char *)(&temp_gen[k]) + value, (char *)(&gen_children[i1]) + value, sizeof(temp) - value);
    }
// Still need to handle the scenarios where we need to decide what to do after retry attempts are equal to 5. Need to do this for all conditions below
    if (compare(temp_gen[k])) {
       if (retry == 5) {
          printf("\nMade 5 attempts to avoid generating a zero child but now we will not perform any more crossover attempts for this pair \
                    of candidates and move forward to the next pair\n");
          /*retry = 0;
          retry_attempts = 0;
          rand_val = (double)rand() / (double)RAND_MAX;*/
          error_flag = 1;
          continue;
       }
       retry++;
       continue;
    }

    if (feasibility(temp_gen[k], netinfo)) {  //Possibly consider to also increment retry here to avoid infinite attempts
       printf("\nChild from crossover not feasible\n");
       memset(&temp_gen[k], 0, sizeof(candidate));
       if (retry_attempts == MAX_CROSSOVER_ATTEMPTS) {
          printf("\nEnough attempts have been made to do a crossover operation for this pair but all crossovers have resulted in infeasible \
                    children\n");
          /*rand_val = (double)rand() / (double)RAND_MAX;
          retry_attempts = 0;
          retry = 0;*/
          error_flag = 1;
          continue;
       }
       retry_attempts++;
       continue;
    }

    dup_parent_gen = check_duplicate(&temp_gen[k], ga_cand, GA_POPULATION_SIZE);

    if (dup_parent_gen) {
       if (retry_attempts == MAX_CROSSOVER_ATTEMPTS) {
          //printf("\nAlready tried enough attempts at generating a unique child from crossover but now we will consider this even though \
                    its a duplicate\n");
          printf("\nAlready tried enough attempts at generating a unique child from crossover so we will not consider the crossover for this \
                    pair and continue with the next consecutive pair\n");
          memset(&temp_gen[k], 0, sizeof(candidate));
          /*rand_val = (double)rand() / (double)RAND_MAX;
          retry_attempts = 0;
          retry = 0;*/
          error_flag = 1;
          continue;
       } else {
          printf("\nThis crossover operation has generated a child that is a duplicate of one of the candidates in the parent population. hence \
                 we need to repeat this crossover operation for this pair of candidates with a different crossover point.\n");
          memset(&temp_gen[k], 0, sizeof(candidate));
          retry_attempts++;
          continue;
       }
    }

    if (k) { dup_child_gen = check_duplicate(&temp_gen[k], gen_children, size/2); }

    if (k && dup_child_gen) {
       printf("\nThis child generated from a crossover operation is already present in this population of children. We do not consider it \
                 and continue with no more attempts\n");
       memset(&temp_gen[k], 0, sizeof(candidate));
       /* rand_val = (double)rand() / (double)RAND_MAX;
       retry = 0;
       retry_attempts = 0; */
       error_flag = 1;
       continue;
    }

    //if (k) { dup_temp_gen = check_duplicate(&temp_gen[k], &temp_gen[0], k); }

 /*   if (k && dup_temp_gen) {
       printf("\nThis child generated from a crossover operation is already present in this population of children. We do not consider it \
                 and continue with no more attempts\n");
       memset(&temp_gen[k], 0, sizeof(candidate));
       // rand_val = (double)rand() / (double)RAND_MAX;
       retry = 0;
       retry_attempts = 0; 
       error_flag = 1;
       continue;
    }*/

    retry = 0;
    retry_attempts = 0;
    k++;
    error_flag = 0;
    rand_val = (double)rand() / (double)RAND_MAX;
    if (k >= size) break;
 }
 memcpy(&gen_children[0], &temp_gen[0], sizeof(candidate) * GA_POPULATION_SIZE);
 free(temp_gen);
 printf("\nExiting crossover function\n");
 return 0;
}

#endif
