/* Laxman Adhikari
   CS 241L 

   This is the header program of project8. I have defined an struct logstr with
   relevant datas in it. I have also defined all the functions that I am using in
   main.c and list.c.
*/ 
#ifndef HEADER_H
#define HEADER_H

typedef struct logstr {
   char arrival[9];
   int  id;
   int  pain_level;
   int  age;
   char timewait[9];
   struct logstr *next;
} log_t;

int read_input(FILE **fi, FILE **fo, char *time, char *buf);

int process(FILE **fi, FILE **fo);

void print_output(FILE **fo, log_t *list, char *time);

void add_time(char *time, int min);

void release_mem();

void sort_list(log_t *log, char *time);

void sort_time(log_t *log);

void swap(log_t *p1, log_t *p2);

int push_list(char *s);

int compare_time(char *time1, char *time2);

int get_waiting_time(char *time, char *arrival, int *hour, int *min, int *sec);

#endif

