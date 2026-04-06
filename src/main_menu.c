#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "logger.h"

int main()
{
    int pipefd[2];

    if (pipe(pipefd) == -1)
    {
        perror("Pipe failed");
        return 1;
    }

    pid_t logger_pid = fork();

    if (logger_pid == 0)
    {
        /* Child process becomes the logger */

        close(pipefd[1]);                // close write end
        dup2(pipefd[0], STDIN_FILENO);   // pipe -> stdin
        close(pipefd[0]);

        execl("./logger", "logger", NULL);

        perror("Failed to start logger");
        exit(1);
    }

    /* Parent process */

    close(pipefd[0]);   // close read end

    char fd_str[10];
    sprintf(fd_str, "%d", pipefd[1]);
    setenv("LOGGER_FD", fd_str, 1);

    LOG("System started");

    int choice;

    while (1)
    {
        printf("\n===== OS Simulator =====\n");
        printf("1. File Management\n");
        printf("2. Peterson\n");
        printf("3. Memory Allocation\n");
        printf("4. Amdahl Law\n");
        printf("5. Exit\n");
        printf("Choice: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input\n");
            while (getchar() != '\n');
            continue;
        }

        LOGF("Main menu choice: %d", choice);

        pid_t pid = fork();

        if (pid == 0)
        {
            /* Child runs module */

            if (choice == 1)
            {
                LOG("Launching File Management module");
                execl("./file_management", "file_management", NULL);
            }
            else if (choice == 2)
            {
                LOG("Launching Peterson module");
                execl("./peterson", "peterson", NULL);
            }
            else if (choice == 3)
            {
                LOG("Launching Memory module");
                execl("./memory", "memory", NULL);
            }
            else if (choice == 4)
            {
                LOG("Launching Amdahl module");
                execl("./amdahl", "amdahl", NULL);
            }
            else if (choice == 5)
            {
                LOG("System exiting");
                exit(0);
            }

            perror("Exec failed");
            exit(1);
        }

        wait(NULL);

        if (choice == 5)
            break;
    }

    close(pipefd[1]);

    return 0;
}
