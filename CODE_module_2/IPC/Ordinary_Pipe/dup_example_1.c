/******************************************************************************
 * File: pipes_dup.c
 *
 * Purpose:
 * This program demonstrates inter-process communication (IPC) using POSIX pipes
 * in a C program. It creates two child processes: one executes the `ls` command
 * and writes its output to a pipe, and the other reads the pipe and processes
 * the output using the `cat` command. This showcases basic usage of pipes for
 * connecting the output of one process to the input of another.
 *
 * Key Concepts Demonstrated:
 * 1. Creation of pipes using the **pipe()** system call.
 * 2. Forking child processes to execute commands.
 * 3. Redirecting standard input/output using **dup()**.
 * 4. Using **execlp()** to execute external commands.
 * 5. Synchronization of parent and child processes using **wait()**.
 *
 * Program Execution:
 * - The parent process creates a pipe.
 * - Two child processes are forked:
 *   - The first child process executes the `ls` command and writes its output
 *     to the writing end of the pipe.
 *   - The second child process reads from the reading end of the pipe and
 *     executes the `cat` command to process the output.
 * - The parent process closes the pipe and waits for the child processes to finish.
 *
 * Usage:
 * - Compile: gcc pipe_dup.c -o pipes
 * - Execute: ./pipes
 *
 * Learning Objectives for Students:
 * 1. Understand how to create and use pipes for IPC in C.
 * 2. Learn how to use **dup()** to redirect standard input/output.
 * 3. Observe how to execute external commands with **execlp()**.
 *
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) 
{
    int pid;                    // Variable to hold process ID
    char buffer[1024];          // Buffer for reading/writing data (unused in this program)
    int descriptor[2];          // Array to hold the two ends of the pipe

    // Create a pipe. The pipe() system call creates a pair of file descriptors:
    // descriptor[0] - the reading end of the pipe
    // descriptor[1] - the writing end of the pipe
    if (pipe(descriptor) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Fork the first child process
    if (fork() == 0) {
        // Child process 1: Executes the `ls` command

        printf("[Child 1] Using dup to map stdout to the writing end of the pipe.\n");

        // Close standard output (stdout)
        close(1);
        // Redirect stdout to the writing end of the pipe
        dup(descriptor[1]);
        // Close the reading end of the pipe (not used by this child)
        close(descriptor[0]);

        printf("[Child 1] Executing 'ls' command.\n");
        // Execute the `ls` command
        execlp("ls", "ls", NULL);

        // If execlp fails, print an error and exit
        perror("execlp");
        exit(EXIT_FAILURE);
    }

    // Fork the second child process
    if (fork() == 0) {
        // Child process 2: Executes the `cat` command

        printf("[Child 2] Using dup to map stdin to the reading end of the pipe.\n");

        // Close the writing end of the pipe (not used by this child)
        close(descriptor[1]);
        // Close standard input (stdin)
        close(0);
        // Redirect stdin to the reading end of the pipe
        dup(descriptor[0]);

        printf("[Child 2] Executing 'cat' command.\n");
        // Execute the `cat` command
        execlp("cat", "cat", NULL);

        // If execlp fails, print an error and exit
        perror("execlp");
        exit(EXIT_FAILURE);
    }

    // Parent process: Close both ends of the pipe (no longer needed)
    printf("[Parent] Closing both ends of the pipe.\n");
    close(descriptor[1]);
    close(descriptor[0]);

    // Wait for both child processes to finish
    printf("[Parent] Waiting for child processes to finish.\n");
    wait(NULL);
    wait(NULL);

    // Print a message indicating that the processes have finished
    printf("[Parent] All child processes have finished.\n");

    return 0;
}
