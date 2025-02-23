/******************************************************************************
 * File: execv.c
 * 
 * Purpose:
 * This program demonstrates the usage of the **execv()** system call, which 
 * is used to execute a program by replacing the current process image with 
 * a new process image. The **execv()** function uses an array of pointers to 
 * null-terminated strings to specify the arguments for the new program.
 * 
 * Key Concepts Demonstrated:
 * 1. Usage of the **execv()** function to execute another program.
 * 2. Providing arguments to the new program via an array of strings (`argv`).
 * 3. Handling errors when the **execv()** function fails to execute.
 * 
 * Program Execution:
 * - The program calls **execv()** to execute the `/bin/ls` command.
 * - The `ls` command is invoked with the `-l` option to list files in long format.
 * - If the **execv()** call fails, the program prints an error message and exits with an error code.
 * 
 * Output:
 * - If successful, the program outputs the list of files in the current directory in long format.
 * - If the **execv()** call fails, it prints "Oops!" to `stderr` and terminates.
 * 
 * Usage:
 * - Compile: gcc execv.c -o execv
 * - Execute: ./execv
 * 
 * Learning Objectives for Students:
 * 1. Understand how the **execv()** function replaces the current process image.
 * 2. Learn how to provide arguments to the new program using the `argv` array.
 * 3. Recognize the behavior of a program after the **execv()** call: the code following 
 *    **execv()** is not executed if the call is successful.
 * 4. Handle errors in system calls such as **execv()**.
 * 
 * Experimentation Instructions:
 * 1. Modify the `argv` array to execute other commands or scripts with different arguments.
 * 2. Try providing an incorrect file path to **execv()** to observe error handling.
 * 3. Experiment with different commands and options to understand how **execv()** works.
 * 
 *****************************************************************************/

#include <unistd.h>  // For execv()
#include <stdio.h>   // For fprintf(), stderr

int main(void)
{
    // Define the arguments for the `ls` command.
    // The first argument is the name of the program being executed ("ls").
    // The second argument is the `-l` option to display files in long format.
    // The array is terminated with a NULL pointer as required.
    char *argv[] = { "ls", "-l", NULL };

    // The execv() function is used to execute the `/bin/ls` command.
    // Parameters:
    // 1. Path to the executable: "/bin/ls".
    // 2. argv: Array of argument strings passed to the new program.
    execv("/bin/ls", argv);

    // If execv() fails, control returns to this point. Print an error message.
    fprintf(stderr, "Oops! Failed to execute the ls command.\n");

    // Return an error code to indicate failure.
    return -1;
}
