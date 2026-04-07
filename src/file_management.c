#include <stdio.h>
#include <stdlib.h>
#include <dirent.h> // for listing files
#include "logger.h"

// Function prototypes
void create_file();
void read_file();
void list_files();
void delete_file();

int main()
{
    int choice;

    while (1)
    {
        printf("\n===== File Management System =====\n");
        printf("1. Create File\n");
        printf("2. Read File\n");
        printf("3. List Files\n");
        printf("4. Delete File\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input\n");
            while (getchar() != '\n')
                ; // clear buffer
            continue;
        }

        switch (choice)
        {
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
            LOG("User exited file management");
            printf("Exiting...\n");
            return 0;
        default:
            printf("Invalid choice\n");
        }
    }
}

// ================= CREATE FILE =================
void create_file()
{
    char filename[100];
    char content[1000];

    printf("Enter filename: ");
    scanf("%99s", filename);
    LOGF("Create file requested: %d", filename);

    
    FILE *fp = fopen(filename, "w");

    if (fp == NULL)
    {
        printf("Error creating file\n");
        LOG("File creation failed");
        return;
    }

    printf("Enter content: ");
    getchar(); // clear newline from buffer
    while (fgets(content, sizeof(content), stdin) != NULL)
    {
        fprintf(fp, "%s", content);
    }
    clearerr(stdin); // clear EOF flag

    fclose(fp);

    printf("File created successfully\n");
    LOG("File created");
}

// ================= READ FILE =================
void read_file()
{
    // Buffers to store the filename and each line read from the file
    char filename[100];
    char line[1000];

    // Prompt user for the file name
    printf("Enter filename: ");
    scanf("%s", filename); // Read file name (stops at whitespace)
    LOGF("Read file requested %d", filename);


    // Open the file in read mode ("r") and obtain a file stream pointer
    FILE *fp = fopen(filename, "r");

    // Check if the file was opened successfully
    if (fp == NULL)
    {
        printf("File not found\n");
        LOG("File read failed");
        return; // Exit function if file cannot be opened
    }

    // Print header for clarity
    printf("\n--- File Content ---\n");
    LOG("File read");


    // Read the file line by line
    // fgets reads up to a newline or buffer size and returns NULL at end of file
    while (fgets(line, sizeof(line), fp))
    {
        // Print each line (fgets preserves the newline character)
        printf("%s", line);
    }

    // Close the file stream and release system resources
    fclose(fp);
}

// ================= LIST FILES =================
void list_files()
{
    // Pointer to a directory entry structure (represents one file/folder at a time)
    struct dirent *entry;

    // Open the current directory (".") and obtain a directory stream
    DIR *dir = opendir(".");

    // Check if the directory was opened successfully
    if (dir == NULL)
    {
        printf("Unable to open directory\n");
        return; // Exit function if directory cannot be accessed
    }

    // Print header for clarity
    printf("\n--- Files in Directory ---\n");
    LOG("Listing files");


    // Iterate through directory entries one by one
    // readdir returns a pointer to the next entry, or NULL when no more entries exist
    while ((entry = readdir(dir)) != NULL)
    {
        // Access the name of the file/directory from the struct and print it
        printf("%s\n", entry->d_name);
    }

    // Close the directory stream and release system resources
    closedir(dir);
}

// ================= DELETE FILE =================
void delete_file()
{
    // Buffer to store the file name
    char filename[100];

    // Get file name from user
    printf("Enter filename: ");
    scanf("%s", filename);
    LOGF("Delete file %d", filename);


    // Attempt to delete the file
    // remove() returns 0 on success, non-zero on failure
    if (remove(filename) == 0)
    {
        printf("File deleted successfully\n");
        LOG("File deleted");

    }
    else
    {
        printf("Error deleting file\n");
        LOG("File deletion failed");

    }
}
