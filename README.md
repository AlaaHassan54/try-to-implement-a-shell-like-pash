# try-to-implement-a-shell-like-bash


This project implements a simple shell in C, gradually expanding from a **Femto Shell** to a **Nano Shell**. Each version adds more functionality, demonstrating the core components of a Unix-like shell.

---

## 🐣 Femto Shell

The **Femto Shell** is the simplest version. It supports:

### ✅ Supported Commands
- `echo`: Prints the arguments back to the terminal.
- `exit`: Terminates the shell with a "Good Bye :)" message.

### ❌ Invalid Command
Any command other than `echo` or `exit` will print:invalid command

### 🧪 Example:
```bash
$ ./myFemtoShell
Femto shell prompt > echo Hello my shell
Hello my shell
Femto shell prompt > ls
Invalid command
Femto shell prompt > exit
Good Bye :)

🐥 Pico Shell

The Pico Shell expands functionality to include:
✅ Supported Built-in Commands

    echo

    pwd: Prints the current working directory.

    cd <dir>: Changes current working directory.

    exit

✅ External Commands

    Commands like ls, cat, etc. are supported via fork and exec.

🧪 Example:

$ ./myPicoShell
Pico shell prompt > cd /tmp
Pico shell prompt > pwd
/tmp
Pico shell prompt > ls
file.txt file.c
Pico shell prompt > exit
Good Bye :)



🧠 Nano Shell

The Nano Shell adds support for local and environment variables, and argument substitution:
✅ Features

    Local variable assignment: Format must be var=value

    Built-in export: Promotes a local variable to an environment variable

    Variable substitution: $var is replaced with its value in commands

    Substitution works in any command, including echo, ls, cd, etc.

⚠️ Validation

    Invalid assignments like x = 5 or x=5 echo hi print:

Invalid command

🧪 Example:

$ ./myNanoShell
Nano Shell Prompt > x=5
Nano Shell Prompt > echo $x
5
Nano Shell Prompt > folder=home
Nano Shell Prompt > ls /$folder
# lists /home directory
Nano Shell Prompt > echo $non_existing
# prints an empty line
Nano Shell Prompt > export x
Nano Shell Prompt > printenv | grep x
x=5


