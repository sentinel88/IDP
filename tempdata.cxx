#include <defs.h>
#include <data.h>
#include <funcs.h>

int init_net_data(network_data *netinfo) {
   printf("\nEntering init_net_data function\n");
   int i;

/* Allocate memory for all the data which needs to be stored */

   netinfo->Ca = (float **)(malloc( (N+1) * sizeof(float *) ));
   netinfo->Ba = (float **)(malloc( (N+1) * sizeof(float *) ));
   netinfo->Ta = (float **)(malloc( (N+1) * sizeof(float *) ));
#ifdef ZONES
   netinfo->Demand = (float **)(malloc( (ZONES+1) * sizeof(float *) ));
#else
   netinfo->Demand = (float **)(malloc( (N+1) * sizeof(float *) ));
#endif
   netinfo->ba = (float **)(malloc( (N+1) * sizeof(float *) ));
   netinfo->EdgeMatrix = (int **)(malloc( (N+1) * sizeof(int *) ));
   netinfo->existing_links = (link_set *)(malloc( EL * sizeof(link_set *) ));
   netinfo->new_links = (link_set *)(malloc( NL * sizeof(link_set *) ));

   for (i=0; i<=N; i++) {
      netinfo->Ca[i] = (float *)malloc( (N+1) * sizeof(float));
      netinfo->Ba[i] = (float *)malloc( (N+1) * sizeof(float));
      netinfo->Ta[i] = (float *)malloc( (N+1) * sizeof(float));
#ifdef ZONES
      netinfo->Demand[i] = (float *)malloc( (ZONES+1) * sizeof(float));
#else
      netinfo->Demand[i] = (float *)malloc( (N+1) * sizeof(float));
#endif
      netinfo->ba[i] = (float *)malloc( (N+1) * sizeof(float));
      netinfo->EdgeMatrix[i] = (int *)malloc( (N+1) * sizeof(int));
   }

/* Set all the memory locations to zero */
/*
   memset(netinfo->Ca, 0, (N+1) * (N+1) * sizeof(float));
   memset(netinfo->Ba, 0, (N+1) * (N+1) * sizeof(float));
   memset(netinfo->Ta, 0, (N+1) * (N+1) * sizeof(float));
   memset(netinfo->Demand, 0, (N+1) * (N+1) * sizeof(float));
   memset(netinfo->ba, 0, (N+1) * (N+1) * sizeof(float));
   memset(netinfo->EdgeMatrix, 0, (N+1) * (N+1) * sizeof(int));
   memset(netinfo->existing_links, 0, EL * sizeof(link_set));
   memset(netinfo->new_links, 0, NL * sizeof(link_set));
*/
   printf("\nExiting init_net_data function\n");

   return 0;
}

int cleanup_net_data(network_data *netinfo) {
   int i;
   printf("\nEntering cleanup_net_data function\n");

   for (i=0; i<=N; i++) {
      free(netinfo->Ca[i]);
      free(netinfo->Ba[i]);
      free(netinfo->Ta[i]);
#ifndef ZONES
      free(netinfo->Demand[i]);
#endif
      free(netinfo->ba[i]);
      free(netinfo->EdgeMatrix[i]);
   }

#ifdef ZONES
   for (i=0; i<=ZONES; i++) {
      free(netinfo->Demand[i]);
   }
#endif

   free(netinfo->Ca);
   free(netinfo->Ba);
   free(netinfo->Ta);
   free(netinfo->Demand);
   free(netinfo->ba);
   free(netinfo->EdgeMatrix);
   free(netinfo->existing_links);
   free(netinfo->new_links);

   printf("\nExiting cleanup_net_data function\n");

   return 0;
} 
