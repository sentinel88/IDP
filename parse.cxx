#include <defs.h>
#include <data.h>
#include <funcs.h>

int parse(network_data *netinfo) {
   FILE *datafile = NULL;
   char str[200], *temp;
   float value;
   int num, count = 0;
   int valcount = 0;
   int linecount = 0;
   int i, j, k;
   float ta, ba, ca, power, speed, toll, type, len;

   printf("\nInside parse function\n");

   datafile = fopen(FILENAME_NET, "r");   /*Open datafile for read access*/

   if (datafile == NULL) {
      printf("\nError in opening the file\n");
      return -1;
   }

   /***Reading from the data file***/
   cout<<"\nReading from data file started\n";

   count = 0;

   while (linecount < LC) {
    // while(fgets(str, 100, datafile)) {
      fgets(str, 200, datafile);
      if (cost_function_selector == 2) {
         if (sscanf(str, "%d %d %f %f %f", &i, &j, &ca, &ta, &ba) == 5) {
            netinfo->Ca[i][j] = ca; netinfo->Ba[i][j] = ba; netinfo->Ta[i][j] = ta;
            //printf("\n[%d %d]:%f %f %f\n", i, j, netinfo->Ca[i][j], netinfo->Ba[i][j], netinfo->Ta[i][j]);
            netinfo->existing_links[count].orig = i;
            netinfo->existing_links[count].term = j;
            netinfo->EdgeMatrix[i][j] = 1;
            count++;
         }
         /*if (sscanf(str, "%d %d %f %f %f %f %f %f %f %f", &i, &j, &ca, &len, &ta, &ba, &power, &speed, &toll, &type) == 10) {
            netinfo->Ca[i][j] = ca; netinfo->Ba[i][j] = ba; netinfo->Ta[i][j] = ta;
            //printf("\n[%d %d]:%f %f %f\n", i, j, netinfo->Ca[i][j], netinfo->Ba[i][j], netinfo->Ta[i][j]);
            netinfo->existing_links[count].orig = i;
            netinfo->existing_links[count].term = j;
            netinfo->EdgeMatrix[i][j] = 1;
            count++;
         }*/
      } else {
         if (sscanf(str, "%d %d %d", &i, &j, &k) == 3) {
            printf("\n%d %d %d\n", i, j, k);
            netinfo->Ta[i][j] = k;
            netinfo->existing_links[count].orig = i;
            netinfo->existing_links[count].term = j;
            netinfo->EdgeMatrix[i][j] = 1;
            count++;
         }
      }
      linecount++;
   }

   fclose(datafile);
   
   count = 0;
#ifdef ZONES
   valcount = 0;
#endif

   printf("\nPartial parsing finished\n");

   memset(str, 0, 100);
   datafile = fopen(FILENAME_TRIPS, "r");
   if (datafile == NULL) {
      printf("\nError in opening the trips file\n");
      return -1;
   }

   while (fgets(str, 100, datafile)) {
      temp = strtok(str, ";");
      while (temp != NULL) {
         if (sscanf(temp, "%d :%f", &num, &value) == 2) {
            netinfo->Demand[count + 1][num] = value;
            #ifdef ZONES
               valcount++;
               if (valcount == ZONES-1) {
                  netinfo->Demand[count+1][count+1] = 0;
                  count++;
                  valcount = 0;
               }
            #else
               if (num == N) {
                  count++;
               }
            #endif
         }
         temp = strtok(NULL, ";");
      }
   }

   fclose(datafile);

//#ifdef DONT_EXECUTE_NOW
   datafile = fopen(FILENAME_NEW_LINKS, "r");
   if (datafile == NULL) {
      printf("\nError in opening the new links file\n");
      return -1;
   }

   memset(str, 0, 100);

   count = 0;

   while (fgets(str, 100, datafile)) {
      if (cost_function_selector == 1) {
         if (sscanf(str, "%d %d %f %f", &i, &j, &ta, &ba) == 4) {
            printf("\n%d %d %f %f\n", i, j, ta, ba);
            netinfo->Ta[i][j] = ta;
            netinfo->ba[i][j] = ba;
            netinfo->new_links[count].orig = i;
            netinfo->new_links[count].term = j;
            netinfo->EdgeMatrix[i][j] = -1;
            count++;
         }
      } else {
         if (sscanf(str, "%d %d %f %f %f", &i, &j, &ta, &ba, &ca) == 5) {
            printf("\n%d %d %f %f %f\n", i, j, ta, ba, ca);
            netinfo->Ta[i][j] = ta;
            netinfo->ba[i][j] = ba;
            netinfo->Ca[i][j] = ca;
            netinfo->new_links[count].orig = i;
            netinfo->new_links[count].term = j;
            netinfo->EdgeMatrix[i][j] = -1;
            count++;
         }
      }
   }
#ifdef DONT_EXECUTE_NOW
   printf("\nPrinting network data\n");
   for (i=1; i<=N; i++) {
      printf("\n");
      for (j=1; j<=N; j++) {
         printf("[%d,%d]:%.2f %.2f\t", i, j, netinfo->Demand[i][j], netinfo->Ta[i][j]);
      }
   }
#endif
//#endif 
   printf("\nExiting parse function\n");
   return 0;
} 

int clean (network_data *netinfo) {
  int i;
  for (i=0; i<=N; i++) {
    free(netinfo->Ta[i]);
    free(netinfo->Demand[i]);
  }
 
  free(netinfo->Ta);
  free(netinfo->Demand);
  return 0;
}
