# Chess – C Programming Project

This repository contains multiple C programming assignments focused on implementing core chess-related logic and game mechanics.

The project is structured in separate homework versions, each extending or refining the previous implementation.

---

## Project Structure

- `Chess_hw09`
- `Chess_hw11`
- `Chess_hw12`

Each folder represents a different stage of development and includes:
- `.c` source files
- `.h` header files
- `main.c` entry point

---

## Project Overview

The application implements fundamental chess functionality, including:

- Chess board representation
- Piece movement validation
- Game logic processing
- Structured modular design using header files
- Separation between logic and main execution

Each homework iteration improves or extends the previous version by refining game logic and adding new functionality.

---

## Technical Concepts Used

- Modular programming in C
- Header and source file separation
- Structured data representation
- Function decomposition
- Pointer usage
- Control structures and validation logic

---

## Technologies

- C language
- Standard C libraries
- GCC / Linux environment# C-Programming

---

## Algorithms and Data Structures Assignments

This repository also includes a collection of programming assignments developed for the **Fundamental Algorithms** course. Each homework focuses on implementing classical algorithms and analyzing their performance.

### Homework Topics

- **Homework 1 – Basic Sorting Algorithms**  
  Implementation and analysis of fundamental sorting algorithms such as Bubble Sort, Insertion Sort, and Selection Sort, including comparison of their performance in best, average, and worst cases.

- **Homework 2 – Heap Construction and HeapSort**  
  Implementation of heap construction using both **bottom-up** and **top-down** approaches, along with the HeapSort algorithm and comparative performance analysis.

- **Homework 3 – Advanced Sorting Algorithms**  
  Implementation and analysis of **QuickSort**, **Hybrid QuickSort**, and **QuickSelect**, with comparisons against HeapSort and evaluation of algorithmic performance. 

- **Homework 4 – Merging k Sorted Lists**  
  Efficient merging of multiple sorted sequences using heap-based techniques with time complexity **O(n log k)**. 

- **Homework 5 – Hash Tables**  
  Implementation of hash tables with **open addressing and quadratic probing**, including insertion, search, deletion, and performance evaluation for different load factors. 

- **Homework 6 – Multi-way Trees**  
  Tree traversal algorithms and transformations between different tree representations (parent array, multi-way tree, and binary tree representation). 

- **Homework 7 – Order Statistic Trees**  
  Implementation of balanced binary search trees supporting operations such as **BUILD**, **SELECT**, and **DELETE** for order statistics. 

- **Homework 8 – Disjoint Sets and Kruskal’s Algorithm**  
  Implementation of union–find data structures with **union by rank** and **path compression**, used for computing the **Minimum Spanning Tree** with Kruskal’s algorithm. 

- **Homework 9 – Breadth-First Search (BFS)**  
  Implementation of BFS traversal on graphs, shortest path discovery in grid graphs, and performance analysis. 

- **Homework 10 – Depth-First Search (DFS)**  
  Implementation of DFS traversal, **topological sorting**, and **Tarjan’s algorithm** for strongly connected components.




# Operating Systems Assignments (C)

This repository contains three programming assignments developed in **C** for the **Operating Systems** course.

The projects focus on fundamental operating system concepts such as:
- file system manipulation
- process creation
- thread synchronization
- interprocess communication
- shared memory and memory mapping

All programs are implemented using **Linux system calls and POSIX APIs**.

---

## Assignment 1 – File System (a1)

The first assignment focuses on working with the **Linux file system** and a custom binary file format called **SF (Section File)**.

The program implements multiple command-line operations.

### Features
- Display the assignment variant
- List directory contents
- Recursive directory traversal
- File filtering based on size or name prefix
- Parsing and validating the custom **SF file format**
- Extracting specific lines from sections of SF files
- Searching directories for SF files that satisfy specific conditions

### Compilation

```bash
gcc -Wall a1.c -o a1
```

### Run

```bash
./a1 [OPTIONS]
```

---

## Assignment 2 – Processes and Threads (a2)

The second assignment focuses on **process creation and thread synchronization**.

The program generates a hierarchy of processes and creates multiple threads inside them.  
Each process and thread must report its execution using special helper functions.

### Features
- Process creation using `fork()`
- Thread creation using `pthread`
- Synchronization between threads
- Controlling execution order between threads
- Limiting the number of simultaneously running threads
- Synchronization between threads from different processes

The tester communicates with the program through helper functions such as `init()` and `info()`.

### Compilation

```bash
gcc -Wall a2.c a2_helper.c -o a2 -lpthread
```

### Run

```bash
./a2
```

---

## Assignment 3 – Interprocess Communication (a3)

The third assignment implements communication between processes using **IPC mechanisms in Linux**.

The program interacts with a tester process through **named pipes**, shared memory, and memory-mapped files.

### Features
- Communication using **named pipes (FIFO)**
- Handling multiple request commands
- Creating shared memory regions
- Writing values to shared memory
- Mapping files into memory using `mmap`
- Reading bytes from mapped files
- Reading data from specific file sections
- Accessing logical memory space inside files

The program continuously reads requests from a request pipe and sends responses through a response pipe.

### Compilation

```bash
gcc -Wall a3.c -o a3 -lrt
```

### Run

```bash
./a3
```

---

## Technologies Used

- C
- POSIX Threads (pthreads)
- Linux System Calls
- Named Pipes (FIFO)
- Shared Memory
- Memory Mapped Files (mmap)

---

## Author

Denisa-Andreea Ujeniuc  
Technical University of Cluj-Napoca  
Computer Science