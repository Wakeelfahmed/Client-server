/******************************************************************************
 * File: process_with_timeout.c
 *
 * Purpose:
 * This program demonstrates process creation, inter-process communication (IPC),
 * and process termination in a C program. It uses pipes to redirect the stdin
 * and stdout of a child process and employs a signal-based mechanism to ensure
 * the child process is terminated if it exceeds a specified time limit.
 *
 * Key Concepts Demonstrated:
 * 1. Process creation using **fork()**.
 * 2. Redirection of stdin and stdout using **pipe()** and **dup2()**.
 * 3. Usage of **execlp()** to execute a command in the child process.
 * 4. Signal handling for managing process timeouts using **SIGALRM**.
 * 5. Synchronization of the parent process with the child process using **waitpid()**.
 *
 * Program Execution:
 * - The parent process creates a child process and sets up pipes for IPC.
 * - The child process executes a user-specified program with optional arguments.
 * - The parent redirects input to the child and retrieves its output.
 * - If the child process does not complete within 5 seconds, it is terminated.
 *
 * Usage:
 * - Compile: gcc process_with_timeout.c -o process_with_timeout
 * - Execute: ./process_with_timeout [command]
 *
 * Learning Objectives for Students:
 * 1. Understand process creation and IPC using pipes in C.
 * 2. Learn to use signals for process timeout management.
 * 3. Observe how to execute shell commands using **execlp()**.
 *
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>

// Global variable to hold the child process ID
pid_t child_pid;

/******************************************************************************
 * Function: handle_alarm
 *
 * Purpose:
 * Signal handler for **SIGALRM**. This function is triggered when the alarm of 3 seconds
 * set by the parent process expires. It terminates the child process to
 * enforce a timeout.
 *
 * Parameters:
 * - sig: The signal number (unused in this function).
 *****************************************************************************/
void handle_alarm(int sig) {
    if (child_pid > 0) {
        // Terminate the child process if it exceeds the timeout
        kill(child_pid, SIGKILL);
        printf("Child process timed out and was terminated...\n");
    }
}

/******************************************************************************
 * Function: main
 *
 * Purpose:
 * Entry point of the program. This function creates a child process to execute
 * a specified program, sets up IPC using pipes, and manages the child process
 * using signals and synchronization.
 *
 * Parameters:
 * - argc: Argument count (number of command-line arguments).
 * - argv: Argument vector (array of command-line arguments).
 *
 * Returns:
 * - Exit status code (0 for success, non-zero for failure).
 *****************************************************************************/

int main(int argc, char *argv[]) {

    printf("\nRun the program with a valid command to see the usage of redirection with PIPE and DUP2\n");

    printf("\nRun the program with 'sleep(5)' command to see the usage of Alarm signal to terminate the child process\n");

    // Check if the program is executed with the correct number of arguments
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <./executable> [any command]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int pipe_stdin[2], pipe_stdout[2]; // Pipes for IPC

    // Create pipes for stdin and stdout
    if (pipe(pipe_stdin) == -1 || pipe(pipe_stdout) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Fork the process to create a child
    child_pid = fork();

    if (child_pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0) { // Child process
        // Redirect stdin
        dup2(pipe_stdin[0], STDIN_FILENO);
        close(pipe_stdin[1]); // Close unused write end of stdin pipe

        // Redirect stdout
        dup2(pipe_stdout[1], STDOUT_FILENO);
        close(pipe_stdout[0]); // Close unused read end of stdout pipe

        // Execute the specified program
        execlp("sh", "sh", "-c", argv[1], NULL);

        // If exec fails
        perror("execvp");
        exit(EXIT_FAILURE);
    } else { // Parent process
        // Set up signal handling for SIGALRM
        signal(SIGALRM, handle_alarm);

        // Set an alarm to terminate the child after 5 seconds
        alarm(3);

        close(pipe_stdin[0]);  // Close unused read end of stdin pipe
        close(pipe_stdout[1]); // Close unused write end of stdout pipe

        // Write input to the child process (if needed)
        close(pipe_stdin[1]); // Close write end of stdin pipe after writing

        // Read output from the child's stdout
        char buffer[1024];
        ssize_t count;
        printf("\n\nParent received output from the command[%s]:\n", argv[1]);
        while ((count = read(pipe_stdout[0], buffer, sizeof(buffer))) > 0) {
            write(STDOUT_FILENO, buffer, count);
        }
        printf("\"\n");
        close(pipe_stdout[0]); // Close read end of stdout pipe

        // Wait for the child process to complete
        int status;
        waitpid(child_pid, &status, 0);

        // Cancel the alarm
        alarm(0);

        // Report the child's exit status
        if (WIFEXITED(status)) {
            printf("\nChild process exited with status %d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("\nChild process terminated by signal %d\n", WTERMSIG(status));
        }
    }

    return 0;
}
