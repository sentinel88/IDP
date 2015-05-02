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
 if (budget_sat > B) return -1;
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
    if (count == 5) break;
    }
    if (count == 5) break;
 }
 printf("\nExiting generate_rand\n");
 return 0;
}

int candidates_sort(candidate *ga_cand) {
   int i, j;
   candidate temp;
   
   memset(&temp, 0, sizeof(candidate));
   
   for (i=0; i<GA_POPULATION_SIZE; i++) {
      for (j=0; j<(GA_POPULATION_SIZE-i-1); j++) {
         if (ga_cand[j].fitness_value > ga_cand[j+1].fitness_value) {
            memcpy(&temp, &(ga_cand[j+1]), sizeof(candidate));
            memcpy(&(ga_cand[j+1]), &(ga_cand[j]), sizeof(candidate));
            memcpy(&(ga_cand[j]), &temp, sizeof(candidate));
         }
      }
   }
return 0;
}
     
int genetic_sp_crossover(genetic_algo *ga, candidate *gen_children) {
 int i, j=0, k=0, rand_elem;
 candidate temp;

 //gen_children = (candidate *)(malloc(sizeof(candidate)));
 memset(gen_children, 0, sizeof(gen_children));
 memset(&temp, 0, sizeof(candidate)); 

 while(1) {
    rand_elem = rand();
    i = rand_elem % NL;
    if (i == 0) {
       if ((rand_elem/NL) <= NL) { i = rand_elem/NL;  }
       else continue;
    }
    
   /* if (j > 1) {
       gen_children = (candidate *)(realloc(gen_children, k * sizeof(candidate)));
    }*/

    memcpy(&gen_children[k], &(ga->population[j].binary_enc), i);
    memcpy((char *)(&gen_children[k]) + i, (char *)(&ga->population[j+1]) + i, sizeof(temp) - i);

    j = j+2;
    k++; 
    
    if (j >= ga->population_size) break;
 }
 return k;
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
      if (CHOICE == 2) {
         summation += (netinfo->Ta[orig][term] * (1 + (netinfo->Ba[orig][term] * pow( (dndp->Xa[orig][term].getSol() / netinfo->Ca[orig][term]), 4))) );
      } else {
         //summation += (netinfo->Ta[orig][term]) * (dndp->Xa[orig][term].getSol());
         summation += (dndp->Xa[orig][term].getSol()) * (netinfo->Ta[orig][term] + (0.008 * pow(dndp->Xa[orig][term].getSol(), 4)));
      }
   } } 

   ga_cand->fitness_value = summation;
   printf("\nCandidate fitness: %f\n", summation);

   printf("\nExiting candidate fitness function\n");
}
