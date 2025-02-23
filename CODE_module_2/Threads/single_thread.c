/******************************************************************************
 * File: single_thread.c
 * 
 * Purpose:
 * This program demonstrates the creation and execution of a thread in C using 
 * the POSIX thread library (**pthread**). A single thread is created to compute 
 * the sum of integers from 1 to a user-specified number, demonstrating basic 
 * concepts of multithreading and shared data.
 * 
 * Key Concepts Demonstrated:
 * 1. Thread creation using **pthread_create()**.
 * 2. Thread attributes initialization using **pthread_attr_init()**.
 * 3. Synchronization of the main thread with the worker thread using 
 *    **pthread_join()**.
 * 4. Shared data between threads and the main process.
 * 5. Error checking for command-line arguments and validating input.
 * 
 * Program Execution:
 * - The user provides a positive integer as a command-line argument.
 * - The program creates a thread that computes the sum of integers from 1 to 
 *   the specified number.
 * - The main thread waits for the worker thread to complete, ensuring correct 
 *   execution order.
 * - The computed sum is displayed to the user after the thread finishes execution.
 * 
 * Output:
 * - The program outputs the computed sum of integers from 1 to the specified number.
 * 
 * Usage:
 * - Compile: gcc thread_sum.c -o thread_sum -lpthread
 * - Execute: ./thread_sum <positive_integer>
 * 
 * Learning Objectives for Students:
 * 1. Understand the creation and lifecycle of threads in a C program.
 * 2. Learn about the role of **pthread_create** and **pthread_join** in thread management.
 * 3. Observe how threads can interact with shared data.
 * 4. Practice error handling for command-line input validation.
 * 
 * Experimentation Instructions:
 * 1. Provide a valid positive integer as input and observe the computed sum.
 * 2. Test the program with invalid input (negative numbers or non-integer values) 
 *    to see error handling in action.
 * 3. Modify the program to create multiple threads, each handling a part of the 
 *    computation.
 * 
 * 
 *****************************************************************************/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int sum; /* Shared data for the thread */

void *runner(void *param); /* The thread function */

int main(int argc, char *argv[]) {
    pthread_t tid; /* Thread identifier */
    pthread_attr_t attr; /* Thread attributes */
    int n;

    
    /* Validate the command-line arguments */
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <integer value>\n", argv[0]);
        return -1;
    }

    n = atoi(argv[1]);
    if (n < 0) {
        fprintf(stderr, "Argument %d must be non-negative\n", n);
        return -1;
    }

    printf("Main Thread: Creating a worker thread to compute the sum of integers from 1 to %d.\n", n);

    /* Initialize thread attributes */
    pthread_attr_init(&attr);

    /* Create the thread */
    printf("Main Thread: Creating a worker thread...\n");
    pthread_create(&tid, &attr, runner, argv[1]);

    /* Wait for the thread to complete */
    printf("Main Thread: Waiting for the worker thread to complete...\n");
    pthread_join(tid, NULL);

    /* Output the result */
    printf("Main Thread: Worker thread has completed.\n");
    printf("Main Thread: sum = %d\n", sum);
    return 0;
}

//The thread will begin control in this function 
void *runner(void *param) {
    int i, upper = atoi((char *)param);
    printf("Worker Thread: I am the single worker thread. Computing the sum of integers from 1 to %d.\n", upper);
    sum = 0;

    for (i = 1; i <= upper; i++) {
        sum += i;
    }

    pthread_exit(0);
}
