#include <defs.h>
#include <data.h>
#include <funcs.h>

int generate_rand(genetic_algo *ga, network_data *netinfo) {
    int i, j, count, k, rand_elem, intervals;
    int value, temp;
    int retry_attempts = 0;
    printf("\nEntering generate_rand\n");
    count = 0;
    k = 0;
    if (NL)
    intervals = (round((log2((double)RAND_MAX))))/NL;
    temp = round((pow(2, NL) - 1));
    srand(time(NULL));

    while(1) {
	k = 0;
	j = intervals;
	rand_elem = rand();
	while(j--) {
	   value = rand_elem & temp;
	   if (value == 0) continue;
	   encode_ga_cand(&(ga->population[count]), value);
	   rand_elem = rand_elem >> NL;
       
	   /* Keeping feasibility check and elem_compare checks separate so that going forward if needed we can surround the code for elem_compare
	      checks by macros in case of problems with low budgets where having duplicate candidates cannot be avoided as there are only very few 
	      feasible candidates */

	   if (feasibility(ga->population[count], netinfo)) {
	      memset(&(ga->population[count].binary_enc), 0, NL);
	      printf("\nNot feasible\n");
	      continue;
	   } else {
	      if (count && check_duplicate(&(ga->population[count]), ga->population, count)) {
		 if (retry_attempts == MAX_RETRY) {
		    printf("\n5 retry attempts have been made to regenerate a random candidate so now we will consider the duplicate itself and proceed forward\n");
		    retry_attempts = 0;
		    count++;
		    continue;
		 } else {
		    retry_attempts++;
		    printf("\nDuplicate random candidate generated. Ignoring it and continuing with further attempts.\n");
		    memset(&(ga->population[count].binary_enc), 0, NL);
		    continue;
		 }
	      } 
	      count++; 
	      retry_attempts = 0;
	   }
	   //printf("\n%d\n", ga.population[count].binary_enc);
	   //count++;
	   if (count == GA_POPULATION_SIZE) break;
	}
	if (count == GA_POPULATION_SIZE) break;
    }    
    printf("\nExiting generate_rand\n");
    return 0;
}


int genetic_crossover(candidate* ga_cand, candidate *pool, candidate *gen_children, network_data *netinfo, int size) {
 int i, j, k=0, l, i1, i2, m, rand_elem, bits;
 int intervals, tempval, srand, value;
#ifdef TWO_POINT_CROSSOVER
 int value2;
 int crossover[2];
#endif
 int best_fit_index = -1;
 int retry=0;
 double rand_val;
 candidate temp, temp1;
 candidate *temp_gen;
 int dup_parent_gen = 0;
 int dup_child_gen = 0;
 int dup_temp_gen = 0;
 int retry_attempts = 0;
 int error_flag = 0;
 int offspring_count = 0;
 int real_offspring_count = 0;
 bool switch_order = false;
 bool select_cross = true;

 printf("\nEntering crossover function\n");

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
          offspring_count = 0;
          rand_val = (double)rand() / (double)RAND_MAX;
          error_flag = 0;
          switch_order = false;
          select_cross = true;
    }
    if (k >= (size - 1)) break;
    if (rand_val > cross_prob) {
       rand_val = (double)rand() / (double)RAND_MAX;
       continue;
    }
    /*if (l==0) {
       rand_elem = rand();
       l = intervals;
    }

    value = rand_elem & tempval;
    l--;
    rand_elem = rand_elem >> NL;
    if (value == 0) continue;*/
    printf("\n");
