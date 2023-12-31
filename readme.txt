this project is made by diffrenet component and also some heeader which i placed in the include folder and
our components are in the src folder .

they are some function which is used multiple times in project , I Wrote them in a header file and used them.
thre is a section which is related to blackboard and we use it in other components.
for the constant file,there is const.h which included the constant things.

About the components :
first of all , I want to explain about master.c component which is a main section.

This master.c which is a main section creates multiple child processes using fork() 
and execvp(). It spawns processes for a server, drone
and two instances of a Konsole window. The main function waits for all child 
processes to complete using waitpid(). The program also includes signal handling 
for SIGINT and functions for logging information. The code structure suggests a 
simulation or control system involving different components. Additionally, 
there are some typos and errors in the code, such as the incorrect file name in the 
log_information function and an unnecessary 
semicolon after the if statement in the spawn function.


at the next step I want to go throgh the drone.c component
the drone.c is 
This C program initializes and configures shared memory 
for inter-process communication using POSIX shared memory functions. 
It establishes a connection to a shared memory segment named "/blackboard," 
maps it into the program's address space, and then unmaps and closes the shared memory 
at the end. Additionally, it sets up signal handling for interrupt signals (SIGINT) 
and logs information to a file named "droneLogData.txt." 
There's a redundant "return 0;" statement at the end.


next component is keyboardManager , 
the keyboardManager.c is a file 
This C program implements a simple drone motion simulation using 
shared memory for communication and user input for controlling the drone. 
It includes configuration for shared memory, drone movement parameters, and 
conditions, and computes the drone's motion numerically. The Drone_Movement_function 
interprets user input to apply forces and logs the actions. The main loop continuously 
updates the drone's position based on user input received through shared memory. 
The computionOfdroneMovement function calculates velocities and updates the drone's 
position, ensuring boundary conditions are handled appropriately. Additionally, 
the program uses signal handling for interrupt signals (SIGINT) and logs information to a 
file named "keyboardLogData.txt." The code has a minor typo in the comment "//make changes," 
and there's a redundant "return 0;" statement at the end of the main function.


about server.c component
the server.c component is This C program establishes a server 
using sockets, creating shared memory, and handling signals. 
It logs information, initializes shared memory, and sets up a socket 
for communication. The server binds to an IP address, listens for 
client requests, and accepts connections. Upon receiving a SIGINT signal,
 it gracefully releases resources, including unmapping shared memory, 
 closing sockets, and unlinking shared memory objects. There is a minor 
 issue where the program attempts to close an integer (close(*pointer)) 
 instead of a file descriptor. Additionally, 
the bzero function is outdated; consider using memset instead.

next component is watch_dog.c :
This C program continuously monitors the activity status of 
specified processes by reading their last recorded activity time from
 log files. It evaluates their inactivity over a specified duration and 
 takes actions accordingly. If a process is inactive for a certain period, 
 it prints information about it. The program repeats this process until all 
 monitored processes become inactive, at which point it kills them. There are
  minor issues such as redundant function declaration and incomplete signal 
  handling registration. Additionally, 
more robust parsing methods for file content may be considered.


window.c component 
 window.c component is continuously monitors the activity status of specified processes by reading 
their last recorded activity time from log files. It evaluates their 
inactivity over a specified duration and takes actions accordingly. 
If a process is inactive for a certain period, it prints information about it. 
The program repeats this process until all monitored processes become inactive,
 at which point it kills them. There are minor issues such as redundant function
  declaration and incomplete signal handling registration. 
Additionally, more robust parsing methods for file content may be considered.