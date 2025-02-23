/******************************************************************************
 * File: sh.c
 * 
 * Purpose:
 * This program demonstrates interprocess communication (IPC) using **POSIX shared memory**. 
 * It combines the producer and consumer logic into a **single program** using **fork()** 
 * to create a parent (producer) and child (consumer) process.
 * 
 * Key Concepts Demonstrated:
 * 1. Creating a shared memory object using **shm_open()**.
 * 2. Configuring and mapping the shared memory object into the process's address space using **ftruncate()** and **mmap()**.
 * 3. Writing data to shared memory (Producer) and reading it (Consumer).
 * 4. Using **shm_unlink()** to clean up shared memory resources after use.
 * 
 * Program Execution:
 * - The parent process acts as the **producer**, writing data to the shared memory.
 * - The child process acts as the **consumer**, reading the data from the shared memory.
 * - The processes use shared memory for communication without requiring explicit synchronization mechanisms.
 * 
 * Output:
 * - Parent writes a message to shared memory, which the child reads and displays.
 * 
 * Usage:
 * - Compile: gcc shm.c -o process -lrt
 * - Execute: ./process
 * 
 * Learning Objectives for Students:
 * 1. Understand the fundamentals of POSIX shared memory for IPC.
 * 2. Learn how to create, configure, and map shared memory segments.
 * 3. Observe parent-child process communication using **fork()** and **shared memory**.
 * 4. Understand proper resource management using **shm_unlink()** and **munmap()**.
 * 
 * Experimentation Instructions:
 * 1. Observe how the child process waits for the parent to write data.
 * 2. Modify the program to handle larger messages or structured data formats.
 * 3. Experiment with multiple child processes reading from the same shared memory.
 * 4. Test the behavior when the shared memory object is unlinked before the child reads.
 * 5. Explore the impact of different message sizes on shared memory performance.
 * 
 * 
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>  // For mode constants
#include <fcntl.h>     // For O_* constants
#include <sys/wait.h>  // For wait()

#define SIZE 4096  // Shared memory size
#define NAME "OS"  // Shared memory object name

int main() {
    int shm_fd;      // Shared memory file descriptor
    void *ptr;       // Pointer to shared memory object

    // Create shared memory object
    shm_fd = shm_open(NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("Shared memory creation failed");
        exit(1);
    }

    // Configure the size of the shared memory object
    if (ftruncate(shm_fd, SIZE) == -1) {
        perror("Failed to set size of shared memory");
        exit(1);
    }

    // Map shared memory to address space
    ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        perror("Memory mapping failed");
        exit(1);
    }

    // Fork a child process
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    }

    if (pid == 0) {
        // **Child Process - Consumer**
        sleep(2); // Ensure parent writes first

        printf("Child (Consumer): Read the message '%s' from the sahred memory\n", (char *)ptr);

        // Unlink shared memory (cleanup)
        if (shm_unlink(NAME) == -1) {
            perror("Error removing shared memory");
            exit(1);
        }

    } else {
        // **Parent Process - Producer**
        const char *message0 = "Studying Operating Systems ";
        const char *message1 = "Is Fun!";

        // Write to shared memory
        printf("Parent (Producer): Writing the message '%s%s' to shared memory\n",message0, message1);
        sprintf(ptr, "%s%s", message0, message1); // Write messages to shared memory

        // Wait for child to finish
        wait(NULL);
    }

    // Cleanup memory mapping
    munmap(ptr, SIZE);

    return 0;
}
