/******************************************************************************
 * File: asynchronous_thread_termination.c
 * 
 * Purpose:
 * This program demonstrates the creation, management, and ASYNCHRONUS termination of threads 
 * in a C program using the POSIX thread library (**pthread**). Selected threads 
 * running infinitely can be canceled by the main thread.
 * 
 * Key Concepts Demonstrated:
 * 1. Creation of multiple threads using **pthread_create()**.
 * 2. Passing unique data to each thread using arguments.
 * 3. Canceling specific threads using **pthread_cancel()**.
 * 4. Synchronization of the main thread with worker threads using **pthread_join()**.
 * 5. Error handling during thread creation and cancellation.
 * 
 * Program Execution:
 * - Four threads are created, with two running infinite loops.
 * - The main process cancels the threads running infinitely.
 * - The main process waits for the remaining threads to complete using **pthread_join()**.
 * 
 * Output:
 * - Each thread outputs a message indicating its unique identifier.
 * - Selected threads are canceled, and the rest complete normally.
 * - The main process outputs a completion message.
 * 
 * Usage:
 * - Compile: gcc terminate_threads.c -o athread -lpthread
 * - Execute: ./athread
 * 
 * Experimentation Instructions:
 * 1. Run the program and observe the output of each thread and the main process.
 * 2. Modify the number of threads and experiment with different data passed to each thread.
 * 3. Remove sleep() calls to observe the unpredictable execution behavior of threads.
 * 4
 * 
 * 
 *  
 *****************************************************************************/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define NUM_THREADS 4 /* Define the number of threads */

/* Infinite Loop Thread Function */
void* infinite_threadfunc(void *arg) 
{
    int thread_id = *(int*)arg; /* Retrieve the thread's unique identifier */
    printf("Worker Thread %d: I am running infinitely and will be ASYNCHRONOUSLY terminated...\n", thread_id);
    while (1) 
    {
        /* Infinite loop simulating a long-running task */
        printf("Thread %d: Doing some infininte work...\n", thread_id);
        sleep(1); /* Sleep for 0.5 seconds to simulate work */
    }
}

/* Finite Task Thread Function */
void* finite_threadfunc(void *arg) 
{
    int thread_id = *(int*)arg; /* Retrieve the thread's unique identifier */
    printf("Worker Thread %d: I am performing a finite task.\n", thread_id);
    printf("Thread %d: Doing some fininte work...\n", thread_id);
    sleep(1); /* Simulate work */
    printf("Worker Thread %d: I have completed my task and will exit VOLUNTARILY...\n", thread_id);
    pthread_exit(0);
}

int main(void) 
{
    pthread_t workers[NUM_THREADS]; /* Array to hold thread identifiers */
    int data[NUM_THREADS] = {1, 2, 3, 4}; /* Unique data for each thread */
    int i, ret;

    /* Main Thread: Start of the program */
    printf("Main Thread: Starting the program and initializing 4 threads.\n");

    /* Create threads */
    ret = pthread_create(&workers[0], NULL, infinite_threadfunc, (void*)&data[0]);
    printf("Main Thread: Thread 1 will run infinitely.... I am going to ASYNCHRONOUSLY terminate it\n");
    if (ret != 0) 
    {
        fprintf(stderr, "Error creating Worker Thread 1!\n");
        return 1;
    }
    sleep(1); /* Sleep for 1 second to allow other threads to start */

    ret = pthread_create(&workers[1], NULL, finite_threadfunc, (void*)&data[1]);
    printf("Main Thread: Thread 2 will run finitely.... Its going to complete its task and exit VOLUNTARILY\n");
    if (ret != 0) 
    {
        fprintf(stderr, "Error creating Worker Thread 2!\n");
        return 1;
    }
    sleep(1); /* Sleep for 1 second to allow other threads to start */

    ret = pthread_create(&workers[2], NULL, infinite_threadfunc, (void*)&data[2]);
    printf("Main Thread: Thread 3 will run infinitely.... I am going to ASYNCHRONOUSLY terminate it\n");
    if (ret != 0) 
    {
        fprintf(stderr, "Error creating Worker Thread 3!\n");
        return 1;
    }
    sleep(1); /* Sleep for 1 second to allow other threads to start */

    ret = pthread_create(&workers[3], NULL, finite_threadfunc, (void*)&data[3]);
    printf("Main Thread: Thread 4 will run finitely.... Its going to complete its task and exit VOLUNTARILY\n");
    if (ret != 0) 
    {
        fprintf(stderr, "Error creating Worker Thread 4!\n");
        return 1;
    }
    sleep(1); /* Sleep for 1 second to allow other threads to start */
    
    /* Main Thread: Cancel infinite loop threads using pthread_cancel()*/
    printf("Main Thread: Canceling Worker Threads 1 & 3 using pthread_cancel().\n");
    pthread_cancel(workers[0]);
    pthread_cancel(workers[2]);
    
     
    /* Wait for all threads to complete */
    for (i = 0; i < NUM_THREADS; ++i) 
    {
        pthread_join(workers[i], NULL);
        if(i==0 || i==2)
            printf("Main Thread: Worker Thread %d was ASYNCHRONOUSLY terminated...\n", i + 1);
        else
            printf("Main Thread: Worker Thread %d has successfully completed and VOLUNTARILY exited.\n", i + 1);
    }

    /* Main Thread: End of the program */
    printf("Main Thread: All threads have completed. Program is now exiting.\n");
    return 0;
}
