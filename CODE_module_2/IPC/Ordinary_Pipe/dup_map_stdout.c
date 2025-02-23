/******************************************************************************
 * File: dup_map_stdout.c
 *
 * Purpose:
 * This program demonstrates the usage of the `dup2()` system call in C, which 
 * is used to duplicate file descriptors. Specifically, this program redirects 
 * the standard output (`stdout`) to a file using `dup2()` so that any 
 * `printf()` calls write to the file instead of the terminal.
 *
 * Key Concepts Demonstrated:
 * 1. Opening a file for writing using `open()`.
 * 2. Redirecting `stdout` to the file using `dup2()`.
 * 3. Closing unused file descriptors to free resources.
 *
 * Program Execution:
 * - The program opens (or creates) a file named `output.txt`.
 * - It redirects `stdout` to the file using `dup2()`.
 * - All `printf()` statements write to the file instead of the console.
 * - After execution, the file `output.txt` contains the redirected output.
 *
 * Usage:
 * - Compile: gcc dup_map_stdout.c -o dup_demo
 * - Execute: ./dup_demo
 * - Check output: cat output.txt
 *
 * Learning Objectives:
 * 1. Understand how to use `dup2()` for file descriptor duplication.
 * 2. Learn how to redirect standard output to a file.
 * 3. Observe proper file handling and resource management in C.
 *****************************************************************************/

#include <stdio.h>  // For printf()
#include <stdlib.h> // For exit() and EXIT_FAILURE
#include <unistd.h> // For dup2()
#include <fcntl.h>  // For open()

int main() {
    int file_fd; // File descriptor for the output file

    /**************************************************************************
     * Step 1: Open the file `output.txt` for writing.
     * - O_WRONLY: Open the file in write-only mode.
     * - O_CREAT: Create the file if it doesn't exist.
     * - O_TRUNC: Truncate the file (clear its contents) if it already exists.
     * - 0644: File permission: Owner can read/write, others can only read.
     **************************************************************************/
    file_fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (file_fd < 0) { // Check if the file was opened successfully
        perror("open"); // Print the error message if opening failed
        exit(EXIT_FAILURE); // Exit the program with a failure status
    }

    /**************************************************************************
     * Step 2: Redirect `stdout` to the file descriptor using `dup2()`.
     * - `dup2(file_fd, STDOUT_FILENO)` duplicates the file descriptor `file_fd`
     *   to `STDOUT_FILENO` (file descriptor 1, used for standard output).
     * - After this, all `printf()` calls will write to the file instead of
     *   printing to the terminal.
     **************************************************************************/
    if (dup2(file_fd, STDOUT_FILENO) < 0) { // Check if `dup2` was successful
        perror("dup2"); // Print the error message if duplication failed
        close(file_fd); // Close the file descriptor before exiting
        exit(EXIT_FAILURE); // Exit the program with a failure status
    }

    /**************************************************************************
     * Step 3: Close the original file descriptor.
     * - After duplicating the file descriptor with `dup2()`, the original
     *   file descriptor `file_fd` is no longer needed and can be closed.
     * - This ensures proper resource management and prevents resource leaks.
     **************************************************************************/
    close(file_fd);

    /**************************************************************************
     * Step 4: Write output using `printf()`.
     * - Since `stdout` is now redirected to the file, these `printf()` calls
     *   will write to `output.txt` instead of the console.
     **************************************************************************/
    printf("Hello, this output is redirected to 'output.txt'.\n");
    printf("dup2() successfully redirected stdout!\n");

    /**************************************************************************
     * Step 5: Program ends.
     * - When the program exits, all file descriptors are automatically closed
     *   by the operating system.
     **************************************************************************/
    return 0;
}
