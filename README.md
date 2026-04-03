# Operating Systems Final Project  
  
A menu-driven modular system in **C** built to demonstrate core operating system concepts through implementation and simulation.  
## Overview  
  
This project was developed for **CSCI 3431** as a practical exploration of operating systems concepts. The system is organized into separate executable modules and launched through a central main menu using process creation and execution system calls.  
  
## Features  
  
The project includes the following modules:  
  
### 1. Logging System  
A centralized logger that records user input and system actions with timestamps.    
Implemented using inter-process communication techniques such as shared memory, pipes, or `fork()` / `exec()`.  
  
### 2. Peterson’s Solution  
A simulation of Peterson’s mutual exclusion algorithm for two processes.    
Demonstrates busy waiting, entry section, critical section, and exit section behavior.  
  
### 3. File Management  
Supports basic file operations:  
- Create files  
- Read files  
- List files  
- Delete files  
  
### 4. Contiguous Memory Allocation + Compaction  
Simulates contiguous memory management using **first-fit allocation**.    
Supports:  
- User-defined initial memory holes  
- Allocation  
- Deallocation  
- Memory map display for allocated and free blocks  
  
### 5. Amdahl’s Law  
Calculates theoretical speedup based on:  
- Parallel fraction  
- Number of cores  
  
Also includes a comparison between serial and parallel execution.  
  
### 6. Boot Script + Main Menu  
A `boot.sh` script handles compilation and launch.    
The main menu is implemented using:  
- `fork()`  
- `exec()`  
- `wait()`  
  
## Project Structure  
  
```text  
.  
├── src/            # All C source files  
├── bin/            # Compiled executables  
├── logs/           # Log output  
├── boot.sh         # Build and run script  
└── README.md
```

## Build and Run

Make the boot script executable and run it:

chmod +x boot.sh  
./boot.sh

## What `boot.sh` Does

- Removes old build and log folders
- Recreates required directories
- Compiles the logger first
- Compiles each module into `/bin`
- Prints compilation success/failure messages
- Launches the main menu

## Requirements

- Written in **C only**
- Modular design
- Each module compiled as a separate executable
- Logging included
- Input validation handled where required
