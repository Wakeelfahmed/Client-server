/******************************************************************************
 * File: execve.c
 * 
 * Purpose:
 * This program demonstrates the usage of the **execve()** system call, which 
 * replaces the current process image with a new process image while allowing 
 * the user to specify both command-line arguments and environment variables 
 * for the new process. This is the most low-level form of the `exec` family.
 * 
 * Key Concepts Demonstrated:
 * 1. Usage of the **execve()** function to execute another program.
 * 2. Passing arguments to the new program via an array of strings (`argv`).
 * 3. Specifying a custom environment for the new program using the `envp` array.
 * 4. Handling errors when the **execve()** function fails to execute.
 * 
 * Program Execution:
 * - The program calls **execve()** to execute the `/bin/sh` shell with the `-c` option, 
 *   which runs the `date` command to display the current date and time.
 * - A custom environment is provided, including variables like `HOME`, `PATH`, `TZ`, and `USER`.
 * - If the **execve()** call fails, the program prints an error message and exits with an error code.
 * 
 * Output:
 * - If successful, the program outputs the current date and time as executed by the `date` command.
 * - If the **execve()** call fails, it prints "Oops!" to `stderr` and terminates.
 * 
 * Usage:
 * - Compile: gcc execve.c -o execve
 * - Execute: ./execve
 * 
 * Learning Objectives for Students:
 * 1. Understand how the **execve()** function replaces the current process image.
 * 2. Learn how to pass arguments to the new program using the `argv` array.
 * 3. Understand the use of the `envp` array to provide custom environment variables.
 * 4. Recognize the behavior of a program after the **execve()** call: the code following 
 *    **execve()** is not executed if the call is successful.
 * 5. Handle errors in system calls such as **execve()**.
 * 
 * Experimentation Instructions:
 * 1. Modify the `argv` array to execute other commands or scripts with different arguments.
 * 2. Change the `envp` array to provide different environment variables and observe the behavior.
 * 3. Try providing an incorrect file path to **execve()** to observe error handling.
 * 4. Experiment with different shell commands or custom environment setups to understand the power of **execve()**.
 * 
 *****************************************************************************/

#include <unistd.h>  // For execve()
#include <stdio.h>   // For fprintf(), stderr

int main(void)
{
    // Define the arguments for the new program.
    // The first element is the program to be executed (`/bin/sh`).
    // The second element is the `-c` option to run a shell command.
    // The third element is the shell command itself (`date`), which displays the current date and time.
    // The array is terminated with a NULL pointer as required.
    char *argv[] = { "/bin/sh", "-c", "date", NULL };

    // Define a custom environment for the new program.
    // These environment variables will be used by the executed program (`/bin/sh` in this case).
    const char *envp[] =
    {
        "HOME=/",         // Set the HOME directory to root.
        "PATH=/bin:/usr/bin", // Specify the PATH for locating executables.
        "TZ=UTC+0",       // Set the time zone to UTC.
        "USER=bhargav.bhatkalkar", // Set the username to "your name".
        NULL              // Null-terminate the environment array.
    };

    // The execve() function is used to execute the `/bin/sh` command.
    // Parameters:
    // 1. Path to the executable: `argv[0]` ("/bin/sh").
    // 2. argv: Array of argument strings passed to the new program.
    // 3. envp: Custom environment array passed to the new program.
    execve(argv[0], argv, (char *const *)envp);

    // If execve() fails, control returns to this point. Print an error message.
    fprintf(stderr, "Oops! Failed to execute the shell command.\n");

    // Return an error code to indicate failure.
    return -1;
}
