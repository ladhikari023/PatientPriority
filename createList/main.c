#include <stdio.h>
#include "header.h"

log_t *inputhead = NULL;
log_t *longwait = NULL;
log_t *current = NULL;

int main(int argc, char *argv[])
{
   FILE *fi, *fo;

   if (argc != 3) {
      printf("Usage: createList [inputfile] [outputfile]\n");
      return 1;
   }

   fi = fopen(argv[1], "r");
   fo = fopen(argv[2], "w");

   process(&fi, &fo);
   release_mem();

   fclose(fi);
   fclose(fo);
   return 0;
}
