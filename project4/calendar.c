/*
    Name: Darius Kianersi
    UID: 118799810
    Username: radius
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "calendar.h"
#include "event.h"
#include "my_memory_checker_216.h"

/*
    init_calendar initializes the calendar argument
    based on the parameters in the function. It
    returns FAILURE if the calendar or name
    pointers are null, or if the number of days
    is less than 1. It dynamically allocates
    memory for the calendar, the name, and
    the list of events.
*/
int init_calendar(const char *name, int days,
                  int (*comp_func) (const void *ptr1, const void *ptr2),
                  void (*free_info_func) (void *ptr), Calendar ** calendar) {
    if (!calendar || !name || days < 1) {
        return FAILURE;
    } else {
        *calendar = malloc(sizeof(Calendar));
        if (!(*calendar)) {
            return FAILURE;
        }

        (*calendar)->days = days;

        (*calendar)->comp_func = comp_func;

        (*calendar)->free_info_func = free_info_func;

        (*calendar)->name = malloc(strlen(name) + 1);
        if (!((*calendar)->name)) {
            return FAILURE;
        }
        strcpy((*calendar)->name, name);

        (*calendar)->events = calloc(days, sizeof(Event *));
        if (!((*calendar)->events)) {
            return FAILURE;
        }

        (*calendar)->total_events = 0;

        return SUCCESS;
    }
}

/*
    print_calendar prints a formatted output of
    the calendar to the given output_stream. If
    print_all is true, it also prints the calendar's
    name, days, and total events. print_calendar
    returns FAILURE if the calendar or output_stream
    pointers are null.
*/
int print_calendar(Calendar *calendar, FILE *output_stream, int print_all) {
    int i;
    Event *ptr;
    if (!calendar || !output_stream) {
        return FAILURE;
    } else {
        if (print_all) {
            fprintf(output_stream, 
                    "Calendar's Name: \"%s\"\nDays: %d\nTotal Events: %d\n\n", \
                    calendar->name, calendar->days, calendar->total_events);
        }
        fprintf(output_stream, "**** Events ****\n");
        if (calendar->total_events > 0) {
            /* Iterate through the days */
            for (i = 0; i < calendar->days; i++) {
                    ptr = calendar->events[i];
                    fprintf(output_stream, "Day %d\n", i + 1);
                    /* Iterate through the linked list */
                    while (ptr) {
                        fprintf(output_stream, \
                                "Event's Name: \"%s\", Start_time: %d, Duration: %d\n", \
                                ptr->name, 
                                ptr->start_time, 
                                ptr->duration_minutes);
                        ptr = ptr->next;
                    }
            }
        }
        return SUCCESS;
    }
}

/*
    add_event adds an event with the given parameters to
    the calendar. It adds the event in order based on
    comp_func to the linked list for the given day. If
    the calendar or name pointers are null, the duration
    is less than or equal to 0, the day is less than 1
    or greater than the number of days, or the event
    already exists, then the function returns FAILURE.
*/
int add_event(Calendar *calendar, const char *name, int start_time,
              int duration_minutes, void *info, int day) {
    if (
        !calendar || !name || 
        duration_minutes <= 0 || 
        day < 1 || day > calendar->days || 
        find_event(calendar, name, NULL) == SUCCESS) {
        return FAILURE;
    } else {
        Event *ptr = calendar->events[day - 1], *prev = NULL;
        Event *new_event = malloc(sizeof(Event));

        if (!new_event) {
            return FAILURE;
        }
        new_event->name = malloc(strlen(name) + 1);
        if (!(new_event->name)) {
            return FAILURE;
        }
        strcpy(new_event->name, name);

        /* Initialize the new event. */
        new_event->start_time = start_time;
        new_event->duration_minutes = duration_minutes;
        new_event->info = info;


        /* 
           Iterate through the linked list until either
           the pointer gets to the end, or the relative 
           value of the new event is less than the current
           event. 
        */
        while (ptr && (*(calendar->comp_func))(new_event, ptr) >= 0) {
            prev = ptr;
            ptr = ptr->next;
        }

        new_event->next = ptr;
        if (prev == NULL) {
            /* If the previous pointer is null,
            that is we are at the beginning,
            then we just have to update head. */
            calendar->events[day - 1] = new_event;
        } else {
            /* If we are in the middle of the
            list then we should update the next
            pointer of the previous event. */
            prev->next = new_event;
        }
        calendar->total_events++;
        return SUCCESS;
    }
}

