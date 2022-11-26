/* timestamp.c: Timestamp Functions */

#include "pqsh/timestamp.h"

#include <time.h>
#include <sys/time.h>

/**
 * Return current timestamp as a double.
 *
 * Utilizes gettimeofday for precision (accounting for seconds and
 * microseconds) and falls back to time (only account for seconds) if that
 * fails.
 **/
double timestamp() 
{
    double final = 0;
    struct timeval my_time;
    gettimeofday(&my_time,NULL);
    
    if(!my_time.tv_usec) {
        final = my_time.tv_sec;
    }
    else {
        final = my_time.tv_sec + my_time.tv_usec / 1000000.0;
    }
    return final;
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
