/******************************************************************************
 * File: consumer.c
 * 
 * Purpose:
 * This program demonstrates interprocess communication (IPC) using **shared memory**. 
 * It acts as the consumer in a producer-consumer setup, reading messages from a 
 * shared memory object created by a producer process.
 * 
 * Key Concepts Demonstrated:
 * 1. Accessing an existing shared memory object using **shm_open()**.
 * 2. Mapping the shared memory object into the process's address space with **mmap()**.
 * 3. Reading data from shared memory.
 * 4. Unlinking the shared memory object using **shm_unlink()** to clean up resources.
 * 
 * Program Execution:
 * - Opens a shared memory object created by the producer process.
 * - Maps the shared memory object into the address space of the consumer process.
 * - Reads and displays the messages stored in shared memory.
 * - Unlinks the shared memory object to free system resources.
 * 
 * Output:
 * - Displays the messages written to the shared memory by the producer process.
 * 
 * Usage:
 * - Compile: gcc consumer.c -o consumer -lrt
 * - Execute: ./consumer
 * 
 * Learning Objectives for Students:
 * 1. Understand the use of shared memory for IPC.
 * 2. Learn how to access, map, and read from shared memory objects using POSIX APIs.
 * 3. Observe the synchronization between producer and consumer processes.
 * 4. Understand resource cleanup using **shm_unlink()**.
 * 
 * Experimentation Instructions:
 * 1. Run the producer program first to write messages to the shared memory object.
 * 2. Execute the consumer program to read and display the contents of the shared memory.
 * 3. Observe the behavior if the consumer is run without the producer creating the shared memory.
 * 4. Modify the program to handle larger or variable-sized messages.
 * 
 *  
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>

int main() {
    const char *name = "OS";  // name of the shared memory object
    const int SIZE = 4096;    // size of shared memory object
    int shm_fd;               // shared memory file descriptor
    void *ptr;                // pointer to shared memory object

    /* open the shared memory segment */
    shm_fd = shm_open(name, O_RDONLY, 0666);
    if (shm_fd == -1) {
        printf("Shared memory failed\n");
        exit(-1);
    }

    /* map the shared memory segment into the address space of the process */
    ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        printf("Memory mapping failed\n");
        exit(-1);
    }

    /* read from the shared memory region */
    printf("Consumer: Message read from shared memory: %s\n", (char *)ptr);

    /* remove the shared memory segment */
    if (shm_unlink(name) == -1) {
        printf("Error removing shared memory object: %s\n", name);
        exit(-1);
    }

    return 0;
}
