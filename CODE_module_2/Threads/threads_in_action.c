/******************************************************************************
 * File: threads_in_action.c
 * 
 * Purpose:
 * This program demonstrates basic thread creation and management using the 
 * POSIX Threads (pthreads) library in C. Two threads are created, each executing 
 * the same function (`work`) concurrently. The program showcases shared data 
 * access, random number generation, and the use of `pthread_join()` for synchronizing 
 * threads.
 * 
 * Key Concepts Demonstrated:
 * 1. Creating threads using `pthread_create()`.
 * 2. Passing arguments to threads.
 * 3. Synchronizing threads using `pthread_join()`.
 * 4. Accessing shared data between threads.
 * 5. Generating random outputs using a shared accumulator variable (`acc`).
 * 
 * Program Execution:
 * - Two threads are created, each executing the `work` function.
 * - The `work` function increments a shared variable (`acc`) and prints a random 
 *   message from a predefined table of strings.
 * - The parent process waits for both threads to complete using `pthread_join()`.
 * - The final value of the shared accumulator (`acc`) is printed.
 * 
 * Output:
 * - The process ID of each thread (shared as all threads run in the same process).
 * - Random messages from the `table` printed by both threads.
 * - The final value of the shared accumulator (`acc`).
 * 
 * Usage:
 * - Compile: gcc threads_in_action.c -o threads -lpthread
 * - Execute: ./threads
 * 
 * Learning Objectives for Students:
 * 1. Understand how to create and manage threads using the POSIX threads library.
 * 2. Learn how to pass arguments to threads and use shared data.
 * 3. Observe the behavior of threads running concurrently.
 * 4. Understand the role of `pthread_join()` in synchronizing thread completion.
 * 
 * Experimentation Instructions:
 * 1. Modify the number of iterations in the `work` function to observe its effect on output.
 * 2. Add thread-safe mechanisms (e.g., mutexes) to synchronize access to shared data (`acc`).
 * 3. Experiment with creating more threads or using different arguments.
 * 
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>   // For getpid()
#include <pthread.h>  // For POSIX threads
#include <time.h>     // For random seed generation

// Predefined table of strings
const char *table[] = {
    "A man, a plan, a canal: Panama!",
    "No lemons, no melon.",
    "Step on no pets.",
    "Was it a rat I saw?",
    "Dog as a devil deified lived as a god.",
    "Able was I ere I saw Elba.",
    "Yawn! Madonna fan? No damn way!",
    "Go hang a salami. I'm a lasagna hog!"
};

// Constants
const int table_size = 8; // Number of entries in the table
int acc = 0;              // Shared accumulator variable

// Thread function
void *work(void *arg) {
    int count = (long)arg; // Convert argument to the number of iterations

    // Print the process ID (shared by all threads)
    printf("\n\nThread Process ID: %d\n", getpid());

    // Perform the work: increment the accumulator and print random messages
    for (int i = 0; i < count; ++i) {
        acc++; // Increment the shared accumulator
        printf("%s\n", table[(rand() + acc) % table_size]); // Print random message
    }

    return NULL; // Return NULL when the thread finishes
}

int main() {
    pthread_t thread_id1, thread_id2; // Thread identifiers
    srand(time(NULL)); // Seed the random number generator

    // Create the first thread
    pthread_create(&thread_id1, 
                   NULL, // Default thread attributes
                   work, // Thread function
                   (void *)10); // Number of iterations as argument

    // Create the second thread
    pthread_create(&thread_id2, 
                   NULL, 
                   work, 
                   (void *)10);

    // Wait for the first thread to finish
    pthread_join(thread_id1, NULL);

    // Wait for the second thread to finish
    pthread_join(thread_id2, NULL);

    // Print the final value of the shared accumulator
    printf("Final value of accumulator: %d\n", acc);

    return 0; // Exit the program
}
