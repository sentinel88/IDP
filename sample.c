#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#define NL 6

int main() {
 /*char *str = "1       2       25900.20064     6       0.15";
 char ch[5] = {'a', 'b', '5', 'd', 'e'};
 int i, j, l;
 float k, m;
 i = (ch[2] - 48) * 1;
 //sscanf(str, "%d %d %f %d %f", &i, &j, &k, &l, &m);
 //printf("\n%d %d %f %d %f\n", i, j ,k ,l, m);
 printf("\n%d, %d, %f\n", i, RAND_MAX, pow(2, 31));*/

/* int i=NL;
 srand(time(NULL));
 while(i--) {
    printf("%d ", 1 + (rand()%(NL)) );
 }*/

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
 int result;

 result = round(log2((double)RAND_MAX));

 printf("\n%d\n", result);
 

 return 0;
}
