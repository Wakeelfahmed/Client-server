/******************************************************************************
 * File: ordinary_pipe.c
 * 
 * Purpose:
 * This program demonstrates interprocess communication between a parent and 
 * child process using a unidirectional ordinary pipe. It shows how data can be passed 
 * from the parent to the child process using file descriptors.
 * 
 * Key Concepts Demonstrated:
 * 1. Creating a pipe for interprocess communication using **pipe()**.
 * 2. Forking a child process using **fork()**.
 * 3. Writing data to the pipe in the parent process and reading it in the child.
 * 4. Managing file descriptors for reading and writing in a pipe.
 * 5. Using **wait()** to synchronize the parent with the child process.
 * 6. Proper error handling for system calls such as **pipe()**, **fork()**, and **read()/write()**.
 * 
 * Program Execution:
 * - The parent process writes a message to the pipe.
 * - The child process reads the message from the pipe and displays it.
 * - The parent waits for the child process to complete execution.
 * 
 * Output:
 * - Displays messages indicating process creation, data transfer, and process termination.
 * - Outputs the message sent by the parent and received by the child.
 * 
 * Usage:
 * - Compile: gcc ordinary_pipe.c -o process
 * - Execute: ./process
 * 
 * Learning Objectives for Students:
 * 1. Understand the use of pipes for interprocess communication.
 * 2. Learn how to manage file descriptors in a unidirectional communication setup.
 * 3. Observe the synchronization of parent and child processes using **wait()**.
 * 4. Experiment with modifying the buffer size and message content to understand limitations.
 * 5. Analyze error handling for common system call failures.
 * 
 * Experimentation Instructions:
 * 1. Modify the message in the parent process and observe its reflection in the child process output.
 * 2. Change the buffer size and test behavior with messages of varying lengths.
 * 3. Comment out or remove the **close()** calls to observe the effects of improper file descriptor management.
 * 4. Use additional **write()** or **read()** calls to simulate multiple data exchanges and analyze the behavior.
 * 
 *  
 *****************************************************************************/


#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFFER_SIZE 80
#define READ_END 0
#define WRITE_END 1

int main(void)
{
    char write_msg[BUFFER_SIZE] = "Greetings from parent process";
    char read_msg[BUFFER_SIZE];
    int fd[2];  
    pid_t pid;
    int status;
    
    /* create the pipe */
    if (pipe(fd) == -1) 
    {
        fprintf(stderr, "Pipe failed");
        return 1;
    }
    
    /* fork a child process */
    pid = fork();
    if (pid < 0) 
    {   
        /* error occurred */
        fprintf(stderr, "Fork Failed");
        return 1;
    }
    if (pid > 0) 
    { 
        /* parent process */
        /* close the unused end of the pipe */
        close(fd[READ_END]);
        /* write to the pipe */
        printf("Parent: Writing the message [%s] to pipe\n", write_msg);
        write(fd[WRITE_END], write_msg, strlen(write_msg) + 1);
        /* close the write end of the pipe */
        close(fd[WRITE_END]);
        pid = wait(&status);
        printf("Parent: child(%d) exited...\n", pid);
    }
    else 
    { 
        /* child process */
        /* close the unused end of the pipe */
        close(fd[WRITE_END]);
        /* read from the pipe */
        read(fd[READ_END], read_msg, BUFFER_SIZE);
        printf("Child: Read the messahe [%s] from the pipe\n", read_msg);
        /* close the read end of the pipe */
        close(fd[READ_END]);
    }
    
    return 0;
}
