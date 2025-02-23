Introduction:
=============

The exec family of system calls in Unix-like operating systems is used to execute a new program by replacing the current process image with a new one. These system calls are part of process control and are commonly used in multitasking environments to execute other programs.

This document will help you understand the key differences between the various system calls in the exec family:
- execl()
- execle()
- execlp()
- execv()
- execve()
- execvp()


Common Concepts:
================
1. Process Replacement:
	When a process calls an exec function, it is completely replaced by the new program. The current process is not terminated but transformed into the new process.

2. Arguments:
	- All exec functions allow passing arguments to the new program.
	- Arguments can be passed as individual strings or as an array of strings.

3. Environment Variables:
	Some exec calls allow you to explicitly pass custom environment variables to the new process.

4. Path Searching:
	Some exec calls use the PATH environment variable to locate the executable if only the program name (e.g., ls) is provided.

5. Function Naming:
	The naming conventions in the exec family indicate the features:
	  L: Arguments are passed as a list of strings (variadic arguments).
	  V: Arguments are passed as a vector (array of strings).
	  E: A custom environment can be specified.
	  P: Uses the PATH variable to locate the executable.


Detailed Explanation of Each System Call
========================================
1. execl()

	int execl(const char *path, const char *arg, ..., NULL);
	
	- Accepts the program path and a list of arguments (variadic arguments).
	- The arguments are passed as individual strings, ending with a NULL.
	- Does not search for the program in the PATH variable. You must provide the full path.
	- Simple when you know the full path to the executable and the exact arguments.

	Example:
		execl("/bin/ls", "ls", "-l", NULL);

2. execle()

	int execle(const char *path, const char *arg, ..., NULL, char *const envp[]);

	- Same as execl() but allows you to pass a custom environment (envp).
	- Does not search for the program in the PATH variable.
	- When you need to run a program with a specific environment, overriding the parent process's environment.

	Example:
		const char *envp[] = { "PATH=/bin:/usr/bin", "USER=student", NULL };
		execle("/bin/date", "date", "--utc", NULL, envp);

3. execlp()

	int execlp(const char *file, const char *arg, ..., NULL);

	- Similar to execl() but searches for the program in the directories listed in the PATH environment variable.
	- Requires only the program name, not the full path.
	- Convenient when you want to execute a program without specifying its full path.

	Example:
		execlp("ls", "ls", "-l", NULL);

4. execv()

	int execv(const char *path, char *const argv[]);

	- Accepts the program path and an array (argv) of arguments.
	- Does not search for the program in the PATH variable.
	- Use when arguments are already stored in an array.

	Example:
		char *argv[] = { "ls", "-l", NULL };
		execv("/bin/ls", argv);

5. execve()
	
	int execve(const char *path, char *const argv[], char *const envp[]);

	- The most low-level form of exec. Takes a program path, an array of arguments, and an array of environment variables.
	- Does not search for the program in the PATH variable.
	- Use when you need to provide both custom arguments and a custom environment.

	Example:
		char *argv[] = { "/bin/sh", "-c", "date", NULL };
		char *envp[] = { "TZ=UTC+0", NULL };
		execve(argv[0], argv, envp);

6. execvp()
	
	int execvp(const char *file, char *const argv[]);
	
	- Similar to execv() but searches for the program in the PATH environment variable.
	- Requires only the program name in the first argument of argv.
	- Convenient for running commands by their program name without needing the full path.

	Example:
		char *argv[] = { "grep", "-r", "execvp", "execvp.c", NULL };
		execvp(argv[0], argv);