/* 
    find_on_day_helper is the helper method for
    find_event_helper. It takes a given day and iterates
    through the linked list to find an event with
    the same name. It also has out parameters
    event and day for the event and the day number
    that it was found on.
*/
static int find_on_day_helper(
    Calendar *calendar, 
    const char *name, 
    Event **event, 
    int day, 
    int *day_out
) {
    if (!calendar || !name || day < 1 || day > calendar->days) {
        return FAILURE;
    } else {
        Event *ptr = calendar->events[day - 1];
        while (ptr) {
            /* If the name of the pointer event
            is the same as the event we're 
            looking for, then we've found it. */
            if (!strcmp(ptr->name, name)) {
                if (event) {
                    *event = ptr;
                }
                if (day_out) {
                    *day_out = day;
                }
                return SUCCESS;
            }
            ptr = ptr->next;
        }
        return FAILURE;
    }
}

/*
    find_event_helper is the helper for find_event.
    It iterates through the days and calls
    find_on_day_helper.
*/
static int find_event_helper(
    Calendar *calendar, 
    const char *name, 
    Event **event, 
    int *day_out
) {
    if (!calendar || !name) {
        return FAILURE;
    } else {
        int i, days = calendar->days;
        for (i = 0; i < days; i++) {
            /* Call the helper method with the given day. */
            if (find_on_day_helper(calendar, name, event, i + 1, day_out)
             == SUCCESS
             ) {
                return SUCCESS;
            }
        }
        return FAILURE;
    }
}

/* find_event calls find_event_helper. */
int find_event(Calendar *calendar, const char *name, Event **event) {
    return find_event_helper(calendar, name, event, NULL);
}

/* fidn_event_in_day calls find_on_day_helper. */
int find_event_in_day(Calendar *calendar, const char *name, int day,
                      Event **event) {
    return find_on_day_helper(calendar, name, event, day, NULL);
}

/*
    remove_event removes the event with the given name.
    First, it finds the day that the event is on using
    find_event_helper. Then, it traverses the linked
    list on the day to remove the event. It returns
    FAILURE if the calender or name pointers are NULL,
    or if the event is not found.
*/
int remove_event(Calendar *calendar, const char *name) {
    int day;
    /* While checking if the event is found, initialize the day
    out parameter with the day that it's found on. */
    if (
        !calendar || 
        !name || 
        find_event_helper(calendar, name, NULL, &day) == FAILURE
    ) {
        return FAILURE;
    } else {
        Event *ptr = calendar->events[day - 1], *prev = NULL;
        /* Traverse the linked list until we get to the event. */
        while (ptr && strcmp(ptr->name, name)) {
            prev = ptr;
            ptr = ptr->next;
        }

        if (prev == NULL) {
            /* If we are at the beginning of
            the list, then we need to simply
            update head. */
            calendar->events[day - 1] = ptr->next;
        } else {
            /* Otherwise, set the next
            pointer of the previous event. */
            prev->next = ptr->next;
        }

        if (ptr->info && calendar->free_info_func) {
            (calendar->free_info_func)(ptr->info);
        }
        free(ptr->name);
        free(ptr);
        calendar->total_events -= 1;
        return SUCCESS;
    }
}

/*
    get_event_info returns the info void pointer
    for an event. It returns NULL if the event
    does not exist.
*/
void *get_event_info(Calendar *calendar, const char *name) {
    Event *event;
    if (find_event(calendar, name, &event) == FAILURE) {
        return NULL;
    } else {
        return event->info;
    }
}

/* 
    clear_calendar uses clear_day to clear each 
    day in the calendar. It returns FAILURE if
    calendar is NULL.
*/
int clear_calendar(Calendar *calendar) {
    if (!calendar) {
        return FAILURE;
    } else {
        int i;
        /* Clear each day. */
        for (i = 0; i < calendar->days; i++) {
            clear_day(calendar, i + 1);
        }
        return SUCCESS;
    }
}

/* 
    clear_day traverses through the linked
    list for a day and clears each event.
*/
int clear_day(Calendar *calendar, int day) {
    if (!calendar || day < 1 || day > calendar->days) {
        return FAILURE;
    } else {
        Event *ptr, *tmp;
        int count = 0;
        ptr = calendar->events[day - 1];
        while (ptr) {
            /* Set a tmp pointer, since we will be clearing
            the event and won't have access to its next pointer. */
            tmp = ptr->next;
            if (ptr->info && calendar->free_info_func) {
                (calendar->free_info_func)(ptr->info);
            }
            free(ptr->name);
            free(ptr);
            ptr = tmp;
            count++;
        }
        calendar->events[day - 1] = NULL;
        calendar->total_events -= count;
        return SUCCESS;
    }
}

/*
    destroy_calendar clears a calendar
    and then deallocates the memory
    for the calendar itself.
*/
int destroy_calendar(Calendar *calendar) {
    if (!calendar) {
        return FAILURE;
    } else {
        clear_calendar(calendar);

        free(calendar->events);
        free(calendar->name);
        free(calendar);
        return SUCCESS;
    }
}
