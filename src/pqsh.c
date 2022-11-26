/* pqsh.c: Process Queue Shell */

#include "pqsh/macros.h"
#include "pqsh/options.h"
#include "pqsh/scheduler.h"
#include "pqsh/signal.h"

#include <errno.h>
#include <string.h>
#include <sys/time.h>

/* Global Variables */

Scheduler PQShellScheduler = {
    .policy    = FIFO_POLICY,
    .cores     = 1,
    .timeout   = 250000,
};

/* Help Message */

void help() {
    printf("Commands:\n");
    printf("  add    command    Add command to waiting queue.\n");
    printf("  status [queue]    Display status of specified queue (default is all).\n");
    printf("  help              Display help message.\n");
    printf("  exit|quit         Exit shell.\n");
}

/* Main Execution */

int main(int argc, char *argv[]) {
    Scheduler *s = &PQShellScheduler;

    /* TODO: Parse command line options */

    if (!parse_command_line_options(argc, argv, s)) {
        help();
        return 1;
    } 
    /* TODO: Register signal handlers */
    if (!signal_register(SIGALRM, 0, sigalrm_handler)) {
        return 1;
    }
    /* TODO: Start timer interrupt */
    struct itimerval interval = {
        .it_interval = { .tv_sec = 0, .tv_usec = s->timeout },
        .it_value    = { .tv_sec = 0, .tv_usec = s->timeout },
    };
    if (setitimer(ITIMER_REAL, &interval, NULL) < 0) {
        return 1;
    }
    /* TODO: Process shell comands */
    // If queue specified, run scheduler_status(s,stdout,bitmask)
    while (!feof(stdin)) {
        char command[BUFSIZ]  = "";
        printf("\nPQSH> ");
        while (!fgets(command, BUFSIZ, stdin) && !feof(stdin));
        chomp(command);
        if (strlen(command) == 0) {
            continue;
        }

        char *token1 = strtok(command," "); //Divide up strings
        char *token2 = strtok(NULL,"");

        /* TODO: Handle add and status commands */
        if (streq(command, "help")) {
            help();
        } else if (streq(command, "exit") || streq(command, "quit")) {
            scheduler_delete(s);
            break;
        } else if (streq(token1, "add")) {
            scheduler_add(s, stdout, token2);
        } else if (streq(token1, "status")) {
            int queue = RUNNING | WAITING | FINISHED;
            if (token2) {
                if (strcmp(token2, "running") == 0) {
                    queue = RUNNING;
                } else if (strcmp(token2, "waiting") == 0) {
                    queue = WAITING;
                } else if (strcmp(token2, "finished") == 0) {
                    queue = FINISHED;
                }
            }
            scheduler_status(s, stdout, queue); 
        } else if (strlen(command)) {
            printf("Unknown command: %s\n", command);
        }     
    }
    return EXIT_SUCCESS;
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
