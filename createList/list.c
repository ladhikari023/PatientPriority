#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

extern log_t *inputhead;
extern log_t *longwait;
extern log_t *current;

int read_input(FILE **fi, FILE **fo, char *time, char *buf)
{
   char arrival[9];
   int age, pain;
   int n, id, ret = 0;
   char buffer[100];
   int h1, h2, m1, m2, s1, s2;
   log_t *ptr, *prev=NULL;
   log_t *tmp;

   strcpy(buf, "");
   while (1) {
      if (fgets(buffer, 100, *fi) == NULL) {
         break;
      }
      n = sscanf(buffer, "%d %s %d %d", &id, arrival, &age, &pain);
      if (n < 0) return 0;

      sscanf(time, "%d:%d:%d", &h2, &m2, &s2);
      sscanf(arrival, "%d:%d:%d", &h1, &m1, &s1);
      if ((h1 > h2) || (h1 == h2 && m1 > m2) ||
          (h1 == h2 && m1 == m2 && s1 > s2)) {
         strcpy (buf, buffer);
         break;
      }

      ptr = (log_t *) malloc(sizeof(log_t));
      if (current == NULL) current = ptr;
      else current->next = ptr;

      if (inputhead == NULL) inputhead = ptr;

      strcpy(ptr->arrival, arrival);
      ptr->id = id;
      ptr->pain_level = pain;
      ptr->age = age;
      ptr->next = NULL;
      current = ptr;
      if (ret == 0) ret = 1;
   }

   prev = NULL;
   if (ret != 2) {
      ptr = inputhead;
      while (ptr) {
         if (ptr->pain_level == 10  &&
             compare_time(time, ptr->arrival) > 0) {
            print_output(fo, ptr, time);
            if (ptr == inputhead) inputhead = inputhead->next;
            if (ptr == current) current = prev;
            if (prev) prev->next = ptr->next;
            free(ptr);
            return 2;
         }
         prev = ptr;
         ptr = ptr->next;
      }

      prev = NULL;
      ptr = inputhead;
      while (ptr) {
         get_waiting_time(time, ptr->arrival, &h1, &m1, &s1);
         if (h1 >= 2) {
            if (ptr == inputhead) inputhead = inputhead->next;
            else {
               if (ptr == current) current = prev;
               if (prev) 
                  prev->next = ptr->next;
            }

            if (longwait == NULL) {
               longwait = ptr;
               ptr = ptr->next;
               longwait->next = NULL;
            } else {
               tmp = ptr->next;
               ptr->next = longwait;
               longwait = ptr;
               ptr = tmp;
            }
            continue;
         } else {
            prev = ptr;
            ptr = ptr->next;
         }
      }
      if (longwait) {
         sort_time(longwait);
         ptr = longwait;
         longwait = ptr->next;
         print_output(fo, ptr, time);
         free(ptr);
         return 2;
      }
   }

   if (!ret) ret = 1;
   return ret;
}

int process(FILE **fi, FILE **fo)
{
   int waiting;
   char time[9];
   log_t *ptr;
   char buf[100];

   strcpy(time, "07:45:00");
   waiting = read_input(fi, fo, time, buf);
   while (1) {
      if (waiting == 1) {
         sort_list(inputhead, time);
         ptr = inputhead;
         inputhead = inputhead->next;
         print_output(fo, ptr, time);
      }
      if (strcmp(buf, "") != 0) push_list(buf);

      add_time(time, 15);
      waiting = read_input(fi, fo, time, buf);
      if (inputhead == NULL && longwait == NULL) break;
   }
   return 0;
}

int push_list(char *buffer)
{
   int id, age, pain;
   char arrival[9];
   int n;
   log_t *ptr;

   n = sscanf(buffer, "%d %s %d %d", &id, arrival, &age, &pain);
   if (n < 0) return 1;

   ptr = (log_t *) malloc(sizeof(log_t));
   if (current == NULL) current = ptr;
   else current->next = ptr;
   if (inputhead == NULL) inputhead = ptr;

   strcpy(ptr->arrival, arrival);
   ptr->id = id;
   ptr->pain_level = pain;
   ptr->age = age;
   ptr->next = NULL;
   current = ptr;

   return 0;
}

