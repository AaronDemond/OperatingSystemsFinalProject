#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include "logger.h"

int main() {
    int choice;

    write_log("MAIN_MENU", "Main menu started");

    while (1) {
        printf("\n=== CSCI 3431 OS Project ===\n");
        printf("1. Logger\n");
        printf("2. Peterson's Solution\n");
        printf("3. File Management\n");
        printf("4. Memory Allocation\n");
        printf("5. Amdahl's Law\n");
        printf("0. Quit\n");
        printf("Choice: ");

        scanf("%d", &choice);
        write_log_input("MAIN_MENU", "choice", (choice == 0) ? "quit" : "menu_option");

        if (choice == 0) {
            write_log("MAIN_MENU", "User quit program");
            break;
        }

        pid_t pid = fork();
        if (pid == 0) {
            switch (choice) {
                case 1:
                    write_log("MAIN_MENU", "Launching logger module");
                    execl("./logger", "logger", NULL);
                    break;
                case 2:
                    write_log("MAIN_MENU", "Launching peterson module");
                    execl("./peterson", "peterson", NULL);
                    break;
                case 3:
                    write_log("MAIN_MENU", "Launching file_management module");
                    execl("./file_management", "file_management", NULL);
                    break;
                case 4:
                    write_log("MAIN_MENU", "Launching memory module");
                    execl("./memory", "memory", NULL);
                    break;
                case 5:
                    write_log("MAIN_MENU", "Launching amdahl module");
                    execl("./amdahl", "amdahl", NULL);
                    break;
                default:
                    write_log("MAIN_MENU", "Invalid menu choice");
                    printf("Invalid choice\n");
            }
            // exec failed
            write_log("MAIN_MENU", "exec failed for module");
            return 1;
        } else {
            // Parent waits for child
            wait(NULL);
            write_log("MAIN_MENU", "Module execution completed");
        }
    }

    write_log("MAIN_MENU", "Program terminated");
    return 0;
}
