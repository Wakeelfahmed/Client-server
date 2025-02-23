/******************************************************************************
 * File: multiple_threadeds.c
 * 
 * Purpose:
 * This program demonstrates the creation and management of multiple threads 
 * in a C program using the POSIX thread library (**pthread**). Each thread executes 
 * a function that prints its unique identifier, demonstrating basic multithreading 
 * concepts.
 * 
 * Key Concepts Demonstrated:
 * 1. Creation of multiple threads using **pthread_create()**.
 * 2. Passing unique data to each thread using arguments.
 * 3. Synchronization of the main thread with worker threads using **pthread_join()**.
 * 4. Error handling during thread creation.
 * 
 * Program Execution:
 * - Four threads are created, each receiving a unique identifier as input.
 * - Each thread executes a function to print its identifier.
 * - The main process waits for all threads to complete using **pthread_join()**.
 * 
 * Output:
 * - Each thread outputs a message indicating its unique identifier.
 * - The main process outputs a message indicating that it is the main process.
 * 
 * Usage:
 * - Compile: gcc multiple_threadeds.c -o mthread -lpthread
 * - Execute: ./mthread
 * 
 * Learning Objectives for Students:
 * 1. Understand how to create and manage multiple threads in a C program.
 * 2. Learn how to pass arguments to threads and manage thread-specific data.
 * 3. Observe the synchronization of threads with the main process.
 * 4. Understand basic error handling for thread creation.
 * 
 * Experimentation Instructions:
 * 1. Run the program and observe the output of each thread and the main process.
 * 2. Modify the number of threads and experiment with different data passed to 
 *    each thread.
 * 3. Introduce delays in the thread function to observe execution order and 
 *    synchronization.
 * 4. Implement error handling for thread creation and observe the behavior.
 * 5. Modify the program to create threads with different functions and data.
 * 6. Remove the sleep(1) in the for loop and observe the output.
 * 
 *  
 *****************************************************************************/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 4 /* Define the number of threads */

/* Worker thread function */
void* threadfunc(void *arg) {
    int thread_id = *(int*)arg; /* Retrieve the thread's unique identifier */
    printf("Worker Thread %d: I am a worker thread. My job is to print my thread ID.\n", thread_id);
    pthread_exit(0); /* Exit the thread */
}

int main(void) {
    pthread_t workers[NUM_THREADS]; /* Array to hold thread identifiers */
    int data[NUM_THREADS] = {1, 2, 3, 4}; /* Unique data for each thread */
    int i, ret;

    /* Main Thread: Start of the program */
    printf("Main Thread: Starting the program and initializing threads.\n");

    /* Create threads */
    for (i = 0; i < NUM_THREADS; ++i) {
        printf("Main Thread: Creating Worker Thread %d...\n", i + 1);
        ret = pthread_create(&workers[i], NULL, threadfunc, (void*)&data[i]);
        if (ret != 0) {
            fprintf(stderr, "Main Thread: Error creating Worker Thread %d!\n", i + 1);
            return 1;
        }
        sleep(1); /* Sleep for 1 second so that threads are created in order*/
    }

    /* Main Thread: Indicates that all threads have been created */
    printf("Main Thread: All Worker Threads have been created. Now waiting for them to complete.\n");

    /* Wait for all threads to complete */
    for (i = 0; i < NUM_THREADS; ++i) {
        pthread_join(workers[i], NULL);
        printf("Main Thread: Worker Thread %d has completed.\n", i + 1);
    }

    /* Main Thread: End of the program */
    printf("Main Thread: All threads have completed. Program is now exiting.\n");
    return 0;
}
