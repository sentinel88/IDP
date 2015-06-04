#include <defs.h>
#include <data.h>
#include <funcs.h>

int feasibility(candidate gen_cand, network_data netinfo) {
 float budget_sat = 0.0;
 int i = 0, orig, term;
 int bin_val;
 printf("\nEntering feasibility\n");
 while (i<NL) {
    orig = netinfo.new_links[i].orig;
    term = netinfo.new_links[i].term;
    bin_val = gen_cand.binary_enc[i];
    printf("%d, orig: %d, term: %d\n ", gen_cand.binary_enc[i], orig, term);
    //budget_sat += ( (gen_cand.binary_enc[i] - 48) * netinfo.ba[orig][term]);
    budget_sat += ( bin_val * netinfo.ba[orig][term]);
    i++;
 }
 printf("\nBudget = %f\n", budget_sat);
 if (budget_sat > Budget) return -1;
 printf("\nExiting feasiblity\n");
 return 0;
}

int encode_ga_cand(candidate *ga_cand, int value) {
 int i = NL, k = 0;
 printf("\nEntering encode_ga_cand\n");
 while(i--) {
    ga_cand->binary_enc[k] = value & 1;
    value = value >> 1;
    printf("%d ", ga_cand->binary_enc[k]);
    k++;
 }
 printf("\nExisting encode_ga_cand\n");
 return 0;
}

int generate_rand(genetic_algo *ga, network_data netinfo) {
 int i, j, count, k, rand_elem, intervals;
 int value, temp;
 printf("\nEntering generate_rand\n");
 count = 0;
 k = 0;
 intervals = (round((log2((double)RAND_MAX))))/NL;
 temp = round((pow(2, NL) - 1));
 srand(time(NULL));

 while(1) {
    k = 0;
    //if (rand_elem == 0) { rand_elem = rand(); }
    /*rand_elem = rand();
    i = (rand_elem % NL);
    printf("\n%d\t", i);
    //i = ga.population[count].binary_enc = (rand() % NL);
    if (i == 0) {
       if ( ((rand_elem/NL) <= NL) ) { i = rand_elem/NL;  }
       else continue;
    }
    printf("%d\t", i);
    while(i) {*/
    j = intervals;
    rand_elem = rand();
    //while(i--) {
    while(j--) {
       //ga->population[count].binary_enc[k++] = i&1;
       //ga->population[count].binary_enc[k++] = rand_elem & 1;
       //i = i>>1;
       //rand_elem = rand_elem >> 1;
       value = rand_elem & temp;
       if (value == 0) continue;
       encode_ga_cand(&(ga->population[count]), value);
       rand_elem = rand_elem >> NL;
       //printf("%d ", ga->population[count].binary_enc[k-1]);
    

    if (feasibility(ga->population[count], netinfo) != 0) {
       memset(&(ga->population[count].binary_enc), 0, NL);
       printf("\nNot feasible\n");
       continue;
    } else { count++; }
    //printf("\n%d\n", ga.population[count].binary_enc);
    //count++;
    if (count == GA_POPULATION_SIZE) break;
    }
    if (count == GA_POPULATION_SIZE) break;
 }
 printf("\nExiting generate_rand\n");
 return 0;
}

int candidates_sort(candidate *ga_cand, int size) {
   int i, j;
   candidate temp;
   
   memset(&temp, 0, sizeof(candidate));
   
   for (i=0; i<size; i++) {
      for (j=0; j<(size-i-1); j++) {
         if (ga_cand[j].fitness_value > ga_cand[j+1].fitness_value) {
            memcpy(&temp, &(ga_cand[j+1]), sizeof(candidate));
            memcpy(&(ga_cand[j+1]), &(ga_cand[j]), sizeof(candidate));
            memcpy(&(ga_cand[j]), &temp, sizeof(candidate));
         }
      }
   }
   return 0;
}

