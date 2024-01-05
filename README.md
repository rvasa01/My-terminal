# My-terminal
This project involves the creation of a command-line interpreter (CLI) named "mysh" in the C programming language. The main learning objectives include understanding basic C/UNIX process management system calls, manipulating and redirecting a program's standard input, output, and error streams, and implementing basic inter-process communication operations.

The mysh CLI supports various functionalities, as would a Linux terminal. It utilizes mechanisms like fork(), wait(), exec(), open(), close(), dup2(), and pipe() for process management, file I/O, and inter-process communication.

The CLI takes an optional prompt string as a command-line argument, defaults to "mysh: " if no argument is given, and omits the prompt if "-" is specified. The main operation involves printing a prompt, and reading a single input line. 
