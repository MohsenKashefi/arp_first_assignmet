
#include "../include/const.h"
#include "../include/common_functions.h"

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <signal.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include "../include/const.h"

int *pointer;
char *memoryObjectName =  "blackboardsection";  

int socket_fd_descriptor,
 newsocket_fd_descriptor,
  client_user;
struct sockaddr_in serverAddress, clientAddress;

int* initializeSharedMemory(const char *memoryObjectName);

void KillingSingnalsHandle(int signo);
void log_information(char * fileName, char * mode);
void makingLogForDifferentThing(char * fileName, char * mode);

int main(int argc, char *argv[])
{   
   //log data into file
  makingLogForDifferentThing("./serverLogData.txt","w+");
  /*Logging the data*/
  log_information("serverLogData.txt","w+");
 //handle signlas
  signal(SIGINT, KillingSingnalsHandle);

    //Creates a shared memory
    pointer = initializeSharedMemory(memoryObjectName);

    //Creates a socket
    socket_fd_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    //Error Handling
    if (socket_fd_descriptor < 0) 
        perror("An error occured in opening socket");

    //Config the server address
    bzero((char *) &serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(3500);

    //Bind the socket to the IP address
    if (bind(socket_fd_descriptor, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) 
        perror("an error occurred in binding");
            
    //Server is ready! Waits for new client to request
    printf ("the server is ok , it is ready for accepting request...\n");
    listen(socket_fd_descriptor,5);

    //Accepts the request
    client_user = sizeof(clientAddress);
    newsocket_fd_descriptor = accept(socket_fd_descriptor, (struct sockaddr *) &clientAddress, &client_user);
    printf ("the request is accepeted\n");

    //Error Handling
    if (newsocket_fd_descriptor < 0)
        perror("An error occured in accepting request");
}

// in this function , there are related code to making sh memroy
int* initializeSharedMemory(const char *memoryObjectName){
  // make object from sh memory
  int shm_fd = shm_open(memoryObjectName, O_CREAT | O_RDWR, 0666);

  //write this below code for handling sh memory
  if(shm_fd == -1 ){  
    printf("error in creating sh memory!");
    exit(1);
  }

  // config sh memory size
  if(ftruncate(shm_fd, Size_Of_Shared_Memory) == -1)
    printf("error related to config sh memory size\n\n");

  //the code is related to maping process to pointer
  int *pointer = (int *)mmap(NULL, Size_Of_Shared_Memory, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

   //this line is for handling error
  if (pointer == MAP_FAILED) {
    printf("oh , there is an error related to map");
  }

    // this line return the pointer related to sh memory
    return pointer;
}


void KillingSingnalsHandle(int signo){
    if (signo == SIGINT){
        printf("the SIGINT signla is recieved!\n");

    

    // in this line we unmap sh memory
      if (munmap(pointer, Size_Of_Shared_Memory) == -1) {
          perror("error unmap sh memory");
          exit(EXIT_FAILURE);
      }

      //condtion relatede to closing sh memory
      if (close(*pointer) == -1) {
          perror("error closen sh memory");
          exit(EXIT_FAILURE);
      }

  
      if (shm_unlink(memoryObjectName) == -1) {
          perror("there is an error in deleting sh memory");
          exit(EXIT_FAILURE);
      }

      
      if (close(socket_fd_descriptor) == -1) {
          perror("there is an error in closing socket_fd");
          exit(EXIT_FAILURE);
      }

      
        kill(getpid(), SIGKILL);
    }
}

