#ifndef _funcs_h
#define _funcs_h

#include <data.h>

/***Function signatures ***/

int model_problem(model_data *, network_data *, genetic_algo *);

int remodel_problem(model_data *, network_data *, candidate);

int parse(network_data *);

int clean(network_data *);

int feasibility(candidate, network_data);

int candidates_sort(candidate *, int);

int generate_rand(genetic_algo *, network_data);

int genetic_sp_crossover(genetic_algo *, candidate *, network_data);

int genetic_mutation(candidate *, network_data, int);

int candidate_fitness(model_data *, network_data *, candidate *);

int print_candidate(candidate *);

int print_generation(candidate *, int, bool);

int encode_ga_cand(candidate *, int);

int count_set_bits(int value);

int compare(candidate);

int check_duplicate(candidate *, candidate *, int);

int elem_compare(candidate *, candidate *);

int cache_lookup(candidate *, candidate *, int *, int);

int init_net_data(network_data *);

int cleanup_net_data(network_data *);

#ifdef ROULETTE_WHEEL_SELECTION
int assign_selection_prob(candidate *, int);

int genetic_rw_crossover(genetic_algo *, candidate *, network_data);

int select_candidate(candidate *, int);
#endif

#endif
