--
Assignment 1 - CPU Scheduling Algorithims
--
In this programming assignment I created a simulator that will simulate the execution of different CPU scheduling algorithms discussed in class.
Input is taken from known formatted text files. In particular, the simulator implemented the following scheduling algorithms:
* First Come First Serve (FCFS)
* Shortest Remaining Time First (SRT)
* Round Robin (RR)

--
Assignment 2 - Shell programming and multithreads
--
Write a program for a basic command line shell using system calls that allows users to execute programs like a linux shell, ie. it should display a prompt and then execute the command the user enters. Also, using pthreads
write a multithreaded program to count the vowels in 20 different text files simultaneaously.
Note: Command.cpp and Command.h is a helper class provided by the instructor to help parse the commands for the shell we made. They were not written by me.

--
Assignment 3 - File System
--
Implementing a simple file system over a simulated disk represented as an array of blocks that could represent directories, i-nodes, or data blocks. 
All files were provided by the professor as the base of the filesystem. My work is done entirely in the FileSys.cpp and FileSys.h classes to implement the following functions:
* mkdir (directory) - Creates an empty subdirectory in the current directory.
* ls - List the contents of the current directory.
* cd (directory) - Change to specified directory. The directory must be a subdirectory in the current directory. No paths or ".." are allowed.
* home - Switch to the home directory.
* rmdir (directory) - Removes a subdirectory. The subdirectory must be empty.
* create (filename) - Creates an empty file of the filename in the current directory. An empty file consists of an inode and no data blocks.
* append (filename) <data> - Appends the data to the file. Data should be appended in a manner to first fill the last data block as much as possible and then allocating new block(s) ONLY if more space is needed.
* stat (name) - Displays stats for the given file or directory.
* cat (filename) - Display the contents of the file to the screen.
* rm (filename) - Removes specified file from the directory

--
Assignment 4 - Client-Server Programming
--
Creating a client and server program to play a simple treasure hunt game. Client programs can connect to the server to play the game. Clients send their guess to
the server. The server responds by sending the current game status back to the client. The server uses Seattle Universities IP to host and per this assignment I was assigned two port numbers
for testing multiple clients connecting at once as the server was inteaded to use threads to handle clients. There was also a leaderboard system in place so long as the server remained active.
