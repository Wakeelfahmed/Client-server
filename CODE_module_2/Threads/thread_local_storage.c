/******************************************************************************
 * File: thread_local_storage.c
 * 
 * Purpose:
 * This program demonstrates the concept of **Thread Local Storage (TLS)** in C using 
 * the POSIX thread library (**pthread**). It highlights the difference between 
 * global variables and thread-local variables, showing how each thread maintains its 
 * own copy of a variable independent of other threads.
 * 
 * Key Concepts Demonstrated:
 * 1. Creation of multiple threads using **pthread_create()**.
 * 2. Using the **__thread** storage class specifier to declare thread-local variables.
 * 3. Observing the behavior of thread-local variables versus global variables.
 * 4. Synchronization of the main thread with worker threads using **pthread_join()**.
 * 
 * Program Execution:
 * - Two threads are created, each modifying a thread-local variable.
 * - Each thread prints its own independent value of the thread-local variable.
 * - The main thread waits for all threads to complete using **pthread_join()**.
 * 
 * Output:
 * - Each thread outputs its local value of the variable, demonstrating thread-local storage.
 * - The main process outputs a message indicating program progress and termination.
 * 
 * Usage:
 * - Compile: gcc thread_local_storage.c -o tls -lpthread
 * - Execute: ./tls
 * 
 * Learning Objectives for Students:
 * 1. Understand how to create and manage multiple threads in a C program.
 * 2. Learn how thread-local storage enables each thread to have its own instance of a variable.
 * 3. Observe the difference between globaland thread-local variables.
 * 4. Understand basic synchronization using **pthread_join()**.
 * 
 * Experimentation Instructions:
 * 1. Run the program and observe the independent values of the thread-local variable.
 * 2. Replace the **__thread** variable with a global variable and observe the difference.
 * 3. Experiment by adding more threads and modifying the thread-local variable.
 * 4. Modify the program to use a local variable and observe the behavior.
 * 
 *  
 *****************************************************************************/

#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>


__thread int thread_var = 5;   // Thread-local variable
int global_var = 10; // Global variable

/* Function prototypes */
void* worker1(void* arg);
void* worker2(void* arg);

int main(){
    pthread_t pid1, pid2;

    /* Create two threads */
    pthread_create(&pid1, NULL, worker1, NULL);
    sleep(1); // Sleep for 1 second to ensure thread 1 starts first
    pthread_create(&pid2, NULL, worker2, NULL);

    /* Wait for threads to finish */
    pthread_join(pid1, NULL);
    pthread_join(pid2, NULL);

    return 0;
}

/* Worker thread 1 */
void* worker1(void* arg){
    printf("Worker 1: 'thread_var' value before modification = %d\n", thread_var); // Print thread-local variable
    printf("Worker 1: 'global_var' value before modification = %d\n", global_var); // Print global variable
    thread_var++; // Modify thread-local variable
    global_var++; // Modify global variable
    printf("Worker 1: 'thread_var' value after modification = %d\n", thread_var); // Print thread-local variable
    printf("Worker 1: 'global_var' value after modification = %d\n", global_var); // Print global variable
    pthread_exit(0); // Exit the thread
}

/* Worker thread 2 */
void* worker2(void* arg){
    printf("Worker 2: 'thread_var' value before modification = %d\n", thread_var); // Print thread-local variable
    printf("Worker 2: 'global_var' value before modification = %d\n", global_var); // Print global variable
    thread_var += 2; // Modify thread-local variable
    global_var +=2; // Modify global variable
    printf("Worker 2: 'thread_var' value after modification = %d\n", thread_var); // Print thread-local variable
    printf("Worker 2: 'global_var' value after modification = %d\n", global_var); // Print global variable
    pthread_exit(0); // Exit the thread
}
