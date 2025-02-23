/******************************************************************************
 * File: process_fork_exit.c
 * 
 * Purpose:
 * This program demonstrates the use of the `fork()` system call to create 
 * a child process, along with the behavior of different termination methods 
 * (`exit()`, `_exit()`, and `return`) in the child process. It also shows 
 * how the parent process waits for the child process to terminate using `wait()`.
 * 
 * Key Concepts Demonstrated:
 * 1. Creation of a child process using `fork()`.
 * 2. The difference between `exit()`, `_exit()`, and `return` when used 
 *    in the child process.
 * 3. Use of the `wait()` function in the parent process to collect the 
 *    termination status of the child process.
 * 4. Understanding `PPID` (Parent Process ID) in the child process.
 * 5. Observing the differences in I/O behavior when `exit()` and `_exit()` are used.
 * 
 * Program Execution:
 * - The parent process creates a child process using `fork()`.
 * - The child process identifies the parent's PID using the `getppid()` function.
 * - The child process terminates using `_exit()` (or `exit()` or `return`).
 * - The parent process waits for the child to terminate and retrieves its exit status.
 * - The program demonstrates the differences in behavior based on the termination method.
 * 
 * Output:
 * - The child process prints the parent PID and a message before terminating.
 * - The parent process prints its message and waits for the child to complete.
 * - If `_exit()` is used in the child, buffered output in the child may not be flushed.
 * - If `exit()` or `return` is used in the child, buffered output will be flushed.
 * 
 * Usage:
 * - Compile: gcc process_fork_exit.c -o exit
 * - Execute: ./exit
 * 
 * Learning Objectives for Students:
 * 1. Understand the use of `fork()` to create child processes.
 * 2. Learn how `exit()`, `_exit()`, and `return` affect process termination 
 *    and buffered output.
 * 3. Observe how the parent process collects the child's exit status using `wait()`.
 * 4. Explore how standard I/O behaves differently based on the termination method.
 * 
 * Experimentation Instructions:
 * 1. Modify the termination method in the child process to `exit()`, `_exit()`, or `return` 
 *    and observe the differences in behavior and output.
 * 2. Use `printf()` before termination to observe buffering behavior.
 * 3. Modify the parent process to analyze the child's exit status from `wait()`.
 * 
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>   // For fork(), _exit(), and standard POSIX functions
#include <sys/wait.h> // For wait()

// Callback function to be executed upon process termination.
void my_last_call()
{
    printf("This is atexit() callback function, executed on process termination.\n");
     printf("atexit(): buffer is flushed flushed...\n");
}

int main()
{
    int pid; // Variable to store the PID of the child process

    // Register the `my_last_call` function to be executed upon process exit.
    atexit(my_last_call);

    // Create a child process using fork()
    pid = fork();

    if (pid == 0) // Child process
    {
        int ppid = getppid(); // Get the parent's process ID
        printf("Child process: Parent's PID is %d.\n", ppid);

        // Demonstrate buffered output
        printf("Child process: This message is buffered but not flushed yet.\n");

        /*
         * Terminate the child process using one of the following methods:
         * 1. `exit(100)`: Flushes buffers and calls atexit() functions.
         * 2. `_exit(10)`: Immediately terminates without flushing buffers or calling atexit() functions.
         * 3. `return 0`: Similar to `exit()` in this context.
         */

        // Uncomment ONE of the following termination methods to observe behavior:

        /* exit(100); */   // Flushes buffers and executes atexit() functions
        _exit(10);         // Does not flush buffers or execute atexit() functions
        /* return 0; */    // Similar to `exit()`
    }
    else if (pid > 0) // Parent process
    {
        int status; // Variable to store the child's exit status

        // Print a message from the parent process
        printf("Parent process: Waiting for the child process to terminate...\n");

        // Wait for the child process to complete and retrieve its status
        wait(&status);

        // Check if the child terminated normally and print its exit status
        if (WIFEXITED(status))
        {
            printf("Parent process: Child exited with status %d.\n", WEXITSTATUS(status));
        }

        // Print a completion message
        printf("Parent process: Done.\n");
    }
    else // Fork failed
    {
        fprintf(stderr, "Fork failed.\n");
        return -1;
    }

    return 0; // Return success
}
