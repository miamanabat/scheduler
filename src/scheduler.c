/* scheduler.c: PQSH Scheduler */

#include "pqsh/macros.h"
#include "pqsh/scheduler.h"
#include "pqsh/timestamp.h"

#include <errno.h>
#include <sys/wait.h>

/**
 * Add new command to waiting queue.
 * @param   s	    Pointer to Scheduler structure.
 * @param   fs      File stream to write to.
 * @param   command Command string for new Process.
 **/
void scheduler_add(Scheduler *s, FILE *fs, const char *command) {
    /* TODO: Implement */
    Process *p = process_create(command);
    queue_push(&(s->waiting), p);
    fprintf(fs, "Added process \"%s\" to waiting queue.\n", command);    
}

/**
 * Display status of queues in Scheduler.
 * @param   s	    Pointer to Scheduler structure.
 * @param   fs      File stream to write to.
 * @param   queue   Bitmask specifying which queues to display.
 **/
void scheduler_status(Scheduler *s, FILE *fs, int queue) {
    double average_turnaround = s->total_turnaround_time / s->finished.size; //Compute turnaround and response based on number of processes
    double average_response = s->total_response_time / s->finished.size;
    fprintf(fs, "Running = %4lu, Waiting = %4lu, Finished = %4lu, Turnaround = %05.2lf, Response = %05.2lf\n", s->running.size, s->waiting.size, s->finished.size, average_turnaround, average_response);
    /* TODO: Complement implementation. */
    if (queue & RUNNING && s->running.size > 0) { // Running
        fprintf(fs, "\nRunning Queue:\n");
        queue_dump(&s->running, fs);
    } 
    if (queue & WAITING && s->waiting.size > 0) { //Waiting 
        fprintf(fs, "\nWaiting Queue:\n");
        queue_dump(&s->waiting, fs);
    } 
    if (queue & FINISHED && s->finished.size > 0) { //Finished
        fprintf(fs, "\nFinished Queue:\n");
        queue_dump(&s->finished, fs);
    }
}

/**
 * Schedule next process using appropriate policy.
 * @param   s	    Pointer to Scheduler structure.
 **/
void scheduler_next(Scheduler *s) {
    /* TODO: Dispatch to appropriate scheduler function. */
    if (s->policy == FIFO_POLICY) {
       scheduler_fifo(s); 
    } else if (s->policy == RDRN_POLICY) {
        scheduler_rdrn(s);
    }
}

/**
 * Wait for any children and remove from queues and update metrics.
 * @param   s	    Pointer to Scheduler structure.
 **/
void scheduler_wait(Scheduler *s) {
    /* TODO: Wait for any children without blocking:
     *
     *  - Remove process from queues.
     *  - Update Process metrics.
     *  - Update Scheduler metrics.
     **/
    pid_t pid;
    while ((pid = waitpid(-1, NULL, WNOHANG)) > 0) {
        Process *found = queue_remove(&s->running, pid);
        // update metrics
        found->end_time = timestamp();
        double turnaround = found->end_time - found->arrival_time; //Compute turnaround and response without common denomiator because it changes for now
        s->total_turnaround_time += turnaround;
        double response_time = found->start_time - found->arrival_time;
        s->total_response_time += response_time;  
        queue_push(&s->finished, found);
    }
} 
/* deletes all the processes in scheduler */
void scheduler_delete(Scheduler *s) {
    queue_clear(&s->waiting);
    queue_clear(&s->running);
    queue_clear(&s->finished);  
}
/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
