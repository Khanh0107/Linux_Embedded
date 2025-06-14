# General Knowlege
## 1.Make file
### 1.1 What is make file
- Make file is a script that contains the following information:
	- Structure of a project (file, dependency)
	- Command lines are used to create and destroy files
-   The make program reads the Makefile, determines which files need to be updated, and executes the necessary commands to build the project efficiently.
- Naming conventions: 
	- `Makefile` : (recommended, as `make` searches for this by default).
	- `makefile`  (less common but still works).
	-   `*.mk`  (used in modular or included Makefiles).
 
### 1.2. How to run make?

-   The  `make`  command executes  **shell commands**  in Linux to update one or more  **target files**  (typically a program or an executable).
-   If the  `-f`  option is not provided,  **make**  automatically looks for a file named  **Makefile**  or  **makefile**  in the current directory.
-   It checks  **dependencies**  and executes the required commands  **only if changes**  are detected, optimizing the build process.
-   For more details, refer to the manual:  `man make`

`[options]`

-   `-f File`  : Use file as a makefile
-   `-i`  : Ignore all errors in commands executed to remake files.
-   `-n`  : Print the commands that would be executed, but do not execute them.

**`[targets]`**

-   **all**: make all the top-level targets the make file know about
-   **info**: Information of program
-   **clean**: Delete all files that are normally created by running make
-   **install**: Copy the executable file into directory that users typically search for commands
-   **uninstall**: Vice versa
-   **tar**: Create a tar file of the source files
-   **test**: Perform self tests on the program this Makefile builds.

### 1.3. Variables in Makefile

**Why Use Variables?**

-   Variables help  **generalize**  the  `Makefile`, avoiding hardcoding specific file names.
-   They make the  `Makefile`  **easier to modify and maintain**.
-   To access the value of a variable, use the syntax:  `$(VARIABLE_NAME)`
-   We can print the value of a variable using an echo command inside a special target:

**Common Compiler & Linker Flags**

```
  CFLAGS += -I./include -DDEBUG -Wall -g
  LDFLAGS += -L./lib -lm

```

-   `CFLAGS`: Flags passed to the C compiler (gcc).
    
    -   `I./include`  → Adds  `include/`  as a header file search path.
    -   `-DDEBUG`  → Defines a preprocessor macro DEBUG.
    -   `-Wall`  → Enables all important warnings.
    -   `-g`  → Generates debugging symbols for gdb.
-   `LDFLAGS`: Flags passed to the  **linker**  (`ld`).
    
    -   `-L./lib`  → Adds  `lib/`  as a  **library search path**.
    -   `-lm`  → Links the  **math library**  (`libm.so`).

| Flags     | Meaning                                                     |
| --------- | ----------------------------------------------------------- |
| `-I`      | Include all header files (`*.h`) that followed by directory |
| `-Wall`   | Enable all warning                                          |
| `-g`      | Enable debug in `gcc` compiler                              |
| `-L./lib` | Path to library                                             |
| `-lm`     | Library’s name                                              |

**Automatic Variables**
- Automatic variables are special predefined variables in Makefile that help streamline rule definitions by referring to targets, dependencies, and commands dynamically.

| **Variable** | **Meaning**                                                     |
| ------------ | --------------------------------------------------------------- |
| `$@`         | Represents the **target name** of the current rule.             |
| `$<`         | Represents the **first prerequisite (dependency)** of the rule. |
| `$^`         | Represents **all prerequisites** of the rule.                   |
| `$?`         | Represents **prerequisites that are newer** than the target.    |
| `$*`         | Represents the **basename** of the target (without extension).  |


## 2. The Process of Compiling a C Program

### 2.1. Pre-processing

The pre-processing step performs the following tasks before actual compilation:

-   **Removes comments**  from the source code.
-   **Expands macros**  (`#define`  and  `#ifdef`).
-   **Expands include files**  by replacing  `#include`  directives with the actual contents of the header files.
-   **Processes conditional compilation statements**  (`#ifdef`,  `#ifndef`,  `#endif`, etc.).
-   **Generates an intermediate file**  with a  `.i`  extension, containing the transformed source code without comments and with all macros and includes expanded.

💡  **Command**

`gcc -E source.c -o source.i`

### 2.2. Compilation

This stage translates the preprocessed C code into assembly language.

-   Translates the preprocessed  `.i`  file into assembly language.
-   Performs  `syntax analysis`  and converts  `C code` into  `assembly`  instructions.
-   The output  `.s`  file contains human-readable assembly code.

💡  **Command**

`gcc -S source.i -o source.s`

### 2.3. Assemply

In this stage, the assembly code is converted into machine code.

-   The file  `*.s`  in the previous stage continues to be used for this stage by using assembler.
-   The output we get is a file  `.o`. This is a file containing machine language level instructions.

💡  **Command**

`gcc -c source.s -o source.o`

### 2.4. Linking

Combining Object Files to Create an Executable

-   The linking stage is where individual  **object files (`.o`)**  are combined to produce a complete executable program.
-   It connects object files with  **static (`.a`  /  `.lib`)**  or  **shared (`.so`  /  `.dll`)**  libraries.
-   The final output is a  **executable file**  (`a.out`  by default, o  `.exe`  on Windows).

💡  **Command**

`gcc source.o -o output_executable`



