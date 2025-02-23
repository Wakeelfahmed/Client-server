/******************************************************************************
 * File: producer.c
 * 
 * Purpose:
 * This program demonstrates interprocess communication (IPC) using **shared memory**. 
 * It acts as the producer in a producer-consumer setup, writing messages to a 
 * shared memory object that can be read by a consumer process.
 * 
 * Key Concepts Demonstrated:
 * 1. Creating and managing a shared memory object using **shm_open()**.
 * 2. Configuring the size of the shared memory segment using **ftruncate()**.
 * 3. Mapping the shared memory object into the process's address space with **mmap()**.
 * 4. Writing data to shared memory and updating the pointer to avoid overwriting.
 * 5. Demonstrating shared memory as a method of efficient IPC.
 * 
 * Program Execution:
 * - Creates a shared memory object with a specified name and size.
 * - Maps the shared memory object into the address space of the producer process.
 * - Writes multiple messages into the shared memory region.
 * - Leaves the shared memory for the consumer process to read.
 * 
 * Output:
 * - The program does not produce output on the console. It writes messages into
 *   shared memory, which are later retrieved by the consumer process.
 * 
 * Usage:
 * - Compile: gcc producer.c -o producer -lrt
 * - Execute: ./producer
 * 
 * Learning Objectives for Students:
 * 1. Understand the use of shared memory for IPC.
 * 2. Learn how to create, map, and write to shared memory objects using POSIX APIs.
 * 3. Observe how memory-mapped regions are used for data exchange.
 * 4. Experiment with different message sizes and observe how pointers are updated.
 * 
 * Experimentation Instructions:
 * 1. Run the producer program to write messages into the shared memory object.
 * 2. Verify the contents of the shared memory using the corresponding consumer program.
 * 3. Experiment with different sizes of shared memory objects and messages.
 * 4. Test what happens when the shared memory size is insufficient for the data.
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
#include <unistd.h>

int main() {
    const int SIZE = 4096;  // the size of the shared memory object
    const char *name = "OS";  // name of the shared memory object
    const char *message0 = "Studying Operating Systems ";  // string written to shared memory
    const char *message1 = "Is Fun!";  // string written to shared memory

    int shm_fd;  // shared memory file descriptor
    void *ptr;   // pointer to shared memory object

    /* create the shared memory segment/object */
    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        printf("Shared memory creation failed\n");
        return -1;
    }

    /* configure the size of the shared memory segment */
    if (ftruncate(shm_fd, SIZE) == -1) {
        printf("Failed to configure the size of shared memory\n");
        return -1;
    }

    /* map the shared memory segment into the address space of the process */
    ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        printf("Memory mapping failed\n");
        return -1;
    }

    /* write to the shared memory region */
    printf("Producer: Writing the message '%s%s' to the shared memory region\n",message0, message1);
    sprintf(ptr, "%s", message0);
    ptr += strlen(message0);
    sprintf(ptr, "%s", message1);

    return 0;
}
