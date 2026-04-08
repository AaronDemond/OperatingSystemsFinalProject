/*
 * Peterson's Solution (2-process mutual exclusion)
 *
 * This program simulates two processes competing for a critical section.
 * It uses Peterson’s algorithm to guarantee that only one process is inside
 * the critical section at any time.
 *
 * Threads are used to simulate processes, but the logic is the same.
 */

#include "logger.h"
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define ITERATIONS 5

// Shared variables
// flag[i] = 1 means process i wants to enter the critical section
volatile int flag[2] = {0, 0};

// turn decides which process gets priority if both want to enter
volatile int turn = 0;

/*
 * Critical section
 * Only one process should be executing this at a time.
 */
void critical_section(int id) {
  printf("Process %d ENTERING critical section\n", id);
  LOG("Entering critical section");

  sleep(1); // simulate work
  printf("Process %d EXITING critical section\n", id);
  LOG("Leaving critical section");
}

/*
 * Simulated process (thread)
 * Each process repeatedly tries to enter the critical section.
 */
void *process(void *arg) {
  int id = *(int *)arg;
  int other = 1 - id;

  for (int i = 0; i < ITERATIONS; i++) {

    /*
     * ENTRY SECTION
     * Indicate interest, then give the other process priority.
     * This prevents both from entering at the same time.
     */
    flag[id] = 1;
    turn = other;

    /*
     * Busy waiting (spin)
     * If the other process also wants to enter and it is their turn,
     * we stay in this loop until it is safe to proceed.
     */
    while (flag[other] && turn == other)
      ;

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

    /*
     * REMAINDER SECTION
     * Code outside the critical section.
     */
    printf("Process %d in remainder section\n", id);
    sleep(1);
  }

  return NULL;
}

/*
 * Main creates two threads to simulate two concurrent processes.
 */
int main() {
  pthread_t t1, t2;
  int id1 = 0;
  int id2 = 1;
  LOG("Peterson simulation started");

  printf("Starting Peterson's Solution...\n\n");

  pthread_create(&t1, NULL, process, &id1);
  pthread_create(&t2, NULL, process, &id2);

  pthread_join(t1, NULL);
  pthread_join(t2, NULL);

  printf("\nSimulation finished.\n");
  LOG("Peterson simulation finished");
  return 0;
}
