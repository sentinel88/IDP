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

int model_data::init_model_data() {
   printf("\nEntering init_model_data function\n");
   /*int i, j;*/

/* Allocate memory for all the data which needs to be stored */

   //x = (XPRBvar ***)(malloc( (N+1) * sizeof(XPRB *) ));
  /* x = new XPRBvar***[N+1];
   //Xa = (XPRBvar **)(malloc( (N+1) * sizeof(XPRB *) ));
   Xa = new XPRBvar**[N+1];
   //Fa_rs = (XPRBvar ***)(malloc( (OD+1) * sizeof(XPRB *) ));
   Fa_rs = new XPRBvar***[OD+1];
   //Ya = (XPRBvar **)(malloc( (N+1) * sizeof(XPRB *) ));
   Ya = new XPRBvar**[N+1];

   for (i=0; i<=N; i++) {
      //x[i] = (XPRBvar **)malloc( (N+1) * sizeof(XPRB *));
      x[i] = new XPRBvar**[N+1];
      //Xa[i] = (XPRBvar *)malloc( (N+1) * sizeof(XPRB *));
      Xa[i] = new XPRBvar*[N+1];
      //Fa_rs[i] = (XPRBvar **)malloc( (N+1) * sizeof(XPRB *));
      Fa_rs[i] = new XPRBvar**[N+1];
      //Ya[i] = (XPRBvar *)malloc( (N+1) * sizeof(XPRB *));
      Ya[i] = new XPRBvar*[N+1];
   }

   for (i=0; i<=N; i++) {
      for (j=0; j<=M; j++) {
         //x[i][j] = (XPRBvar *)(malloc( (N+1) * sizeof(XPRB *) ));
         x[i][j] = new XPRBvar*[M+1];
      }
      for (j=0; j<=N; j++) {
         //Fa_rs[i][j] = (XPRBvar *)(malloc( (N+1) * sizeof(XPRB *) ));
         Fa_rs[i][j] = new XPRBvar*[N+1];
      }
   }*/

/* Set all the memory locations to zero */
/*
   memset(x, 0, (N+1) * (N+1) * (M+1) * sizeof(XPRBvar *));
   memset(Xa, 0, (N+1) * (N+1) * sizeof(XPRBvar *));
   memset(Fa_rs, 0, (OD+1) * (N+1) * (N+1) * sizeof(XPRBvar *));
   memset(Ya, 0, (N+1) * (N+1) * sizeof(XPRB *));
*/
   printf("\nExiting init_model_data function\n");

   return 0;
}

int model_data::cleanup_model_data() {
 /*  int i, j;
   printf("\nEntering cleanup_net_data function\n");

   for (i=0; i<=N; i++) {
      for (j=0; j<=M; j++) {
         //free(x[i][j]);
         delete[] x[i][j];
      }
      for (j=0; j<=N; j++) {
         //free(Fa_rs[i][j]);
         delete[] Fa_rs[i][j];
      }
   } 

   for (i=0; i<=N; i++) {
      //free(x[i]);
      delete[] x[i];
      //free(Xa[i]);
      delete[] Xa[i];
      //free(Fa_rs[i]);
      delete[] Fa_rs[i];
      //free(Ya[i]);
      delete[] Ya[i];
   }

   //free(x);
   delete[] x;
   //free(Xa);
   delete[] Xa;
   //free(Fa_rs);
   delete[] Fa_rs;
   //free(Ya);
   delete[] Ya;
*/
   printf("\nExiting cleanup_model_data function\n");
   return 0;
} 
