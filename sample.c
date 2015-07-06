#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#define NL 6

int main(int argc, char *argv[]) {
 /*char *str = "1       2       25900.20064     6       0.15";
 char ch[5] = {'a', 'b', '5', 'd', 'e'};
 int i, j, l;
 float k, m;
 i = (ch[2] - 48) * 1;
 //sscanf(str, "%d %d %f %d %f", &i, &j, &k, &l, &m);
 //printf("\n%d %d %f %d %f\n", i, j ,k ,l, m);
 printf("\n%d, %d, %f\n", i, RAND_MAX, pow(2, 31));*/
 int min=1, max=NL;
 int i=NL;
 srand(time(NULL));
 /*while(i--) {
    //double scaled = (double)rand()/RAND_MAX;
    //printf("%d\n", (int)((double)rand() / ((double)RAND_MAX) * NL));
    printf("%lf\t", drand48());
    printf("%d\n", (int)(drand48() * NL));
//    printf("%d	\n", (max - min +1)*scaled + min);
//    printf("%d ", rand()%(NL) );
 }*/
 if (argc < 2) {
    printf("Invalid number of arguments\n");
    return 0;
 }
 i = atoi(argv[1]);
 int arr[6];
 memset(arr, 0, 6 * sizeof(int));
 unsigned int x = (RAND_MAX + 1u) / NL;
 unsigned int y = x * NL;
 unsigned int r;
 while(i--) {
	do {
		r = rand();
	} while(r >= y);
	printf("%d ", (r / x) + 1); 
        //r = rand() % NL;
        arr[r/x]++;        
        //arr[r]++;
 } 
 printf("\n\n");
 for (i=0; i<6; i++) {
    printf("%d\n", arr[i]);
 }
 //srand(time(NULL));
 //int n = 6;
/* Returns an integer in the range [0, n).
 *
 * Uses rand(), and so is affected-by/affects the same seed.
 */
    // Chop off all of the values that would cause skew...
 /*   long end = RAND_MAX / n; // truncate skew
    assert (end > 0L);
    end *= n;

    // ... and ignore results from rand() that fall above that limit.
    // (Worst case the loop condition should succeed 50% of the time,
    // so we can expect to bail out of this loop pretty quickly.)
    int r;
    while ((r = rand()) >= end); */


/*int limit = 6;

    int divisor = RAND_MAX/(limit+1);
    int retval;

    do { 
        retval = rand() / divisor;
    } while (retval > limit);

 
    printf("%d\n", retval);*/
 //int result;

 //result = round(log2((double)RAND_MAX));

 //printf("\n%d\n", result);
 

 return 0;
}
