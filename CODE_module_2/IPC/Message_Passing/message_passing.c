/******************************************************************************
 * File: message_passing.c
 * 
 * Purpose:
 * This program demonstrates **Interprocess Communication (IPC)** using **POSIX message queues**.
 * This is a demnstration of indirect communication where the processes communicate by sending messages 
 * to a **shared message queue (mailbox)**. 
 * It implements a **producer-consumer** model where the **parent process** acts as the **producer** 
 * and the **child process** acts as the **consumer**.
 * 
 * Key Concepts Demonstrated:
 * 1. Creating a **POSIX message queue** using **mq_open()**.
 * 2. Sending messages using **mq_send()** and receiving them using **mq_receive()**.
 * 3. Synchronization between parent and child processes using **fork()**.
 * 4. Cleaning up resources with **mq_close()** and **mq_unlink()**.
 * 
 * Program Execution:
 * - The parent process creates a **message queue** and sends messages to it.
 * - The child process reads messages from the queue and displays them.
 * 
 * Output:
 * - Displays the messages sent by the parent process and received by the child process.
 * 
 * Usage:
 * - Compile: gcc message_passing.c -o process -lrt
 * - Execute: ./process
 * 
 * Learning Objectives for Students:
 * 1. Understand **POSIX message queues** as an IPC mechanism.
 * 2. Learn how to create, open, and close message queues in POSIX.
 * 3. Observe parent-child communication using **fork()** and **message passing**.
 * 4. Understand the importance of proper resource cleanup with **mq_unlink()**.
 * 
 * Experimentation Instructions:
 * 1. Observe message ordering and size limitations.
 * 2. Modify the program to handle variable message priorities.
 * 3. Extend the program to allow multiple readers and writers.
 * 4. Test error handling by attempting to read from an empty queue.
 * 
 *  
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>      // For O_* constants
#include <sys/stat.h>   // For mode constants
#include <mqueue.h>     // For POSIX message queues
#include <unistd.h>     // For fork(), sleep()
#include <sys/wait.h>   // For wait()

#define QUEUE_NAME "/posix_mq" // POSIX queue name
#define MAX_SIZE 1024                  // Max size of each message
#define MSG_STOP "STOP"                // Message indicating end

int main(void) {
    pid_t pid;                 // Process ID
    mqd_t mq;                  // Message queue descriptor
    struct mq_attr attr;       // Queue attributes
    char buffer[MAX_SIZE];     // Buffer for messages

    // Define message queue attributes
    attr.mq_flags = 0;                     // Blocking mode
    attr.mq_maxmsg = 10;                    // Max number of messages in queue
    attr.mq_msgsize = MAX_SIZE;             // Max message size
    attr.mq_curmsgs = 0;                     // Number of current messages

    // Create and open the message queue
    mq = mq_open(QUEUE_NAME, O_CREAT | O_RDWR, 0666, &attr);
    if (mq == -1) {
        perror("Message queue creation failed");
        exit(1);
    }

    // Fork a child process
    pid = fork();
    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    }

    if (pid > 0) { // **Parent Process (Producer)**

        printf("Parent: Sending messages to the queue.\n");
        
        // Send messages to the queue
        mq_send(mq, "Was it a rat I saw?", strlen("Was it a rat I saw?") + 1, 0);
        mq_send(mq, "No lemons, no melons.", strlen("No lemons, no melons.") + 1, 0);

        // Signal end of messages
        mq_send(mq, MSG_STOP, strlen(MSG_STOP) + 1, 0);

        // Wait for child process to complete
        wait(NULL);

        // Close and unlink the message queue (cleanup)
        mq_close(mq);
        mq_unlink(QUEUE_NAME);
    } 
    else { // **Child Process (Consumer)**

        printf("Child: Received the following messages from the queue:\n");

        while (1) {
            // Receive message from the queue
            ssize_t bytes_read = mq_receive(mq, buffer, MAX_SIZE, NULL);
            if (bytes_read < 0) {
                perror("Failed to receive message");
                exit(1);
            }

            buffer[bytes_read] = '\0'; // Null-terminate the string
            if (strcmp(buffer, MSG_STOP) == 0) {
                break; // Exit loop on STOP message
            }

            printf("Received Message: %s\n", buffer);
        }

        // Close the message queue
        mq_close(mq);
    }

    return 0;
}
