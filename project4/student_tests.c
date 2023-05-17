#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "event.h"
#include "calendar.h"
#include "my_memory_checker_216.h"

/*****************************************************/
/* In this file you will provide tests for your      */
/* calendar application.  Each test should be named  */
/* test1(), test2(), etc. Each test must have a      */
/* brief description of what it is testing (this     */
/* description is important).                        */
/*                                                   */
/* You can tell whether any test failed if after     */
/* executing the students tests, you executed        */
/* "echo $?" on the command line and you get a value */
/* other than 0.  "echo $?" prints the status of     */
/* the last command executed by the shell.           */ 
/*                                                   */
/* Notice that main just calls test1(), test2(), etc.*/
/* and once one fails, the program eventually        */
/* return EXIT_FAILURE; otherwise EXIT_SUCCESS will  */
/* be returned.                                      */
/*****************************************************/

static int comp_minutes(const void *ptr1, const void *ptr2) {
   return ((Event *)ptr1)->duration_minutes - ((Event *)ptr2)->duration_minutes;
}

/* This test checks init_calendar, print_calendar
and destroy_calendar with basic functionality. */
static int test1() {
   int days = 7;
   Calendar *calendar;

   if (init_calendar("Spr", days, comp_minutes, NULL, &calendar) == SUCCESS) {
      if (print_calendar(calendar, stdout, 1) == SUCCESS) {
         return destroy_calendar(calendar);
      }
   }
    
   return FAILURE;
}

/* Check init_calendar with an invalid number of days. */
static int test2() {
   Calendar *calendar;
   return init_calendar("Cal", 0, comp_minutes, NULL, &calendar) == FAILURE;
}

/* Check add_event with basic functionality. */
static int test3() {
   Calendar *calendar;
   if (init_calendar("Cal", 10, comp_minutes, NULL, &calendar) == SUCCESS) {
      if (add_event(calendar, "lunch", 1200, 60, NULL, 10) == SUCCESS) {
         return destroy_calendar(calendar);
      }
   }
   return FAILURE;
}

/* Check adding the same event twice. */
static int test4() {
   Calendar *calendar;
   if (init_calendar("Cal", 10, comp_minutes, NULL, &calendar) == SUCCESS) {
      if (add_event(calendar, "lunch", 1200, 60, NULL, 10) == SUCCESS) {
         if (add_event(calendar, "lunch", 900, 20, NULL, 9) == FAILURE) {
            return destroy_calendar(calendar);
         }
      }
   }
   return FAILURE;
}

/* Check adding multiple distinct events. */
static int test5() {
   Calendar *calendar;
   if (init_calendar("Cal", 10, comp_minutes, NULL, &calendar) == SUCCESS) {
      if (add_event(calendar, "lunch", 1200, 60, NULL, 10) == SUCCESS) {
         if (add_event(calendar, "breakfast", 900, 20, NULL, 9) == SUCCESS) {
            return destroy_calendar(calendar);
         }
      }
   }
   return FAILURE;
}

/* Check finding an event that exists. */
static int test6() {
   Calendar *calendar;
   if (init_calendar("Cal", 10, comp_minutes, NULL, &calendar) == SUCCESS) {
      if (add_event(calendar, "lunch", 1200, 60, NULL, 10) == SUCCESS) {
         if (find_event(calendar, "lunch", NULL) == SUCCESS) {
            return destroy_calendar(calendar);
         }
      }
   }
   return FAILURE;
}

/* Check finding an event that doesn't exist. */
static int test7() {
   Calendar *calendar;
   if (init_calendar("Cal", 10, comp_minutes, NULL, &calendar) == SUCCESS) {
      if (add_event(calendar, "lunch", 1200, 60, NULL, 10) == SUCCESS) {
         if (find_event(calendar, "breakfast", NULL) == FAILURE) {
            return destroy_calendar(calendar);
         }
      }
   }
   return FAILURE;
}

