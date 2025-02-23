/******************************************************************************
 * File: process_hierarchy_waitpid.c
 * 
 * Purpose:
 * This program demonstrates process creation, hierarchy, and synchronization 
 * using **fork()** and **waitpid()** system calls in C. It illustrates:
 * 
 * 1. Creation of multiple child processes.
 * 2. Parent-child relationships and process hierarchy.
 * 3. Waiting for specific child processes using **waitpid()**.
 * 4. Displaying process IDs (PIDs) and parent process IDs (PPIDs).
 * 5. Handling and displaying exit statuses of child processes.
 * 
 * Program Structure:
 * - Parent process creates a first child (Child1).
 * - Child1 creates a second child (Child2).
 * - Each process prints its PID and PPID for clarity.
 * - Processes wait for their respective child processes using **waitpid()** 
 *   and report their exit statuses.
 * 
 * Output:
 * The output shows the execution order of processes, including their IDs, 
 * and verifies that processes exit properly after completing execution.
 * 
 * Usage:
 * - Compile: gcc process_hierarchy_waitpid.c -o process
 * - Execute: ./process
 * 
 * Learning Objectives for Students:
 * 1. Understand how **fork()** creates new processes.
 * 2. Learn to use **waitpid()** for synchronizing process execution.
 * 3. Analyze process relationships through printed PIDs and PPIDs.
 * 4. Observe exit statuses and process termination using **WEXITSTATUS**.
 * 
 * Experimentation Instructions:
 * 1. Observe the process IDs and parent-child relationships in the output.
 * 2. Modify the program to create more child processes and observe the hierarchy.
 * 3. Change the order of process creation and analyze the output.
 * 4. Implement error handling for **fork()** and **waitpid()** failures.
 * 
 *  
 *****************************************************************************/


#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(void) 
{
    printf("\nParent (PID: %d): Start\n", getpid());

    pid_t pid1 = fork(); // Create first child
    if (pid1 == 0) 
    {  
        // First child
        printf("Child1 (PID: %d): I'm the first child, my parent is (PID: %d)\n", getpid(), getppid());

        pid_t pid2 = fork(); // Create second child from child1
        if (pid2 == 0) 
        {  
            // Second child
            printf("Child2 (PID: %d): I'm the second child, my parent is (PID: %d)\n", getpid(), getppid());
            printf("Child2 (PID: %d): I have finished executing\n", getpid());
            exit(0); // Child2 exits
        } 
        else 
        {  
            // Child1 waits specifically for Child2
            int status2;
            pid_t waited2 = waitpid(pid2, &status2, 0);
            printf("Child1 (PID: %d): Waited for Child2 (PID: %d), Status: %d\n", getpid(), waited2, WEXITSTATUS(status2));
            printf("Child1 (PID: %d): I have finished executing\n", getpid());
            exit(0); // Child1 exits
        }
    } 
    else 
    {  
        // Parent process waits for Child1
        int status1;
        pid_t waited1 = waitpid(pid1, &status1, 0);
        printf("Parent (PID: %d): Waited for Child1 (PID: %d), Status: %d\n", getpid(), waited1, WEXITSTATUS(status1));
        printf("Parent (PID: %d): I have finished executing\n", getpid());
    }

    return 0;
}
