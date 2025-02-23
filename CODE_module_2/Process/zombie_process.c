/******************************************************************************
 * File: zombie_process.c
 * 
 * Purpose:
 * This program demonstrates the creation of a **zombie process**. A zombie 
 * process occurs when a child process terminates, but the parent does not 
 * call **wait()** to read the child's exit status, leaving the child process 
 * information in the process table.
 * 
 * Key Concepts Demonstrated:
 * 1. Process creation using **fork()**.
 * 2. Child process termination using **exit()**.
 * 3. Parent process not calling **wait()**, causing the child to become a zombie process.
 * 4. Zombie process lifecycle: The child process remains in the process table, 
 *    occupying system resources until the parent collects its exit status.
 * 5. Understanding **PID** (Process ID) for the parent and child processes.
 * 6. Parent process running an infinite loop while the child terminates and becomes a zombie.
 * 7. Viewing the child process as **defunct** in the process list using `ps`.
 * 
 * Program Execution:
 * - The parent creates a child process using **fork()**.
 * - The child process prints a message, terminates using **exit(0)**.
 * - The parent process prints its PID and enters an infinite loop, 
 *   while the child terminates and becomes a zombie.
 * - The parent does not call **wait()**, so the child becomes a zombie until 
 *   the parent exits.
 * - **Zombie Process**: The child will appear as **defunct** in the process list 
 *   when you run the `ps aux` command.
 * 
 * Output:
 * - The output will show the **PID** of both the child and parent processes.
 * - The parent process will remain stuck in an infinite loop, while the child will terminate.
 * - The child will become a zombie process until the parent process exits.
 * 
 * Usage:
 * - Compile: gcc zombie_process.c -o process
 * - Execute: ./process
 * 
 * Learning Objectives for Students:
 * 1. Understand the creation of a zombie process when a child terminates, 
 *    but the parent does not wait for it.
 * 2. Learn about the **wait()** system call and its role in preventing zombie processes.
 * 3. Observe the behavior of the parent continuing execution while the child becomes a zombie.
 * 4. Understand the role of the **PID** in distinguishing between the parent and child processes.
 * 5. Discuss the concept of process lifecycle where a child process terminates but remains 
 *    in the process table as a zombie.
 * 6. Observe the child process as **defunct** in the process list when **ps aux** is run.
 * 
 * Experimentation Instructions:
 * 1. Observe the output to see the **PID** of the parent and child processes.
 * 2. Notice how the parent continues execution even after the child terminates, 
 *    while the child becomes a zombie.
 * 3. Use the `ps aux` command to check the process list. The child will be shown as **defunct**.
 * 4. Modify the program to include **wait()** in the parent process to avoid zombie processes.
 * 5. Experiment with the infinite loop in the parent by adding conditions for termination.
 * 
 *  
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(){
    pid_t pid = fork(); // Create a child process
    if (pid == 0) { // This code is executed by the child process
        printf("Terminating Child, PID = %d\n", getpid());
        exit(0); // Child process terminates
    } else { // This code is executed by the parent process
        printf("Running Parent, PID = %d\n", getpid());
        
        // Sleep for 30 seconds to allow time for observing the 'Zombie' process. 
        printf("Parent: Sleeping for 30 seconds to observe the 'Zombie' process...\n");
        printf("Open a new terminal and run `ps aux` within 30 seconds to see the 'Zombie' process...\n");
        sleep(30);
        
        // You can now run `ps aux` in a separate terminal to see the child as "defunct"
        printf("Parent: Exiting after 30 seconds\n");
    }
    return 0;
}

