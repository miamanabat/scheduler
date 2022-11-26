/* queue.c: PQSH Queue */

#include "pqsh/macros.h"
#include "pqsh/queue.h"

#include <assert.h>

/**
 * Push process to back of queue.
 * @param q     Pointer to Queue structure.
 **/
void        queue_push(Queue *q, Process *p) {
    /* TODO: Implement */
    if (q->size == 0) {
        q->head = p;
        q->tail = p;
        q->size = 1;
    } else {
        q->tail->next = p;
        q->tail = p;
        q->size++;
    }
}

/**
 * Pop process from front of queue.
 * @param q     Pointer to Queue structure.
 * @return  Process from front of queue.
 **/
Process *   queue_pop(Queue *q) {
    /* TODO: Implement */
    if (q->head == NULL) {
        return NULL;
    }
    Process *temp = q->head;
    q->head = q->head->next;
    temp->next = NULL;
    q->size--;
    return temp;
}

/**
 * Remove and return process with specified pid.
 * @param q     Pointer to Queue structure.
 * @param pid   Pid of process to return.
 * @return  Process from Queue with specified pid.
 **/
Process *   queue_remove(Queue *q, pid_t pid) {
    /* TODO: Implement */
    Process *temp = q->head;
    Process *slow = q->head;
    while (temp != NULL) {
        if (temp->pid == pid) { 
            if (temp == q->head) {
                q->head = temp->next;
            } 
            if (temp == q->tail) {
                q->tail = slow;
            } else {
                slow->next = temp->next;
            }
            q->size--;
            temp->next = NULL;
            return temp;
        }
        slow = temp;
        temp = temp->next;
    }
    return NULL;
}

/**
 * Dump the contents of the Queue to the specified stream.
 * @param q     Queue structure.
 * @param fs    Output file stream.
 **/
void        queue_dump(Queue *q, FILE *fs) {
    fprintf(fs, "%6s %-30s %-13s %-13s %-13s\n", 
                "PID", "COMMAND", "ARRIVAL", "START", "END");
    /* TODO: Display information for each item in Queue. */
    Process *p = q->head;
    while (p != NULL) {
        fprintf(fs, "%6ld %-30s %.2lf %.2lf %.2lf\n", (long)(p->pid), p->command, p->arrival_time, p->start_time, p->end_time);
       p = p->next; 
    }
}

/* Deallocates all the processes in each of the queues */
void queue_clear(Queue *q) {
    Process *p = q->head;
    Process *p_next;
    while (p) {
        p_next = p->next;
        free(p);
        p = p_next;
    }
}
/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