void sort_time(log_t *log)
{
   log_t *ptr1, *ptr2;

   ptr1 = log;
   while (ptr1) {
      ptr2 = ptr1->next;
      while (ptr2) {
         if (compare_time(ptr1->arrival, ptr2->arrival) > 0)
            swap(ptr1, ptr2);
         ptr2 = ptr2->next;
      }
      ptr1 = ptr1->next;
   }
}

void sort_list(log_t *log, char *time)
{
   log_t *ptr1, *ptr2;

   ptr1 = log;

   while (ptr1) {
      if (compare_time(ptr1->arrival, time) > 0) break;
      ptr2 = ptr1->next;
      while (ptr2) {
         if (compare_time(ptr2->arrival, time) > 0) break;

         if (ptr2->pain_level > ptr1->pain_level) {
            swap(ptr1, ptr2);
         } else if (ptr2->pain_level == ptr1->pain_level) {
            if (ptr2->age > ptr1->age) {
               swap(ptr1, ptr2);
            } else if (ptr2->age == ptr1->age) {
               if (compare_time(ptr1->arrival, ptr2->arrival) > 0) {
                  swap(ptr1, ptr2);
               }
            }
         }
         ptr2 = ptr2->next;
      }
      ptr1 = ptr1->next;
   }
}

int compare_time(char *time1, char *time2)
{
   int h1, h2, m1, m2, s1, s2;

   sscanf(time1, "%d:%d:%d", &h1, &m1, &s1);
   sscanf(time2, "%d:%d:%d", &h2, &m2, &s2);

   if (h1 > h2) return 1;
   if (h1 == h2) {
      if (m1 > m2) return 1;
      if (m1 == m2)
         if (s1 > s2) return 1;
   }
   return 0;
}

void swap(log_t *p1, log_t *p2)
{
   log_t tmp;

   strcpy(tmp.arrival, p2->arrival);
   tmp.id = p2->id;
   tmp.pain_level = p2->pain_level;
   tmp.age = p2->age;
   strcpy(tmp.timewait, p2->timewait);

   strcpy(p2->arrival, p1->arrival);
   p2->id = p1->id;
   p2->pain_level = p1->pain_level;
   p2->age = p1->age;
   strcpy(p2->timewait, p1->timewait);

   strcpy(p1->arrival, tmp.arrival);
   p1->id = tmp.id;
   p1->pain_level = tmp.pain_level;
   p1->age = tmp.age;
   strcpy(p1->timewait, tmp.timewait);
}

void add_time(char *time, int mins)
{
   int hour, min, sec;

   sscanf(time, "%d:%d:%d", &hour, &min, &sec);
   min += mins;
   if (min > 59) {
      min -= 60;
      hour++;
   }
   sprintf(time, "%02d:%02d:%02d", hour, min, sec);
}

void print_output(FILE **fo, log_t *list, char *time)
{
   int hour, min, sec;
   char buff1[200], buff2[100];

   get_waiting_time(time, list->arrival, &hour, &min, &sec);

   printf("%s Patient: ID = %d, Pain Level (1-10) = %d, Age = %d years, ",
          time, list->id, list->pain_level, list->age);
   printf("Time waiting = %02dh%02dm%02ds\n", hour, min, sec);

   sprintf(buff1, "%s Patient: ID = %d, Pain Level (1-10) = %d, ",
          time, list->id, list->pain_level);
   sprintf(buff2, "Age = %d years, Time waiting = %02dh%02dm%02ds\n",
           list->age, hour, min, sec);

   strcat(buff1, buff2);
   fputs(buff1, *fo);
}

int get_waiting_time(char *tm, char *arrv, int *hour, int *min, int *sec)
{
   int hour1, hour2, min1, min2, sec1, sec2;

   sscanf(arrv, "%d:%d:%d", &hour1, &min1, &sec1);
   sscanf(tm, "%d:%d:%d", &hour2, &min2, &sec2);

   if (sec2 >= sec1) sec2 -= sec1;
   else {
     sec2 += (60 - sec1);
     min2--;
     if (min2 < 0) {
        min2 += 60;
        hour2--;
     }
   }

   if (min2 >= min1) min2 -= min1;
   else {
      min2 += (60 - min1);
      hour2--;
   }
   hour2 -= hour1;

   *hour = hour2;
   *min = min2;
   *sec = sec2;

   return 0;
}

void release_mem()
{
   log_t *ptr;

   while (inputhead) {
      ptr = inputhead->next;
      free(inputhead);
      inputhead = ptr;
   }
}

