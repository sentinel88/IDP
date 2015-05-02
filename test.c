#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include <stdlib.h>
#include <time.h>
int main()
{

 int count = 0;

#ifdef CODE
 FILE *datafile = NULL;
 char str[100], *temp;
 int ret;
 char *str1, *str2, *str3;
 float value;
 int num, count = 0;

 memset(str, 0, 100);
 datafile = fopen("IDP_Datasets/SiouxFalls_trips.txt", "r");
 
 while (fgets(str, 100, datafile) ) {
    printf("\nInput string: %s\n", str);
    temp = strtok(str, ";");
    while (temp != NULL) {
       printf("%s", temp);
       if (sscanf(temp, "%d :%f", &num, &value) == 2) {
          printf(" [%f] ", value);
          if (num == 24) count++;
       }
       temp = strtok(NULL, ";");
    }
 }
 printf("\n%d\n", count);
 fclose(datafile);
#endif

while(1) {
 int num;
 //srand(time(NULL));
 num = rand();
 printf("\n%d\n", num % 144);
 if (count++ == 25) break;
}

 printf("\nRAND_MAX=%d\n", RAND_MAX);

 return 0;
} 
