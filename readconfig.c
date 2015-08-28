#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
   printf("\nEnter main\n");
   FILE *datafile = NULL;
   char str[200];
   char *substr = "filename";
   char *temp, *temp1;
   char *filename_net = "./tempconfig.txt";
   datafile = fopen(filename_net, "r"); 
   
   memset(str, 0, 200);
   while (fgets(str, 200, datafile)) {
      temp = strtok(str, "=");
      temp1 = strtok(NULL, "\n");
      if (strstr(str, substr) != NULL)
         printf("%s=%s\n", temp, temp1);
      else
	 printf("%s=%d\n", temp, atoi(temp1));
      //printf("%s\t%s\n", temp, temp1);
   }
   printf("\nExiting main\n");  
   close(datafile);
   return 0;
}
