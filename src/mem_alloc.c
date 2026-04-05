#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include "logger.h"

// [Original mem_block and mblock_array structs unchanged...]

// [All original functions unchanged except logging calls added at key points]

int main(void) {
    unsigned int total_mem;
    printf("Enter total memory size: ");
    scanf("%u", &total_mem);
    write_log_input("MEMORY", "total_memory", total_mem);

    mblock_array arr;
    mba_init(&arr, 8, total_mem);

    // [Original initialization code...]

    unsigned int num_holes;
    printf("Enter number of initial holes: ");
    scanf("%u", &num_holes);
    write_log_input("MEMORY", "initial_holes", num_holes);

    for (unsigned int i = 0; i < num_holes; i++) {
        unsigned int start, size;
        printf("Hole %u start: ", i + 1);
        scanf("%u", &start);
        printf("Hole %u size: ", i + 1);
        scanf("%u", &size);

        char hole_str[64];
        sprintf(hole_str, "hole_%u_start=%u_size=%u", i+1, start, size);
        write_log("MEMORY", hole_str);

        // [Original hole insertion code...]
    }

    mba_print(&arr);
    write_log("MEMORY", "Initial memory map printed");

    int choice;
    do {
        printf("\n--- Memory Manager ---\n");
        printf("1. Allocate\n");
        printf("2. Deallocate\n");
        printf("3. Display Memory Map\n");
        printf("4. Compact\n");
        printf("5. Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);
        write_log_input("MEMORY", "menu_choice", choice);

        switch (choice) {
            case 1: {
                unsigned int pid, size;
                printf("Process ID: ");
                scanf("%u", &pid);
                printf("Size: ");
                scanf("%u", &size);

                char alloc_str[64];
                sprintf(alloc_str, "allocate_pid=%u_size=%u", pid, size);
                write_log("MEMORY", alloc_str);

                mba_push(&arr, size, pid);
                break;
            }
            case 2: {
                unsigned int pid;
                printf("Process ID: ");
                scanf("%u", &pid);
                write_log_input("MEMORY", "dealloc_pid", pid);
                mba_dealloc(&arr, pid);
                break;
            }
            case 3:
                mba_print(&arr);
                break;
            case 4:
                write_log("MEMORY", "Compaction started");
                mba_compact(&arr);
                printf("Memory compacted.\n");
                mba_print(&arr);
                write_log("MEMORY", "Compaction complete");
                break;
            case 5:
                write_log("MEMORY", "User selected exit");
                printf("Exiting.\n");
                break;
            default:
                write_log("MEMORY", "Invalid menu choice");
                printf("Invalid choice.\n");
        }
    } while (choice != 5);

    mba_free(&arr);
    write_log("MEMORY", "Module finished");
    return 0;
}
