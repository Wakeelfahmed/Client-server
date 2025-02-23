/******************************************************************************
 * File: execle.c
 * 
 * Purpose:
 * This program demonstrates the usage of the **execle()** system call, which 
 * is used to execute a program by replacing the current process image with 
 * a new process image. Additionally, it allows the caller to specify a custom 
 * environment for the executed program through the `envp` argument.
 * 
 * Key Concepts Demonstrated:
 * 1. Usage of the **execle()** function to execute another program.
 * 2. Passing arguments to the new program, including command-line options.
 * 3. Specifying a custom environment for the new program via the `envp` argument.
 * 4. Handling errors when the **execle()** function fails to execute.
 * 
 * Program Execution:
 * - The program calls **execle()** to execute the `/bin/date` command.
 * - The `date` command is invoked with the `--utc` option to display the current time in UTC.
 * - A custom environment is provided, setting variables such as `HOME`, `PATH`, `TZ`, and `USER`.
 * - If the **execle()** call fails, the program prints an error message and exits with an error code.
 * 
 * Output:
 * - If successful, the program outputs the current date and time in UTC format.
 * - If the **execle()** call fails, it prints "Oops!" to `stderr` and terminates.
 * 
 * Usage:
 * - Compile: gcc execle.c -o execle
 * - Execute: ./execle
 * 
 * Learning Objectives for Students:
 * 1. Understand how the **execle()** function replaces the current process image.
 * 2. Learn how to pass arguments to the new program via **execle()**.
 * 3. Understand the use of the `envp` argument to provide a custom environment.
 * 4. Recognize the behavior of a program after the **execle()** call: the code following 
 *    **execle()** is not executed if the call is successful.
 * 5. Handle errors in system calls such as **execle()**.
 * 
 * Experimentation Instructions:
 * 1. Modify the arguments passed to **execle()** to execute other commands or scripts.
 * 2. Change the `envp` array to provide different environment variables and observe the behavior.
 * 3. Try providing an incorrect file path to **execle()** to observe error handling.
 * 4. Experiment with different commands and options to understand how **execle()** works.
 * 
 *****************************************************************************/

#include <unistd.h>  // For execle()
#include <stdio.h>   // For fprintf(), stderr

int main(void)
{
    // Define a custom environment for the new process.
    const char *envp[] =
    {
        "HOME=/",         // Set the HOME directory to root.
        "PATH=/bin:/usr/bin", // Specify the PATH for locating executables.
        "TZ=UTC+0",       // Set the time zone to UTC.
        "USER=bhargav.bhatkalkar", // Set the username to "your name".
        NULL              // Null-terminate the environment array.
    };

    // The execle() function is used to execute the `/bin/date` command.
    // Parameters:
    // 1. Path to the executable: "/bin/date".
    // 2. First argument (by convention): The name of the program being executed ("date").
    // 3. Second argument: Option for the `date` command ("--utc").
    // 4. NULL: Terminates the argument list.
    // 5. envp: Custom environment array to pass to the new program.
    execle("/bin/date", "date", "--utc", NULL, envp);

    // If execle() fails, control returns to this point. Print an error message.
    fprintf(stderr, "Oops! Failed to execute the date command.\n");

    // Return an error code to indicate failure.
    return -1;
}