/* Check finding an event that exists on a day. */
static int test8() {
   Calendar *calendar;
   if (init_calendar("Cal", 10, comp_minutes, NULL, &calendar) == SUCCESS) {
      if (add_event(calendar, "lunch", 1200, 60, NULL, 10) == SUCCESS) {
         if (find_event_in_day(calendar, "lunch", 10, NULL) == SUCCESS) {
            return destroy_calendar(calendar);
         }
      }
   }
   return FAILURE;
}

/* Check finding an event that doesn't exist on a day. */
static int test9() {
   Calendar *calendar;
   if (init_calendar("Cal", 10, comp_minutes, NULL, &calendar) == SUCCESS) {
      if (add_event(calendar, "lunch", 1200, 60, NULL, 10) == SUCCESS) {
         if (find_event_in_day(calendar, "lunch", 9, NULL) == FAILURE) {
            return destroy_calendar(calendar);
         }
      }
   }
   return FAILURE;
}

/* Check removing an event that exists. */
static int test10() {
   Calendar *calendar;
   if (init_calendar("Cal", 10, comp_minutes, NULL, &calendar) == SUCCESS) {
      if (add_event(calendar, "lunch", 1200, 60, NULL, 10) == SUCCESS) {
         if (remove_event(calendar, "lunch") == SUCCESS) {
            return destroy_calendar(calendar);
         }
      }
   }
   return FAILURE;
}

/* Check removing an event that doesn't exist. */
static int test11() {
   Calendar *calendar;
   if (init_calendar("Cal", 10, comp_minutes, NULL, &calendar) == SUCCESS) {
      if (add_event(calendar, "lunch", 1200, 60, NULL, 10) == SUCCESS) {
         if (remove_event(calendar, "breakfast") == FAILURE) {
            return destroy_calendar(calendar);
         }
      }
   }
   return FAILURE;
}

typedef struct task_info {
  double cost;
  char *prog_language;
} Task_info;

static int comp_name(const void *ptr1, const void *ptr2) {
   return strcmp(((Event *)ptr1)->name, ((Event *)ptr2)->name);
}

static Task_info *create_task_info(double cost, const char *prog_language) {
   Task_info *task_info = malloc(sizeof(Task_info));

   if (task_info) {
      task_info->prog_language = malloc(strlen(prog_language) + 1);
      if (task_info->prog_language) {
         task_info->cost = cost;
         strcpy(task_info->prog_language, prog_language);
         return task_info;
      }
   }

   return NULL;
}

static void free_task_info(void *ptr) {
   Task_info *task_info = (Task_info *)ptr;

   free(task_info->prog_language);
   free(task_info);
}

/* Check get event info and destroy with free_info func. */
static int test12() {
   Calendar *calendar;
   Task_info *info;
   if (init_calendar("Cal", 10, comp_minutes, free_task_info, &calendar) == SUCCESS) {
      info = create_task_info(35000, "Java");
      if (add_event(calendar, "lunch", 1200, 60, info, 10) == SUCCESS) {
         if (get_event_info(calendar, "lunch")) {
            return destroy_calendar(calendar);
         }
      }
   }
   return FAILURE;
}

int main() {
   int result = SUCCESS;

   /***** Starting memory checking *****/
   start_memory_check();
   /***** Starting memory checking *****/

   if (test1() == FAILURE) result = FAILURE;
   if (test2() == FAILURE) result = FAILURE;
   if (test3() == FAILURE) result = FAILURE;
   if (test4() == FAILURE) result = FAILURE;
   if (test5() == FAILURE) result = FAILURE;
   if (test6() == FAILURE) result = FAILURE;
   if (test7() == FAILURE) result = FAILURE;
   if (test8() == FAILURE) result = FAILURE;
   if (test9() == FAILURE) result = FAILURE;
   if (test10() == FAILURE) result = FAILURE;
   if (test11() == FAILURE) result = FAILURE;
   if (test12() == FAILURE) result = FAILURE;

   /****** Gathering memory checking info *****/
   stop_memory_check();
   /****** Gathering memory checking info *****/
   
   if (result == FAILURE) {
      exit(EXIT_FAILURE);
   }

   return EXIT_SUCCESS;
}
