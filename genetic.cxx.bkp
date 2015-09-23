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


int genetic_sp_crossover(genetic_algo *ga, candidate *gen_children, network_data *netinfo) {
 int i, j=0, k=0, l, rand_elem, bits;
 int intervals, tempval, value;
 int retry=0;
 double rand_val;
 candidate temp, temp1;
 int dup_parent_gen = 0;
 int dup_child_gen = 0;
 int retry_attempts = 0;
 int crossover_point = 0;

 printf("\nEntering crossover function\n");

 memset(&temp, 0, sizeof(candidate));

 if (NL) {
    intervals = (round((log2((double)RAND_MAX)))) / NL;
    tempval = pow(2, NL) - 1;
 }
 //srand(time(NULL));
 i = rand_elem = rand();
 l = intervals;
 rand_val = (double)rand() / (double)RAND_MAX;

 while(1) {
    if (j >= (ga->population_size-1)) break;
    if (rand_val > cross_prob) {
       rand_val = (double)rand() / (double)RAND_MAX;
       j++;
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
    //value = crossover_point;
    printf("\nCrossover point is %d\n", value);

/* Following memory copy operations do not reference the binary_enc member inside structure candidate for every gen_children because it is 
   just a shortcut since the first member in the candidate data structure is binary_enc and the memcpy will work correctly without specfiying
   the binary_enc */
//    memcpy(&gen_children[k], &(ga->population[j].binary_enc), i);
    memcpy(&gen_children[k].binary_enc, &(ga->population[j].binary_enc), value);
//    memcpy((char *)(&gen_children[k]) + i, (char *)(&ga->population[j+1]) + i, sizeof(temp) - i);
    memcpy((char *)(&gen_children[k]) + value, (char *)(&ga->population[j+1]) + value, sizeof(temp) - value);

    if (check_if_zero(gen_children[k])) {
       memcpy(&gen_children[k], &(ga->population[j+1].binary_enc), value);
       memcpy((char *)(&gen_children[k]) + value, (char *)(&ga->population[j]) + value, sizeof(temp) - value);
 }

    if (check_if_zero(gen_children[k])) {
       if (retry == MAX_RETRY) {
          printf("\nMade 5 attempts to avoid generating a zero child but now we will not perform any more crossover attempts for this pair \
                    of candidates and move forward with the next consecutive pair\n");
          retry = 0;
          retry_attempts = 0;
          j++;
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
          rand_val = (double)rand() / (double)RAND_MAX;
          retry_attempts = 0;
          retry = 0;
          j++;
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
                    pair and continue with the next consecutive pair\n");
          memset(&gen_children[k], 0, sizeof(candidate));
          rand_val = (double)rand() / (double)RAND_MAX;
          retry_attempts = 0;
          retry = 0;
          j++;
          continue;
       } else {
          printf("\nThis crossover operation has generated a child that is a duplicate of one of the candidates in the parent population. hence \
                 we need to repeat this crossover operation for this pair of candidates with a different crossover point.\n");
          memset(&gen_children[k], 0, sizeof(candidate));
          retry_attempts++;
          continue;
       }
    }

    //if (k && check_duplicate(&gen_children[k], gen_children, k)) {
    if (k && dup_child_gen) {
       printf("\nThis child generated from a crossover operation is already present in this population of children. We do not consider it \
                 and continue with no more attempts\n");
       memset(&gen_children[k], 0, sizeof(candidate));
       rand_val = (double)rand() / (double)RAND_MAX;
       retry = 0;
       retry_attempts = 0;
       j++;
       continue;
    }

    retry = 0;
    retry_attempts = 0;
    j = j+2;
    k++;
rand_val = (double)rand() / (double)RAND_MAX;
    if (j >= (ga->population_size-1)) break;
 }
 printf("\nExiting crossover function\n");
 return k;
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



