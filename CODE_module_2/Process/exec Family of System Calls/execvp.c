/******************************************************************************
 * File: execvp.c
 * 
 * Purpose:
 * This program demonstrates the usage of the **execvp()** system call, which 
 * is used to execute a program by replacing the current process image with 
 * a new process image. The **execvp()** function automatically searches for 
 * the specified program in the directories listed in the `PATH` environment variable.
 * 
 * Key Concepts Demonstrated:
 * 1. Usage of the **execvp()** function to execute another program.
 * 2. Passing arguments to the new program via an array of strings (`argv`).
 * 3. Leveraging the `PATH` environment variable to locate the executable.
 * 4. Handling errors when the **execvp()** function fails to execute.
 * 
 * Program Execution:
 * - The program calls **execvp()** to execute the `grep` command.
 * - The `grep` command is invoked with the `-r` option to recursively search for the 
 *   keyword "execvp" in the file `execvp.c`.
 * - If the **execvp()** call fails, the program prints an error message and exits with an error code.
 * 
 * Output:
 * - If successful, the program outputs all lines in the file `execvp.c` that contain the keyword "execvp".
 * - If the **execvp()** call fails, it prints "Oops!" to `stderr` and terminates.
 * 
 * Usage:
 * - Compile: gcc execvp.c -o execvp
 * - Execute: ./execvp
 * 
 * Learning Objectives for Students:
 * 1. Understand how the **execvp()** function replaces the current process image.
 * 2. Learn how to pass arguments to the new program using the `argv` array.
 * 3. Recognize how **execvp()** leverages the `PATH` environment variable to locate executables.
 * 4. Recognize the behavior of a program after the **execvp()** call: the code following 
 *    **execvp()** is not executed if the call is successful.
 * 5. Handle errors in system calls such as **execvp()**.
 * 
 * Experimentation Instructions:
 * 1. Modify the `argv` array to execute other commands or scripts with different arguments.
 * 2. Ensure the `PATH` variable is properly set to include the directory containing the executable.
 * 3. Try providing an incorrect file name or path to observe error handling.
 * 4. Experiment with different commands and options to better understand the behavior of **execvp()**.
 * 
 *****************************************************************************/

#include <unistd.h>  // For execvp()
#include <stdio.h>   // For fprintf(), stderr

int main(void)
{
    // Define the arguments for the `grep` command.
    // The first element is the name of the program to be executed ("grep").
    // The second element is the `-r` option, which enables recursive searching.
    // The third element is the keyword to search for ("execvp").
    // The fourth element is the file to search in ("execvp.c").
    // The array is terminated with a NULL pointer as required.
    char *argv[] = { "grep", "-r", "execvp", "execvp.c", NULL };

    // The execvp() function is used to execute the `grep` command.
    // Parameters:
    // 1. argv[0]: The program name ("grep"), which is searched for in the directories listed in `PATH`.
    // 2. argv: Array of argument strings passed to the new program.
    execvp(argv[0], argv);

    // If execvp() fails, control returns to this point. Print an error message.
    fprintf(stderr, "Oops! Failed to execute the grep command.\n");

    // Return an error code to indicate failure.
    return -1;
}
