/******************************************************************************
 * File: consumer.c
 * 
 * Purpose:
 * This program demonstrates the use of a **named pipe (FIFO)** for interprocess 
 * communication. It acts as the consumer in a producer-consumer setup, 
 * reading a message from a named pipe that was written by a producer process.
 * 
 * Key Concepts Demonstrated:
 * 1. Using a named pipe (FIFO) for one-way interprocess communication.
 * 2. Opening a named pipe for reading using **open()** with the **O_RDONLY** flag.
 * 3. Reading data from the pipe using **read()** and displaying it to the user.
 * 
 * Program Execution:
 * - Opens an existing named pipe in read-only mode.
 * - Reads the message written by the producer process from the pipe.
 * - Displays the received message to the user.
 * 
 * Output:
 * - Prints the message received from the named pipe.
 * 
 * Usage:
 * - Compile: gcc consumer.c -o consumer
 * - Execute: ./consumer
 * 
 * Learning Objectives for Students:
 * 1. Understand the use of **O_RDONLY** for reading from a named pipe.
 * 2. Learn how to read and display data from a named pipe using **read()**.
 * 3. Analyze how the consumer process interacts with the producer in a FIFO setup.
 * 4. Experiment with various scenarios, such as reading messages of different lengths or multiple consumers reading from the same pipe.
 * 
 * Experimentation Instructions:
 * 1. Run the consumer program after the producer has written a message to the named pipe.
 * 2. Test what happens if the consumer starts before the producer writes to the pipe.
 * 3. Simulate multiple consumers attempting to read from the same named pipe and observe the behavior.
 * 4. Modify the buffer size and observe how it affects the reading process.
 * 
 *  
 *****************************************************************************/

#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_BUF 1024

int main() {
    int fd;
    char *myfifo = "/tmp/myfifo";
    char buf[MAX_BUF];

    /* open, read, & display the msg from the FIFO */
    fd = open(myfifo, O_RDONLY);   
    read(fd, buf, MAX_BUF);
    printf("Receiver: Received: message [%s] from the pipe\n", buf);
    
    close(fd);

    return 0;
}