if (select_cross) {
    value = get_random(NL, false);
#ifdef TWO_POINT_CROSSOVER
    do {
       value2 = get_random(NL, false);
    } while(value2 == value);

    if (value < value2) {
       crossover[0] = value;
       crossover[1] = value2;
    } else {
       crossover[0] = value2;
       crossover[1] = value;
    }
#endif
    printf("\n");
    printf("\nCrossover point is %d\n", value);
#ifdef TWO_POINT_CROSSOVER
    printf("\nSecond crossover point is %d\n", value2);
#endif
}
    if (!retry_attempts) {
       i1 = get_random(size, true);
       i2 = get_random(size, true);
       printf("\n");
       if (i1 == i2) continue;

       best_fit_index = i1;
       if (pool[i2].fitness_value < pool[i1].fitness_value)
          best_fit_index = i2;

       if (!memcmp(pool[i1].binary_enc, pool[i2].binary_enc, NL * sizeof(unsigned char)) ) {
          printf("\nThe parent chromosomes at these indexes are the same\n");
          memset(&gen_children[k], 0, sizeof(candidate));
          if (retry == MAX_CROSSOVER_ATTEMPTS) {
             printf("\nEnough attempts have been made to do a crossover operation but the parent chromosome pairs are always same.\n");
             /*rand_val = (double)rand() / (double)RAND_MAX;
             retry_attempts = 0;
             retry = 0;*/
             memcpy((char *)&gen_children[k].binary_enc, (char *)&pool[best_fit_index].binary_enc, NL * sizeof(unsigned char));
             k++;
             error_flag = 1;
             continue;
          }
          //retry_attempts++;
          retry++;
          continue;
       }
   }
/* Following memory copy operations do not reference the binary_enc member inside structure candidate for every gen_children because it is 
   just a shortcut since the first member in the candidate data structure is binary_enc and the memcpy will work correctly without specfiying
the binary_enc */
//    memcpy(&gen_children[k], &(ga->population[j].binary_enc), i);
    if (switch_order) {
       i1 = i1 + i2;
       i2 = i1 - i2;
       i1 = i1 - i2;
       //switch_order = false;
    }

// The below 2 point crossover could be wrong because we are doing the last step of the operation by copying bytes into the new chromosome
// from position crossover[1] onwards using bytes from position crossover[0] onwards of chromosome 1 whereas it should be crossover[1]
#ifdef TWO_POINT_CROSSOVER
    memcpy((char *)&gen_children[k].binary_enc, (char *)&pool[i1].binary_enc, crossover[0]);
    memcpy((char *)&gen_children[k].binary_enc + crossover[0], (char *)&pool[i2].binary_enc + crossover[0], crossover[1] - crossover[0]);
    memcpy((char *)&gen_children[k].binary_enc + crossover[1], (char *)&pool[i1].binary_enc + crossover[0], NL - crossover[1]);
    /*if (k < (size-1)) {
       memcpy((char *)&gen_children[k+1].binary_enc, (char *)&pool[i2].binary_enc, crossover[0]);
       memcpy((char *)&gen_children[k+1].binary_enc + crossover[0], (char *)&pool[i1].binary_enc + crossover[0], crossover[1] - crossover[0]);
       memcpy((char *)&gen_children[k+1].binary_enc + crossover[1], (char *)&pool[i2].binary_enc + crossover[0], NL - crossover[1]);
    }*/
    //memcpy((char *)&gen_children[k].binary_enc + crossover[0], (char *)&pool[i2].binary_enc + crossover[0], crossover[1] - crossover[0]);
#else
    memcpy((char *)&gen_children[k].binary_enc, (char *)&pool[i1].binary_enc, value);
//    memcpy((char *)(&gen_children[k]) + i, (char *)(&ga->population[j+1]) + i, sizeof(temp) - i);
    memcpy((char *)&gen_children[k].binary_enc + value, (char *)&pool[i2].binary_enc + value, sizeof(temp) - value);
    /*if (k < (size-1)) {
       memcpy((char *)&gen_children[k+1].binary_enc, (char *)&pool[i2].binary_enc, value);
       memcpy((char *)&gen_children[k+1].binary_enc + value, (char *)&pool[i1].binary_enc + value, sizeof(temp) - value);
    } */
#endif

//Commented the below section to avoid crossover in the other direction to make the implementation easier and consider the other order only in case we need to switch the order.
    /*if (compare(gen_children[k])) {
       memcpy((char *)&gen_children[k], &pool[i2].binary_enc, value);
       memcpy((char *)&gen_children[k] + value, (char *)&pool[i1].binary_enc + value, NL - value);
    } //else {
       //switch_order == true ? false: true;
//switch_order = true;
    //} */
         // memcpy((char *)&gen_children[k+1].binary_enc, (char *)&pool[i2].binary_enc, value);
         // memcpy((char *)&gen_children[k+1].binary_enc + value, (char *)&pool[i1].binary_enc + value, sizeof(temp) - value);