int genetic_sp_crossover(genetic_algo *ga, candidate *gen_children, network_data netinfo) {
 int i, j=0, k=0, l, rand_elem, bits;
 int intervals, tempval, srand, value;
 int retry=0; 
 double rand_val;
 candidate temp;

 printf("\nEntering crossover function\n");

 //gen_children = (candidate *)(malloc(sizeof(candidate)));
 //memset(gen_children, 0, sizeof(gen_children));
 memset(&temp, 0, sizeof(candidate)); 

 //bits = ceil(log2((double)NL));
 //intervals = (round((log2((double)RAND_MAX)))) / bits;
 intervals = (round((log2((double)RAND_MAX)))) / NL;
 tempval = pow(2, NL) - 1;
 //tempval = !(1<<(NL-1)) + 1;
 //srand(time(NULL));
 i = rand_elem = rand();
 //i = rand_elem;
 l = intervals; 
 rand_val = (double)rand() / (double)RAND_MAX;

 while(1) {
    //rand_elem = rand();
    //i = rand_elem % NL;
    /*if (i == 0) {
       if ((rand_elem/NL) <= NL) { i = rand_elem/NL;  }
       else continue;
    }*/
    
   /* if (j > 1) {
       gen_children = (candidate *)(realloc(gen_children, k * sizeof(candidate)));
    }*/
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

/* Following memory copy operations do not reference the binary_enc member inside structure candidate for every gen_children because it is 
   just a shortcut since the first member in the candidate data structure is binary_enc and the memcpy will work correctly without specfiying
   the binary_enc */
//    memcpy(&gen_children[k], &(ga->population[j].binary_enc), i);
    memcpy(&gen_children[k].binary_enc, &(ga->population[j].binary_enc), value);
//    memcpy((char *)(&gen_children[k]) + i, (char *)(&ga->population[j+1]) + i, sizeof(temp) - i);
    memcpy((char *)(&gen_children[k]) + value, (char *)(&ga->population[j+1]) + value, sizeof(temp) - value);

    if (compare(gen_children[k])) {
       memcpy(&gen_children[k], &(ga->population[j+1].binary_enc), value);
       memcpy((char *)(&gen_children[k]) + value, (char *)(&ga->population[j]) + value, sizeof(temp) - value);
    } 
   
    if (compare(gen_children[k])) {
       if (retry == 5) {
          retry = 0;
          j++;
          continue;
       }
       retry++;
       continue;
    }

    if (feasibility(gen_children[k], netinfo) != 0) {
       memset(&gen_children[k], 0, sizeof(candidate));
       continue;
    }

    retry = 0;
    j = j+2;
    k++; 
    
    if (j >= (ga->population_size-1)) break;
 }
 printf("\nExiting crossover function\n");
 return k;
}

int genetic_mutation(candidate *gen_children, network_data netinfo, int pool_size) {
   printf("\nEntering mutation routine\n");
   candidate temp;
   double rand_elem;
   int pos_mutate;
   int range = RAND_MAX;
   int k = 0;
   int retry = 0;
   int i,j,l;
   int tempval, intervals, value;
   intervals = (round((log2((double)RAND_MAX)))) / NL;
   tempval = pow(2, NL) - 1;
   srand(time(NULL));
   i = rand();
   l = intervals;
   rand_elem = (double)rand() / (double)range;
   while(1) {
      if (k == pool_size) break;
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
         if (retry == 5) { retry = 0; k++; continue; }
         retry++;
         continue;
      }
      pos_mutate = count_set_bits(value);
      memcpy(&temp, &gen_children[k], sizeof(candidate));
      gen_children[k].binary_enc[pos_mutate] = (gen_children[k].binary_enc[pos_mutate] + 1)%2; 
      if (feasibility(gen_children[k], netinfo) != 0) {
         memcpy(&gen_children[k].binary_enc, &temp, sizeof(candidate));
         if (retry == 5) { retry = 0; k++; continue; }
         retry++;
         continue;
      }
      memset(&temp, 0, sizeof(candidate));
      printf("\nMutation successfully done for candidate %d in the pool of children\n", k+1);
      k++;
      rand_elem = (double)rand() / (double)range;
      retry = 0;
   }
   printf("\nExiting mutation routine\n");
   return 0;
}

int candidate_fitness(model_data *dndp, network_data *netinfo, candidate *ga_cand) {
   float summation = 0.0;
   int i = 0, orig, term;
  
   printf("\nEntering candidate fitness function\n");

   for (i=0; i<EL; i++) {
      orig = netinfo->existing_links[i].orig;
      term = netinfo->existing_links[i].term;
      if (CHOICE == 2) {
         summation += (netinfo->Ta[orig][term] * (1 + (netinfo->Ba[orig][term] * pow( (dndp->Xa[orig][term].getSol() / netinfo->Ca[orig][term]), 4))) );
      } else {
         //summation += (netinfo->Ta[orig][term]) * (dndp->Xa[orig][term].getSol());
         summation += (dndp->Xa[orig][term].getSol()) * (netinfo->Ta[orig][term] + (0.008 * pow(dndp->Xa[orig][term].getSol(), 4)));
      }
   }

   for (i=0; i<NL; i++) {
   if (ga_cand->binary_enc[i]) {
      orig = netinfo->new_links[i].orig;
      term = netinfo->new_links[i].term;
#ifdef _CODE   
   if (CHOICE == 2) {
         summation += (netinfo->Ta[orig][term] * (1 + (0.15 * pow( (dndp->Xa[orig][term].getSol() / netinfo->Ca[orig][term]), 4))) );
      } else {
#endif
         //summation += (netinfo->Ta[orig][term]) * (dndp->Xa[orig][term].getSol());
         summation += (dndp->Xa[orig][term].getSol()) * (netinfo->Ta[orig][term] + (0.008 * pow(dndp->Xa[orig][term].getSol(), 4)));
     // }
   } } 

   ga_cand->fitness_value = summation;
   printf("\nCandidate fitness: %f\n", summation);

   printf("\nExiting candidate fitness function\n");
}

int print_candidate(candidate *ga_cand) {
   int i;
   for (i=0; i<NL; i++) {
      printf("%d", ga_cand->binary_enc[i]);
   }
   return 0;
}

int count_set_bits(int value) {
   int count=0;
   while(value) {
      if (value & 1) count++;
      value = value>>1;
   }
   return count;
}

int compare(candidate gen_child) {
   int i=0, k=0;
   while (i<NL) {
      if (gen_child.binary_enc[k++] & 1) return 0;
      i++;
   }
   return 1;
}
