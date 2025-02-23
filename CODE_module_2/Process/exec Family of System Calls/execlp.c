/******************************************************************************
 * File: execlp.c
 * 
 * Purpose:
 * This program demonstrates the usage of the **execlp()** system call, which 
 * is similar to **execl()**, but with the added capability of searching for 
 * the specified program in the directories listed in the `PATH` environment variable. 
 * Specifically, this program runs the `env` command to display the current 
 * environment variables.
 * 
 * Key Concepts Demonstrated:
 * 1. Usage of the **execlp()** function to execute another program.
 * 2. Automatic path searching for the executable file using the `PATH` environment variable.
 * 3. Handling errors when the **execlp()** function fails to execute.
 * 
 * Program Execution:
 * - The program calls **execlp()** to execute the `env` command.
 * - The `env` command lists all environment variables for the current process.
 * - If the **execlp()** call fails, the program prints an error message and exits with an error code.
 * 
 * Output:
 * - If successful, the program outputs all environment variables.
 * - If the **execlp()** call fails, it prints "Oops!" to `stderr` and terminates.
 * 
 * Usage:
 * - Compile: gcc execlp.c -o execlp
 * - Execute: ./execlp
 * 
 * Learning Objectives for Students:
 * 1. Understand how the **execlp()** function replaces the current process image.
 * 2. Learn the difference between **execl()** and **execlp()**, particularly the automatic 
 *    path searching feature of **execlp()**.
 * 3. Recognize the behavior of a program after the **execlp()** call: the code following 
 *    **execlp()** is not executed if the call is successful.
 * 4. Handle errors in system calls such as **execlp()**.
 * 
 * Experimentation Instructions:
 * 1. Modify the arguments passed to **execlp()** to execute other commands or scripts.
 * 2. Try providing a command that does not exist in the `PATH` environment to observe error handling.
 * 3. Experiment with different commands or arguments to understand how **execlp()** works.
 * 
 *****************************************************************************/

#include <unistd.h>  // For execlp()
#include <stdio.h>   // For fprintf(), stderr

int main(void)
{
    // The execlp() function is used to execute the `env` command.
    // Parameters:
    // 1. First argument: The name of the command to search in `PATH` ("env").
    // 2. Second argument: The name of the program being executed (by convention, "env").
    // 3. NULL: Terminates the argument list.
    execlp("env", "env", NULL);

    // If execlp() fails, control returns to this point. Print an error message.
    fprintf(stderr, "Oops! Failed to execute the env command.\n");

    // Return an error code to indicate failure.
    return -1;
}
