/******************************************************************************
 * File:graceful_thread_termination.c
 * 
 * Purpose:
 * This program demonstrates the creation, management, and deferred termination 
 * of threads in a C program using the POSIX thread library (**pthread**). 
 * Selected threads running infinitely are terminated in a controlled and 
 * graceful manner using signals (SIGINT, SIGTERM) and thread cancellation 
 * points (`pthread_testcancel()`).
 * 
 * Key Concepts Demonstrated:
 * 1. Creation of multiple threads using **pthread_create()**.
 * 2. Passing unique data to each thread using arguments.
 * 3. Gracefully terminating specific threads using **pthread_kill()** with signals.
 * 4. Incorporating cancellation points in long-running tasks using **pthread_testcancel()**.
 * 5. Synchronization of the main thread with worker threads using **pthread_join()**.
 * 6. Error handling during thread creation and termination.
 * 
 * Program Execution:
 * - Four threads are created, with two running infinite loops.
 * - The main process sends termination signals to specific threads running 
 *   infinitely, allowing them to terminate gracefully.
 * - The main process waits for the remaining threads to complete using **pthread_join()**.
 * 
 * Output:
 * - Each thread outputs a message indicating its unique identifier.
 * - Selected threads handle the termination signals and exit gracefully.
 * - The main process outputs a completion message.
 * 
 * Usage:
 * - Compile: gcc graceful_threads_termination.c -o gthread -lpthread
 * - Execute: ./gthread
 * 
 * Experimentation Instructions:
 * 1. Run the program and observe the output of each thread and the main process.
 * 2. Modify the number of threads and experiment with different data passed to each thread.
 * 3. Experiment with different signals for terminating threads.
 * 4. Observe the behavior of threads when signals are not handled.
 * 5. Increase the sleep time in the infinite loop threads to observe the graceful termination.
 * 
 *  
 *****************************************************************************/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#define NUM_THREADS 4 /* Define the number of threads */


/* Thread-local storage for thread ID */
__thread int thread_id;

/* Signal handler for threads */
void signal_handler(int signum) {
    if (signum == SIGINT)
        printf("Thread %d received signal %d (SIGINT). Preparing to exit gracefully...\n", thread_id, signum);
    else if (signum == SIGTERM)
        printf("Thread %d received signal %d (SIGTERM). Preparing to exit gracefully...\n", thread_id, signum);
    
    pthread_exit(NULL); /* Exit the thread gracefully */
}


/* Signal handler for threads */
//void signal_handler(int signum) 
//{
  //  if(signum == SIGINT)
    //    printf("Thread received signal %d (SIGINT). Preparing to exit gracefully...\n", signum);
    //else if(signum == SIGTERM)
      //  printf("Thread received signal %d (SIGTERM). Preparing to exit gracefully...\n", signum);
    
   // pthread_exit(NULL); /* Exit the thread gracefully */
//}

/* Infinite Loop Thread Function */
void* infinite_threadfunc(void* arg) 
{
    thread_id = *(int*)arg; /* Retrieve the thread's unique identifier */
    
    /* Register signal handler for SIGINT and SIGTERM */
    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);

    printf("Worker Thread %d: Running infinitely. Waiting for termination signal...\n", thread_id);
    while (1) 
    {
        /* Cancellation point */
        pthread_testcancel();
        printf("Thread %d: Doing some work...\n", thread_id);
        sleep(1); /* Simulate work */
    }
}

/* Finite Task Thread Function */
void* finite_threadfunc(void* arg)
{
    thread_id = *(int*)arg; /* Retrieve the thread's unique identifier */
    printf("Worker Thread %d: Performing a finite task.\n", thread_id);
    printf("Thread %d: Doing some work...\n", thread_id);
    sleep(1); /* Simulate work */
    printf("Worker Thread %d: Task completed.\n", thread_id);
    pthread_exit(0);
}

int main(void) 
{
    pthread_t workers[NUM_THREADS]; /* Array to hold thread identifiers */
    int data[NUM_THREADS] = {1, 2, 3, 4}; /* Unique data for each thread */
    int i, ret;

    printf("Main Thread: Starting the program and initializing threads.\n");

    /* Create threads */
    ret = pthread_create(&workers[0], NULL, infinite_threadfunc, (void*)&data[0]);
    printf("Main Thread: Thread 1 will run infinitely.\n");
    if (ret != 0) 
    {
        fprintf(stderr, "Error creating Worker Thread 1!\n");
        return 1;
    }

    ret = pthread_create(&workers[1], NULL, finite_threadfunc, (void*)&data[1]);
    printf("Main Thread: Thread 2 will run finitely.\n");
    if (ret != 0) 
    {
        fprintf(stderr, "Error creating Worker Thread 2!\n");
        return 1;
    }

    ret = pthread_create(&workers[2], NULL, infinite_threadfunc, (void*)&data[2]);
    printf("Main Thread: Thread 3 will run infinitely.\n");
    if (ret != 0) 
    {
        fprintf(stderr, "Error creating Worker Thread 3!\n");
        return 1;
    }

    ret = pthread_create(&workers[3], NULL, finite_threadfunc, (void*)&data[3]);
    printf("Main Thread: Thread 4 will run finitely.\n");
    if (ret != 0) 
    {
        fprintf(stderr, "Error creating Worker Thread 4!\n");
        return 1;
    }

    /* Allow threads to start */
    sleep(3);

    /* Send termination signals to infinite loop threads */
    printf("Main Thread: Sending SIGTERM to Worker Threads 1 and 3.\n");
    pthread_kill(workers[0], SIGTERM);
    pthread_kill(workers[2], SIGTERM);
    //printf("Main Thread: Sending SIGINT to Worker Threads 1 and 3.\n");
    //pthread_kill(workers[0], SIGINT);
    //pthread_kill(workers[2], SIGINT);

    /* Wait for all threads to complete */
    for (i = 0; i < NUM_THREADS; ++i)
    {
        pthread_join(workers[i], NULL);
        if (i == 0 || i == 2)
            printf("Main Thread: Worker Thread %d terminated gracefully.\n", i + 1);
        else
            printf("Main Thread: Worker Thread %d completed normally.\n", i + 1);
    }

    printf("Main Thread: All threads have completed. Program is now exiting.\n");
    return 0;
}
