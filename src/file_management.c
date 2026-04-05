#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>  // for listing files
#include "logger.h"

// Function prototypes
void create_file();
void read_file();
void list_files();
void delete_file();

int main() {
    int choice;

    write_log("FILE_MANAGEMENT", "Module started");

    while (1) {
        printf("\n===== File Management System =====\n");
        printf("1. Create File\n");
        printf("2. Read File\n");
        printf("3. List Files\n");
        printf("4. Delete File\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            write_log("FILE_MANAGEMENT", "Invalid menu input");
            printf("Invalid input\n");
            while (getchar() != '\n'); // clear buffer
            continue;
        }

        write_log_input("FILE_MANAGEMENT", "menu_choice", choice);

        switch (choice) {
            case 1:
                create_file();
                break;
            case 2:
                read_file();
                break;
            case 3:
                list_files();
                break;
            case 4:
                delete_file();
                break;
            case 5:
                write_log("FILE_MANAGEMENT", "User selected exit");
                printf("Exiting...\n");
                write_log("FILE_MANAGEMENT", "Module finished");
                return 0;
            default:
                write_log("FILE_MANAGEMENT", "Invalid menu choice");
                printf("Invalid choice\n");
        }
    }
}

// ================= CREATE FILE =================
void create_file() {
    char filename[100];
    char content[1000];

    printf("Enter filename: ");
    scanf("%99s", filename);
    write_log_input("FILE_MANAGEMENT", "create_filename", filename);

    FILE *fp = fopen(filename, "w");

    if (fp == NULL) {
        write_log("FILE_MANAGEMENT", "Create file failed");
        printf("Error creating file\n");
        return;
    }

    printf("Enter content: ");
    getchar(); // clear newline from buffer
    fgets(content, sizeof(content), stdin);

    fprintf(fp, "%s", content);
    fclose(fp);

    write_log("FILE_MANAGEMENT", "File created successfully");
    printf("File created successfully\n");
}

// ================= READ FILE =================
void read_file() {
    // Buffers to store the filename and each line read from the file
    char filename[100];
    char line[1000];

    // Prompt user for the file name
    printf("Enter filename: ");
    scanf("%s", filename); // Read file name (stops at whitespace)
    write_log_input("FILE_MANAGEMENT", "read_filename", filename);

    // Open the file in read mode ("r") and obtain a file stream pointer
    FILE *fp = fopen(filename, "r");

    // Check if the file was opened successfully
    if (fp == NULL) {
        write_log("FILE_MANAGEMENT", "Read file not found");
        printf("File not found\n");
        return; // Exit function if file cannot be opened
    }

    write_log("FILE_MANAGEMENT", "File read started");

    // Print header for clarity
    printf("\n--- File Content ---\n");

    // Read the file line by line
    // fgets reads up to a newline or buffer size and returns NULL at end of file
    while (fgets(line, sizeof(line), fp)) {
        // Print each line (fgets preserves the newline character)
        printf("%s", line);
    }

    // Close the file stream and release system resources
    fclose(fp);

    write_log("FILE_MANAGEMENT", "File read complete");
}

// ================= LIST FILES =================
void list_files() {
    // Pointer to a directory entry structure (represents one file/folder at a time)
    struct dirent *entry;

    // Open the current directory (".") and obtain a directory stream
    DIR *dir = opendir(".");

    // Check if the directory was opened successfully
    if (dir == NULL) {
        write_log("FILE_MANAGEMENT", "List files failed");
        printf("Unable to open directory\n");
        return; // Exit function if directory cannot be accessed
    }

    write_log("FILE_MANAGEMENT", "Listing directory");

    // Print header for clarity
    printf("\n--- Files in Directory ---\n");

    // Iterate through directory entries one by one
    // readdir returns a pointer to the next entry, or NULL when no more entries exist
    while ((entry = readdir(dir)) != NULL) {
        // Access the name of the file/directory from the struct and print it
        printf("%s\n", entry->d_name);
    }

    // Close the directory stream and release system resources
    closedir(dir);

    write_log("FILE_MANAGEMENT", "Directory list complete");
}

// ================= DELETE FILE =================
void delete_file() {
    // Buffer to store the file name
    char filename[100];

    // Get file name from user
    printf("Enter filename: ");
    scanf("%s", filename);
    write_log_input("FILE_MANAGEMENT", "delete_filename", filename);

    // Attempt to delete the file
    // remove() returns 0 on success, non-zero on failure
    if (remove(filename) == 0) {
        write_log("FILE_MANAGEMENT", "File deleted successfully");
        printf("File deleted successfully\n");
    } else {
        write_log("FILE_MANAGEMENT", "Delete file failed");
        printf("Error deleting file\n");
    }
}
