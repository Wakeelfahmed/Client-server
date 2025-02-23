/******************************************************************************
 * File: process_creation.c
 * 
 * Purpose:
 * This program demonstrates the creation of a child process using **fork()** 
 * and execution of a new program in the child process using **execlp()** system call
 * 
 * Key Concepts Demonstrated:
 * 1. Process creation with **fork()**.
 * 2. Replacing the child process code with a new program using **execlp()**.
 * 3. Trying variations of **execlp()** with arguments.
 * 4. Parent process waiting for the child to complete execution using **wait()**.
 * 6. Handling errors if **fork()** or **exec()** fails.
 * 7. Displaying process behavior and exit statuses.
 * 
 * Program Execution:
 * - The parent process creates a child process using **fork()**.
 * - The child process replaces its code with the **ls** command using **execlp()**.
 * - The parent waits for the child to complete and displays its exit status.
 * - Variations of **execlp()**can be tested by uncommenting specific lines.
 * 
 * Output:
 * - Prints messages indicating process creation, execution, and termination.
 * - Displays directory contents (output of the **ls** command).
 * 
 * Usage:
 * - Compile: gcc process_creation.c -o process
 * - Execute: ./process
 * 
 * Learning Objectives for Students:
 * 1. Understand the use of **fork()** for process creation.
 * 2. Learn how **execlp()** replace the process image with a new program.
 * 3. Observe process synchronization using **wait()**.
 * 4. Analyze error handling in process creation and program execution.
 * 5. Experiment with variations of **execlp()** to execute programs.
 * 
 * 
 * Experimentation Instructions:
 * 1. Uncomment the **execlp()** calls with different parameters to observe behavior:
 *      - Single command execution: execlp("/bin/ls", "ls", NULL);
 *      - Command with arguments: execlp("/bin/ls", "ls", "-a", NULL);
 *      - Command targeting a directory: execlp("/bin/ls", "ls", "-a", "/home", NULL);
 * 2. Observe how the parent waits for the child and displays exit status.
 *   
 * 
 *****************************************************************************/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(void) {
    pid_t pid;  // PID of child. pid_t is the same as int
    int status; // Exit status of child.
  
    printf("\nParent: calling fork()\n");
    pid = fork(); // New child process starts executing code from here.
    //printf("This line is printed by both parent and child processes\n");
    if(pid < 0) { // No child process created.
        printf("\nParent: fork failed....\n");
        return 1; // Parent process dies.
    }
    else if(pid == 0) {
        // This code only executed by the new child process.
        printf("\nChild: now running the same program as parent, calling exec()\n");

        // Try execlp() to execute 'ls' without arguments.
        printf("\nChild: executing execlp()\n");
        printf("Displaying directory contents using 'ls' command:\n");
        execlp("/bin/ls", "ls", NULL); // Default example

        // Uncomment the following lines to test variations of execlp() and execvp():
        // execlp("/bin/ls", "ls", "-a", NULL);                // With options
        // execlp("/bin/ls", "ls", "-a", "/home", NULL);       // With directory
       

        // If exec succeeds, the new child process runs the new program.
        // The code below is only executed if exec() fails.
        printf("\nChild: exec failed, die...\n");
        exit(1); // Child process dies after failed exec.
    } else {
        // This code only executed by the parent process.
        printf("\nParent: now sleeping and waiting for child %d to end\n", pid);
        wait(&status);
        printf("\nParent: finished waiting for child, child is dead\n");
        printf("\nParent: child exit status is %d\n\n", status);
        return 0; // Parent process dies.
    }

    return 0;
}
