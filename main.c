/* Laxman Adhikari
   CS 241L 

   This is the main program of project 8. I have included header.h in this program
   and written main function. Command Lines are read here and assigned as read and 
   write files. A function called process is called which is written in list.c.
*/ 
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
