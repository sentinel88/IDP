#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LC 73

int main() {
 float Ca[N+1][N+1];            /* Capacity of link a */
 float Ba[N+1][N+1];            /* Congestion factor for link a */
 float Ta[N+1][N+1];            /* Free flow travel time on link a */
 int i, j, k;           /* Iteration variables */
 FILE *datafile;
 int linecount = 0;    /* Line count in files */


 memset(Ca, 0, sizeof(Ca));
 memset(Ba, 0, sizeof(Ba));
 memset(Ta, 0, sizeof(Ta));

 datafile = fopen("SiouxFalls_net.txt", "r");   /*Open datafile for read access*/


/***Reading from the data file***/
 cout<<"\nReading from data file started\n";
 while (linecount < LC) {
    XPRBreadlinecb(XPRB_FGETS, datafile, 99, "i i g g g", &i, &j, &Ca[i][j], &Ta[i][j], &Ba[i][j]);
    printf("%d %d %f %f %f\n", i, j, Ca[i][j], Ta[i][j], Ba[i][j]);
    linecount++;
 }
 fclose(datafile);

 return 0;
}

