#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "logger.h"

#define ITERATIONS 5

// Shared variables
// flag[i] = 1 means process i wants to enter the critical section
int flag[2] = {0, 0};

// turn decides which process gets priority if both want to enter
int turn = 0;

/*
 * Critical section
 * Only one process should be executing this at a time.
 */
void critical_section(int id) {
    write_log_input("PETERSON", "critical_section", id ? "process1" : "process0");
    printf("Process %d ENTERING critical section\n", id);
    sleep(1); // simulate work
    printf("Process %d EXITING critical section\n", id);
    write_log("PETERSON", "Critical section exit");
}

/*
 * Simulated process (thread)
 * Each process repeatedly tries to enter the critical section.
 */
void *process(void *arg) {
    int id = *(int *)arg;
    int other = 1 - id;

    char proc_name[16];
    sprintf(proc_name, "process%d", id);
    write_log_input("PETERSON", "process_id", proc_name);

    for (int i = 0; i < ITERATIONS; i++) {
        write_log_input("PETERSON", "iteration", i);

        /*
         * ENTRY SECTION
         * Indicate interest, then give the other process priority.
         * This prevents both from entering at the same time.
         */
        flag[id] = 1;
        turn = other;
        write_log("PETERSON", "Entry section complete");

        /*
         * Busy waiting (spin)
         * If the other process also wants to enter and it is their turn,
         * we stay in this loop until it is safe to proceed.
         */
        while (flag[other] && turn == other);

        write_log("PETERSON", "Busy waiting ended");

        /*
         * CRITICAL SECTION
         * At this point, only one process can reach here.
         */
        critical_section(id);

        /*
         * EXIT SECTION
         * Clear the flag so the other process can enter.
         */
        flag[id] = 0;
        write_log("PETERSON", "Exit section complete");

        /*
         * REMAINDER SECTION
         * Code outside the critical section.
         */
        printf("Process %d in remainder section\n", id);
        sleep(1);
    }

    write_log("PETERSON", "Process finished all iterations");
    return NULL;
}

/*
 * Main creates two threads to simulate two concurrent processes.
 */
int main() {
    pthread_t t1, t2;
    int id1 = 0;
    int id2 = 1;

    write_log("PETERSON", "Module started");
    printf("Starting Peterson's Solution...\n\n");

    pthread_create(&t1, NULL, process, &id1);
    pthread_create(&t2, NULL, process, &id2);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("\nSimulation finished.\n");
    write_log("PETERSON", "Module finished");

    return 0;
}
