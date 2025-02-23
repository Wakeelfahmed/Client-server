/******************************************************************************
 * File: execl.c
 * 
 * Purpose:
 * This program demonstrates the usage of the **execl()** system call, which 
 * is used to execute a program by replacing the current process image with 
 * a new process image. Specifically, this program runs the `grep` command 
 * to search for the keyword "execlp" in the file "execlp.c".
 * 
 * Key Concepts Demonstrated:
 * 1. Usage of the **execl()** function to execute another program.
 * 2. Passing arguments to the new program through the variadic arguments of **execl()**.
 * 3. Handling errors when the **execl()** function fails to execute.
 * 
 * Program Execution:
 * - The program calls **execl()** to execute the `grep` command located at `/usr/bin/grep`.
 * - The command searches for the keyword "PCB" recursively in the file `PCB.c`.
 * - If the **execl()** call fails, the program prints an error message and exits with an error code.
 * 
 * Output:
 * - If successful, the program outputs the result of the `grep` command and shows evey line with keyword "PCB".
 * - If the **execl()** call fails, it prints "Oops!" to `stderr` and terminates.
 * 
 * Usage:
 * - Compile: gcc execl.c -o execl
 * - Execute: ./execl
 * 
 * Learning Objectives for Students:
 * 1. Understand how the **execl()** function replaces the current process image.
 * 2. Learn how to pass arguments to the new program via **execl()**.
 * 3. Recognize the behavior of a program after the **execl()** call: the code following 
 *    **execl()** is not executed if the call is successful.
 * 4. Handle errors in system calls such as **execl()**.
 * 
 * Experimentation Instructions:
 * 1. Modify the arguments passed to **execl()** to execute other commands or scripts.
 * 2. Try providing an incorrect file path to **execl()** to observe error handling.
 * 3. Experiment with different `grep` options or commands to understand argument passing.
 * 
 *****************************************************************************/

#include <unistd.h>  // For execl()
#include <stdio.h>   // For fprintf(), stderr

int main(void)
{
    // The execl() function is used to execute the `grep` command.
    // Parameters:
    // 1. Path to the executable: "/usr/bin/grep".
    // 2. First argument (by convention): The name of the command being executed ("grep").
    // 3. Subsequent arguments: Options for the `grep` command.
    //    - "-r": Recursively search through directories.
    //    - "PCB": The keyword to search for.
    //    - "PCB.c": The file to search within.
    // 4. NULL: Terminates the argument list.
    execl("/usr/bin/grep", "grep", "-r", "PCB", "PCB.c", NULL);

    // If execl() fails, control returns to this point. Print an error message.
    fprintf(stderr, "Oops!\n");

    // Return an error code to indicate failure.
    return -1;
}

