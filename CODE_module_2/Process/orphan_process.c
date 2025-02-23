/******************************************************************************
 * File: orphan_process.c
 * 
 * Purpose:
 * This program demonstrates the creation of an **orphan process**. An orphan 
 * process occurs when a parent process terminates before its child process. 
 * When the parent terminates, the child process is adopted by the **init** process 
 * (with PID = 1) and continues execution. The child process does not become a zombie,
 * but the parent-child relationship is altered.
 * 
 * Key Concepts Demonstrated:
 * 1. Process creation using **fork()**.
 * 2. Parent process termination using **exit(0)**.
 * 3. Child process entering an infinite loop while the parent terminates.
 * 4. Orphan process lifecycle: When the parent process exits before the child,
 *    the child becomes an orphan and is adopted by the **init** process.
 * 5. Understanding **PID** (Process ID) for the parent and child processes.
 * 6. Viewing the child process as an orphan in the process list using `ps`.
 * 7. Child process continues execution under the **init** process after the parent terminates.
 * 
 * Program Execution:
 * - The parent creates a child process using **fork()**.
 * - The child process enters an infinite loop, simulating long execution.
 * - The parent process prints its PID and terminates using **exit(0)**.
 * - The child process becomes an orphan when the parent exits, and it is adopted by **init**.
 * - The child process will continue running as an orphan process.
 * 
 * Output:
 * - The output will show the **PID** of both the child and parent processes.
 * - The parent process will terminate, while the child will continue execution 
 *   in an infinite loop.
 * - The child will be adopted by the **init** process, and the relationship is changed.
 * 
 * Usage:
 * - Compile: gcc orphan.c -o orphan_process
 * - Execute: ./orphan_process
 * 
 * Learning Objectives for Students:
 * 1. Understand the creation of an orphan process when the parent terminates 
 *    before the child.
 * 2. Learn about the **init** process and its role in adopting orphan processes.
 * 3. Observe the behavior of the parent process terminating while the child remains in execution.
 * 4. Understand the role of the **PID** in distinguishing between the parent and child processes.
 * 5. Discuss the concept of process lifecycle where a child process continues to run 
 *    even after the parent terminates.
 * 6. Observe the child process as an orphan in the process list when **ps aux** is run.
 * 
 * Experimentation Instructions:
 * 1. Observe the output to see the **PID** of the parent and child processes.
 * 2. Notice how the parent terminates while the child process enters an infinite loop.
 * 3. Use the `ps aux` command to check the process list. The child will be shown 
 *    as an orphan, adopted by **init** (PID 1).
 * 4. Modify the program to add conditions for the child process to exit after some time.
 * 5. Change the parent process to wait for the child to complete execution before terminating.
 * 
 * 
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(){
    pid_t pid = fork(); // Create a child process
    if (pid == 0) 
    { // This code is executed by the child process
        printf("Running child (PID: %d), my parent before I become an 'Orphan'is (PPID: %d)\n", getpid(), getppid());          
        printf("Child: Sleeping for 5 seconds to simulate long execution...\n");               
        sleep(5); // Child process sleeps for 5 seconds
        printf("Running child (PID: %d), my parent after I became an 'Orphan'is (PPID: %d)\n", getpid(), getppid());
        printf("Child: Finished execution. Press enter to continue...\n");
    } 
    else 
    { // This code is executed by the parent process
        printf("Terminating Parent, PID = %d, the child becomes orphaned\n", getpid());
        exit(0); // Parent process terminates, the child becomes orphaned
    }

    return 0;
}
