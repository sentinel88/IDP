#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <string.h>
#include "xprb_cpp.h"
#define N 5

int main(int argc, char **argv) {
   
   int a[N] = {1, 2, 3, 4, 5};
   int i, j, k;
   FILE *datafile = fopen("IDP_Datasets/example_net.txt", "r");
   XPRBreadlinecb(XPRB_FGETS, datafile, 99, "i i i", &i, &j, &k);
   printf("\n%d %d %d\n", i, j, k);
   a[2] = 5;
   printf("\nValue = %d\n", a[2]);
   return 0;
}
