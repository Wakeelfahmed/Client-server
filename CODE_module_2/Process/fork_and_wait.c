/******************************************************************************
 * File:fork_and_wait.c
 * 
 * Purpose:
 * This program demonstrates the use of the `fork()` system call to create 
 * a child process and the use of `wait()` to retrieve the exit status of 
 * the child process in the parent process. It also compares the behavior of 
 * different termination methods: `exit()`, `_exit()`, and `return`.
 * 
 * Key Concepts Demonstrated:
 * 1. Process creation using `fork()`.
 * 2. Handling child process termination and retrieving its exit status using `wait()` 
 *    and `WEXITSTATUS()`.
 * 3. Understanding the behavior of `exit()`, `_exit()`, and `return` in terms of 
 *    process termination and result communication.
 * 4. Differentiating between normal and abnormal process termination using `WIFEXITED()`.
 * 
 * Program Execution:
 * - The parent process creates a child process using `fork()`.
 * - The child process executes its code and terminates using `exit()`, `_exit()`, 
 *   or `return` with a status value (19).
 * - The parent process waits for the child to terminate, checks if it exited 
 *   normally using `WIFEXITED()`, and retrieves its exit status using `WEXITSTATUS()`.
 * 
 * Output:
 * - The child process prints its message before terminating.
 * - The parent process prints the exit status of the child process and its own message.
 * 
 * Usage:
 * - Compile: gcc process_exit_status.c -o process
 * - Execute: ./process
 * 
 * Learning Objectives for Students:
 * 1. Understand the role of `fork()` in creating a new process.
 * 2. Learn how the parent process collects the exit status of the child process.
 * 3. Observe the differences between `exit()`, `_exit()`, and `return` in process termination.
 * 4. Explore the usage of macros like `WIFEXITED()` and `WEXITSTATUS()` to handle process status.
 * 
 * Experimentation Instructions:
 * 1. Modify the child process termination method by uncommenting `exit(19)`, `_exit(19)`, 
 *    or `return 19` to observe their behavior.
 * 2. Add buffered I/O (e.g., `printf()`) in the child process to observe differences 
 *    between `exit()` and `_exit()` in handling standard I/O.
 * 3. Experiment with abnormal process termination to observe how the parent detects 
 *    such cases (e.g., by dividing by zero or using `abort()` in the child process).
 * 
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>   // For fork(), _exit(), and standard POSIX functions
#include <sys/wait.h> // For wait()

int main()
{
    int pid; // Variable to store the PID of the child process

    // Create a child process using fork()
    pid = fork();

    if (pid == 0) // Child process
    {
        // Child process code
        printf("In child Process.\n");

        /*
         * Terminate the child process using one of the following methods:
         * 1. `exit(19)`: Flushes I/O buffers and executes functions registered with `atexit()`.
         * 2. `_exit(19)`: Immediately terminates without flushing I/O buffers or calling `atexit()` functions.
         * 3. `return 19`: Similar to `exit(19)` when used in main().
         */

        /* exit(19); */   // Uncomment this to use exit()
        /* _exit(19); */  // Uncomment this to use _exit()
        return 19;        // Default behavior: return from main()
    }
    else if (pid > 0) // Parent process
    {
        int status; // Variable to store the child's termination status

        // Wait for the child process to terminate
        wait(&status);

        // Check if the child exited normally
        if (WIFEXITED(status))
        {
            // Print the exit status of the child process
            printf("Child exited with status = %d\n", WEXITSTATUS(status));
        }

        // Print a message from the parent process
        printf("In parent Process.\n");
    }
    else // Fork failed
    {
        fprintf(stderr, "Fork failed.\n");
        return -1; // Exit with an error code
    }

    return 0; // Exit successfully
}