// Still need to handle the scenarios where we need to decide what to do after retry attempts are equal to 5. Need to do this for all conditions below

    if (check_if_zero(gen_children[k])) {
       //switch_order = false;
       if (retry == MAX_RETRY || offspring_count) {
          printf("\nCrossover for 2nd offspring in the other order unsuccessful or we already Made 5 attempts to avoid generating a zero child but now we will not perform any more crossover attempts for this pair \
                    of candidates\n");
          /*retry = 0;
          retry_attempts = 0;
          rand_val = (double)rand() / (double)RAND_MAX;*/
          if (!offspring_count) {
             memcpy((char *)&gen_children[k].binary_enc, (char *)&pool[best_fit_index].binary_enc, NL * sizeof(unsigned char));
             k++;
          }
          error_flag = 1;
          continue;
       }
       //switch_order = false;
       if (!switch_order) {
          switch_order = true;
          select_cross = false;
       } else {
          select_cross = !select_cross;
       }
       retry++;
       continue;
    }

    if (feasibility(gen_children[k], netinfo)) {  //Possibly consider to also increment retry here to avoid infinite attempts
       printf("\nChild from crossover not feasible\n");
       memset(&gen_children[k], 0, sizeof(candidate));
       if (retry_attempts == MAX_CROSSOVER_ATTEMPTS || offspring_count) {
printf("\nCrossover for the 2nd offspring in the other order is unsuccessful or Enough attempts have been made to do a crossover operation for this pair but all crossovers have resulted in infeasible \
                    children\n");
          /*rand_val = (double)rand() / (double)RAND_MAX;
          retry_attempts = 0;
          retry = 0;*/
          if (!offspring_count) {
             memcpy((char *)&gen_children[k].binary_enc, (char *)&pool[best_fit_index].binary_enc, NL * sizeof(unsigned char));
             k++;
          }
          error_flag = 1;
          continue;
       }
       //switch_order = false;
       if (!switch_order) {
          switch_order = true;
          select_cross = false;
       } else {
          select_cross = !select_cross;
       }
       retry_attempts++;
       continue;
    }

    dup_parent_gen = check_duplicate(&gen_children[k], ga_cand, GA_POPULATION_SIZE);

    if (dup_parent_gen) {
       if (retry_attempts == MAX_CROSSOVER_ATTEMPTS || offspring_count) {
          //printf("\nAlready tried enough attempts at generating a unique child from crossover but now we will consider this even though \
                    its a duplicate\n");
          printf("\nCrossover for the 2nd offspring in the other order is unsuccessful or we have Already tried enough attempts at generating a unique child from crossover. Reached max. limit for crossover attempts.\n");
          //memset(&gen_children[k], 0, sizeof(candidate));
          if (!offspring_count) {
             memcpy((char *)&gen_children[k].binary_enc, (char *)&pool[best_fit_index].binary_enc, NL * sizeof(unsigned char));
             k++;
          }
          /*rand_val = (double)rand() / (double)RAND_MAX;
 retry_attempts = 0;
          retry = 0;*/
          error_flag = 1;
          continue;
       } else {
          printf("\nThis crossover operation has generated a child that is a duplicate of one of the candidates in the parent population. hence \
                 we need to repeat this crossover operation for this pair of candidates with a different crossover point.\n");
          memset(&gen_children[k], 0, sizeof(candidate));
          //switch_order = false;
          if (!switch_order) {
             switch_order = true;
             select_cross = false;
          } else {
             select_cross = !select_cross;
          }
          retry_attempts++;
          continue;
       }
    }

    if (k) { dup_child_gen = check_duplicate(&gen_children[k], gen_children, k); }

    if (k && dup_child_gen) {
       printf("\nThis child generated from a crossover operation is already present in this population of children. We do not consider it \
                 and continue with no more attempts\n");
       if (retry_attempts == MAX_CROSSOVER_ATTEMPTS || offspring_count) {
          printf("\nCrossover for the 2nd offspring in the other order is unsuccessful or we have Already tried enough attempts at generating a unique child from crossover. Reached max. limit for crossover attempts.\n");
          if (!offspring_count) {
             memcpy((char *)&gen_children[k].binary_enc, (char *)&pool[best_fit_index].binary_enc, NL * sizeof(unsigned char));
             k++;
          }
          /* rand_val = (double)rand() / (double)RAND_MAX;
          retry = 0;
          retry_attempts = 0; */
          error_flag = 1;
          continue;
       } else {
          memset(&gen_children[k], 0, sizeof(candidate));
          //switch_order = false;
          if (!switch_order) {
             switch_order = true;
             select_cross = false;
          } else {
             select_cross = !select_cross;
          }
          retry_attempts++;
          continue;
       }
    }

    retry = 0;
    retry_attempts = 0;
    k++;
    error_flag = 0;
    offspring_count = (offspring_count+1) % 2;
    if (!offspring_count) rand_val = (double)rand() / (double)RAND_MAX;
    if (k >= (size - 1)) break;
    //if (!switch_order) { retry_attempts = 1; switch_order = true; } else { switch_order = false; }
    if (!offspring_count) {
       retry_attempts = 0;
       switch_order = false;
       select_cross = true;
    } else {
       retry_attempts = 1;
       select_cross = false;
       switch_order = true;
    }
 }
 //memcpy(&gen_children[0], &temp_gen[0], sizeof(candidate) * GA_POPULATION_SIZE);
 //free(temp_gen);
 printf("\nExiting crossover function\n");
 return 0;
}


