#include <defs.h>
#include <data.h>
#include <funcs.h>

#ifdef ROULETTE_WHEEL_SELECTION
int assign_selection_prob(candidate *ga_cand, int size) {
   int i;
   for (i=0; i<size; i++) {
      ga_cand[i].selection_prob = ga_cand[i].fitness_value/total_fitness;
   }
   return 0;
}

int select_candidate(candidate *ga_cand, int size) {
   int i;
   float sum = 0.0;
   double rand_elem = (double)rand()/(double)RAND_MAX;
   for (i=0; i<size; i++) {
      sum += ga_cand[i].selection_prob;
      if (sum >= rand_elem)
         return i;
   }
   return -1;         
}   
int genetic_rw_crossover(genetic_algo *ga, candidate *gen_children, network_data netinfo) {
 int i, j=0, k=0, l, rand_elem, bits;
 int intervals, tempval, srand, value;
 int retry=0;
 double rand_val;
 candidate temp, temp1;
 int dup_parent_gen = 0;
 int dup_child_gen = 0;
 int retry_attempts = 0;
 int crossover_point = 0;
 int parent1 = -1;
 int parent2 = -1;

 printf("\nEntering crossover function\n");

 memset(&temp, 0, sizeof(candidate));

 if (NL)
 intervals = (round((log2((double)RAND_MAX)))) / NL;
 tempval = pow(2, NL) - 1;

 i = rand_elem = rand();
 l = intervals;
 rand_val = (double)rand() / (double)RAND_MAX;

 while(1) {
    crossover_point = (crossover_point + 1) % NL;
    crossover_point = (crossover_point == 0) ? 1: crossover_point;
    //if (j >= (ga->population_size-1)) break;
    if (k >= (ga->population_size-1)) break;
    if (rand_val > cross_prob) {
       rand_val = (double)rand() / (double)RAND_MAX;
       //j++;
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

    value = count_set_bits(value);
    value = crossover_point;
    printf("\nCrossover point is %d\n", value);

/* Select a pair of candidates as parents for a potential crossover operation to generate a feasible child */
   parent1 = select_candidate(ga->population, GA_POPULATION_SIZE);
   parent2 = select_candidate(ga->population, GA_POPULATION_SIZE);

   printf("\nSelected parents are candidates %d and %d\n", parent1, parent2);

/* Following memory copy operations do not reference the binary_enc member inside structure candidate for every gen_children because it is 
   just a shortcut since the first member in the candidate data structure is binary_enc and the memcpy will work correctly without specfiying
   the binary_enc */
//    memcpy(&gen_children[k], &(ga->population[j].binary_enc), i);
    memcpy(&gen_children[k].binary_enc, &(ga->population[parent1].binary_enc), value);
//    memcpy((char *)(&gen_children[k]) + i, (char *)(&ga->population[j+1]) + i, sizeof(temp) - i);
    memcpy((char *)(&gen_children[k]) + value, (char *)(&ga->population[parent2]) + value, sizeof(temp) - value);

    if (compare(gen_children[k])) {
       memcpy(&gen_children[k], &(ga->population[parent2].binary_enc), value);
       memcpy((char *)(&gen_children[k]) + value, (char *)(&ga->population[parent1]) + value, sizeof(temp) - value);
    }

    if (compare(gen_children[k])) {
       if (retry == 5) {
          printf("\nMade 5 attempts to avoid generating a zero child but now we will not perform any more crossover attempts for the next \
                    offspring and move forward with copying the fitter of the two parents as the next offspring\n");
          if (ga->population[parent1].fitness_value < ga->population[parent2].fitness_value) {
             memcpy(&gen_children[k], &(ga->population[parent1].binary_enc), value);
          } else {
             memcpy(&gen_children[k], &(ga->population[parent2].binary_enc), value);
          }
          retry = 0;
          retry_attempts = 0;
          k++;
          rand_val = (double)rand() / (double)RAND_MAX;
          continue;
       }
       retry++;
       continue;
    }

    if (feasibility(gen_children[k], netinfo)) {  //Possibly consider to also increment retry here to avoid infinite attempts
       printf("\nChild from crossover not feasible\n");
       memset(&gen_children[k], 0, sizeof(candidate));
       if (retry_attempts == MAX_CROSSOVER_ATTEMPTS) {
          printf("\nEnough attempts have been made to do a crossover operation for this pair but all crossovers have resulted in infeasible \
                    children\n");
          if (ga->population[parent1].fitness_value < ga->population[parent2].fitness_value) {
             memcpy(&gen_children[k], &(ga->population[parent1].binary_enc), value);
          } else {
             memcpy(&gen_children[k], &(ga->population[parent2].binary_enc), value);
          }
          rand_val = (double)rand() / (double)RAND_MAX;
          retry_attempts = 0;
          retry = 0;
          k++;
          continue;
       }
       retry_attempts++;
       continue;
    }

    dup_parent_gen = check_duplicate(&gen_children[k], ga->population, GA_POPULATION_SIZE);

    if (k) { dup_child_gen = check_duplicate(&gen_children[k], gen_children, k); }

    if (dup_parent_gen) {
       if (retry_attempts == MAX_CROSSOVER_ATTEMPTS) {
          //printf("\nAlready tried enough attempts at generating a unique child from crossover but now we will consider this even though \
                    its a duplicate\n");
          printf("\nAlready tried enough attempts at generating a unique child from crossover so we will not consider the crossover for this \
                    pair and use the fitter of the two parents as the next offspring\n");
          if (ga->population[parent1].fitness_value < ga->population[parent2].fitness_value) {
             memcpy(&gen_children[k], &(ga->population[parent1].binary_enc), value);
          } else {
             memcpy(&gen_children[k], &(ga->population[parent2].binary_enc), value);
          }
          memset(&gen_children[k], 0, sizeof(candidate));
          rand_val = (double)rand() / (double)RAND_MAX;
          retry_attempts = 0;
          retry = 0;
          k++;
          continue;
       } else {
          printf("\nThis crossover operation has generated a child that is a duplicate of one of the candidates in the parent population. hence \
                 we need to repeat this crossover operation for this pair of candidates with a different crossover point.\n");
          memset(&gen_children[k], 0, sizeof(candidate));
          retry_attempts++;
          continue;
       }
    }

    if (k && dup_child_gen) {
       printf("\nThis child generated from a crossover operation is already present in this population of children. We do not consider it \
                 and continue with no more attempts\n");
          if (ga->population[parent1].fitness_value < ga->population[parent2].fitness_value) {
             memcpy(&gen_children[k], &(ga->population[parent1].binary_enc), value);
          } else {
             memcpy(&gen_children[k], &(ga->population[parent2].binary_enc), value);
          }
       memset(&gen_children[k], 0, sizeof(candidate));
       rand_val = (double)rand() / (double)RAND_MAX;
       retry = 0;
       retry_attempts = 0;
       k++;
       continue;
    }

    retry = 0;
    retry_attempts = 0;
    //j = j+2;
    k++;

    rand_val = (double)rand() / (double)RAND_MAX;
    if (k >= (ga->population_size-1)) break;
 }
 printf("\nExiting crossover function\n");
 return k;
}
#endif
