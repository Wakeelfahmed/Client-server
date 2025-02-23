/******************************************************************************
 * File: producer.c
 * 
 * Purpose:
 * This program demonstrates the use of a **named pipe (FIFO)** for interprocess 
 * communication. It acts as the producer in a producer-consumer setup, 
 * writing a message into the named pipe for a consumer process to read.
 * 
 * Key Concepts Demonstrated:
 * 1. Creating a named pipe (FIFO) using **mkfifo()**.
 * 2. Writing data to a named pipe using **open()** and **write()** system calls.
 * 3. Cleaning up resources by removing the named pipe with **unlink()**.
 * 4. Demonstrating one-way communication between processes using a FIFO.
 * 
 * Program Execution:
 * - Creates a named pipe in the file system.
 * - Writes a predefined message into the pipe.
 * - Closes the pipe and removes it from the file system.
 * 
 * Output:
 * - No direct output from this program, but the message is written to the named pipe.
 * - To observe the communication, a separate consumer process must read from the pipe.
 * 
 * Usage:
 * - Compile: gcc producer.c -o producer
 * - Execute: ./producer
 * 
 * Learning Objectives for Students:
 * 1. Understand the concept and functionality of named pipes in IPC.
 * 2. Learn how to create, open, write to, and clean up a named pipe.
 * 3. Analyze how the producer-consumer model works with FIFOs.
 * 4. Observe the behavior when multiple producers or consumers interact with the same pipe.
 * 
 * Experimentation Instructions:
 * 1. Pair this program with a consumer program that reads from the same named pipe.
 * 2. Modify the message and observe its impact on the consumer process output.
 * 3. Test scenarios where the producer writes to the pipe before the consumer starts reading.
 * 4. Simulate multiple producer processes to understand the FIFO queue behavior.
 * 5. Observe what happens if the consumer process does not read from the pipe.
 * 
 *  
 *****************************************************************************/

#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    int fd;
    char *myfifo = "/tmp/myfifo";
    char msg[] = "Hi, this is a named pipe";

    /* create the FIFO (named pipe) */
    mkfifo(myfifo, 0666);

    /* write msg to the FIFO */
    printf("Producer: Writing the message [%s] to the pipe\n", msg);
    printf("Producer: Waiting for the consumer to read the message...\n");
    fd = open(myfifo, O_WRONLY);   
    write(fd, msg, sizeof(msg));

    close(fd);

    /* remove the FIFO */
    unlink(myfifo);
    return 0;
}
