#ifndef _DATE_H_INCLUDED_
#define _DATE_H_INCLUDED_
#include <stdio.h>
#include <stdlib.h>

typedef struct date {
    int day;
    int month;
    int year;
} Date;

/*
 * date_create creates a Date structure from `datestr`
 * `datestr' is expected to be of the form "dd/mm/yyyy"
 * returns pointer to Date structure if successful,
 *         NULL if not (syntax error)
 */
Date *date_create(char *datestr) {
    
    Date *date_ptr = (Date *) malloc (sizeof(struct date));
    if (date_ptr == NULL) {return NULL;}

    int d;
    int m;
    int y;
    sscanf(datestr, "%d/%d/%d", &d, &m, &y);
    date_ptr->day = d;
    date_ptr->month = m;
    date_ptr->year = y;

    return date_ptr;
}

/*
 * date_duplicate creates a duplicate of `d'
 * returns pointer to new Date structure if successful,
 *         NULL if not (memory allocation failure)
 */
Date *date_duplicate(Date *d) {

    Date *date_ptr = (Date *) malloc (sizeof(struct date));
    if (date_ptr == NULL) {return NULL;}

    int day = d->day;
    int month = d->month;
    int year = d->year;

    date_ptr->day = day;
    date_ptr->month = month;
    date_ptr->year = year;

    return date_ptr;
}

/*
 * date_compare compares two dates, returning <0, 0, >0 if
 * date1<date2, date1==date2, date1>date2, respectively
 */
int date_compare(Date *date1, Date *date2) {
    if(date1->year > date2->year) {return 1;}
    if(date2->year > date1->year) {return -1;}
    if(date1->month > date2->month) {return 1;}
    if(date2->month > date1->month) {return -1;}
    if(date1->day > date2->day) {return 1;}
    if(date2->day > date1->day) {return -1;}
    return 0;
}

/*
 * date_destroy returns any storage associated with `d' to the system
 */
void date_destroy(Date *d) {
    free(d);
}

#endif /* _DATE_H_INCLUDED_ */