int genetic_mutation(candidate *gen_children, network_data *netinfo, int pool_size) {
   printf("\nEntering mutation routine\n");
   candidate temp, temp1;
   double rand_elem;
   int pos_mutate;
   int range = RAND_MAX;
   int k = 0;
   int retry = 0;
   int i,j,l;
   int tempval, intervals, value;
   if (NL) {
      intervals = (round((log2((double)RAND_MAX)))) / NL;
      tempval = pow(2, NL) - 1;
   }
   srand(time(NULL));
   i = rand();
   l = intervals;
   rand_elem = (double)rand() / (double)range;
   while(1) {
      if (k >= pool_size) break;
      if (rand_elem > mut_prob) {
         rand_elem = (double)rand() / (double)range;
         k++;
         continue;
      }
      if (l == 0) {
         i = rand();
         l = intervals;
      }
      value = i & tempval;
      l--;
      i = i >> NL;
      if (value == 0) {
         if (retry == MAX_RETRY) { retry = 0; k++; continue; }
         retry++;
         continue;
      }
      pos_mutate = count_set_bits(value);
      memcpy(&temp, &gen_children[k], sizeof(candidate));
      gen_children[k].binary_enc[pos_mutate] = (gen_children[k].binary_enc[pos_mutate] + 1)%2;
      if (check_if_zero(gen_children[k])) {
         memcpy(&gen_children[k].binary_enc, &temp, sizeof(candidate));
         k++;
         rand_elem = (double)rand() / (double)range;
         retry = 0;
         continue;
      }
      if (feasibility(gen_children[k], netinfo) != 0) {
         memcpy(&gen_children[k].binary_enc, &temp, sizeof(candidate));
         if (retry == MAX_RETRY) { retry = 0; k++; continue; }
         retry++;
         continue;
      }
      memcpy(&temp1, &gen_children[k].binary_enc, sizeof(candidate));
      memcpy(&gen_children[k].binary_enc, &temp, sizeof(candidate));
      //if (k && check_duplicate(&gen_children[k], gen_children, pool_size)) {
      if (k && check_duplicate(&temp1, gen_children, pool_size)) {
         //memcpy(&gen_children[k].binary_enc, &temp, sizeof(candidate));
         k++;
         rand_elem = (double)rand() / (double)range;
         retry = 0;
         continue;
      }
      memset(&temp, 0, sizeof(candidate));
      memcpy(&gen_children[k].binary_enc, &temp1, sizeof(candidate));
      memset(&temp1, 0, sizeof(candidate));
      printf("\nMutation successfully done for candidate %d in the pool of children\n", k+1);
      k++;
      rand_elem = (double)rand() / (double)range;
      retry = 0;
   }
   printf("\nExiting mutation routine\n");
   return 0;
}



