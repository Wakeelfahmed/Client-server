/******************************************************************************
 * File: multiple_fork.c
 * 
 * Purpose:
 * This program demonstrates the creation of multiple child processes using 
 * **fork()** system calls, and shows the hierarchy of processes in the process 
 * tree. The program creates a child process (Child1) and further creates another 
 * child process (Child2) from Child1. It also shows how processes wait for 
 * each other to complete using **wait()** and how to handle child process 
 * return statuses.
 * 
 * Key Concepts Demonstrated:
 * 1. Process creation using **fork()**.
 * 2. Multiple **fork()** calls to create multiple child processes.
 * 3. Process tree hierarchy and parent-child relationships.
 * 4. Synchronization between processes using **wait()** to wait for specific 
 *    child processes to finish.
 * 5. Process exit status and how the parent process retrieves it.
 * 6. Understanding the flow of execution and process IDs in a multi-process 
 *    environment.
 * 
 * Program Execution:
 * - The parent process creates **Child1** using **fork()**.
 * - Child1 creates **Child2**, forming a process hierarchy.
 * - Each child prints its status and execution sequence.
 * - The parent waits for **Child1** to finish, and **Child1** waits for 
 *   **Child2** to complete.
 * 
 * Output:
 * - Prints the creation of processes, including their order and execution 
 *   status.
 * - Shows how the process tree is built with parent-child relationships.
 * 
 * Usage:
 * - Compile: gcc multiple_fork.c -o process 
 * - Execute: ./process
 * 
 * Learning Objectives for Students:
 * 1. Understand how multiple processes can be created using multiple **fork()** calls.
 * 2. Visualize the process tree hierarchy where child processes are created 
 *    by parent processes.
 * 3. Learn how to manage multiple processes and wait for specific child processes 
 *    to finish using **wait()**.
 * 4. Analyze the **exit status** of child processes and how the parent retrieves it.
 * 
 * Experimentation Instructions:
 * 1. Observe the process IDs (PIDs) of each child and the parent, and understand 
 *    how the process tree develops.
 * 2. Check how each process prints its message, especially how Child2 is created 
 *    by Child1.
 * 3. Modify the program to create more child processes and observe the process tree.
 *
 *
 *****************************************************************************/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(void) 
{
    printf("\nParent: Start\n");
    int status; // Exit status of child.
    
    pid_t pid1 = fork(); // Create first child
    if (pid1 == 0) 
    {  // First child
        printf("Child1: I'm the first child (PID: %d)\n", getpid());
        
        pid_t pid2 = fork(); // Create second child from child1
        if (pid2 == 0) 
        {  // Second child
            printf("Child2: I'm the second child (PID: %d), created by Child1\n", getpid());
            printf("Child2: I have finished executing\n");
            exit(0); // Child2 exits
        } 
        else 
        {  // Child1 continues here
            wait(&status); // Wait for child2 to finish
            printf("Child2 finished executing with return status[%d] \n", status);
            printf("Child1: I have finished executing\n");
            exit(0); // Child1 exits           
        }
        
    } 
    else 
    {  // Parent process
        wait(&status); // Wait for the first child to finish
        printf("Child1 finished executing with return status[%d] \n", status);
        printf("Parent: I have finished executing\n");
    }

   // printf("This statement is accessible to all processes\n");
 
    return 0;
}
