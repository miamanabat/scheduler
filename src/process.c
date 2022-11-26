/* process.c: PQSH Process */

#include "pqsh/macros.h"
#include "pqsh/process.h"
#include "pqsh/timestamp.h"

#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

/**
 * Create new process structure given command.
 * @param   command     String with command to execute.
 * @return  Pointer to new process structure
 **/
Process *process_create(const char *command) {
    /* TODO: Implement */

    Process *p = malloc(sizeof(Process));

    if(p) {
        sprintf(p->command,"%s",command);

        p->pid = 0;

        p->arrival_time = timestamp();
        p->start_time = 0;
        p->end_time = 0;
        p->next = NULL;
        return p;
    }
    else {
        return NULL;
    }
}

/**
 * Start process by forking and executing the command.
 * @param   p           Pointer to Process structure.
 * @return  Whether or not starting the process was successful
 **/
bool process_start(Process *p) {
    /* TODO: Implement */

    p->pid = fork();
    if(p->pid < 0) // Error
    {
        return false;
    }
    if(p->pid == 0) // Child
    {
        int i = 0;
        char *argv[MAX_ARGUMENTS] = {0};
        for(char *token = strtok(p->command, " "); token; token = strtok(NULL, " ")) {
        argv[i++] = token;
        }
        if (execvp(argv[0], argv) < 0) {
            exit(false);
        }
    } 
    p->start_time = timestamp(); 
    return true;
}

/**
 * Pause process by sending it the appropriate signal.
 * @param   p           Pointer to Process structure.
 * @return  Whether or not sending the signal was successful.
 **/
bool process_pause(Process *p) {
    /* TODO: Implement */
    
    if(kill(p->pid,SIGSTOP) == 0) {
        return true;
    }
    else {
        return false;
    }
}

/**
 * Resume process by sending it the appropriate signal.
 * @param   p           Pointer to Process structure.
 * @return  Whether or not sending the signal was successful.
 */
bool process_resume(Process *p) {
    /* TODO: Implement */

    if(kill(p->pid,SIGCONT) == 0) {
        return true;
    }
    else {
        return false;
    }
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
