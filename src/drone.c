#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <signal.h>
#include <time.h>
#include <fcntl.h> 
#include <unistd.h> 
#include <sys/stat.h> 
#include <sys/mman.h>
#include <sys/types.h> 
#include <sys/select.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <errno.h>
#include <termios.h>

#include "../include/common_functions.h"
#include "../include/blackboard_section.h"
#include "../include/const.h"
#include <sys/mman.h>
#include <math.h>


// shared memory config
int sharedMemory_fd;
BlackBoard_Section *sharedMemory;


int main()
{
    makingLogForDifferentThing("./droneLogData.txt", "w+");
    log_information("droneLogData.txt", "w+");

    signal(SIGINT, listenerForSignals);

    sharedMemory_fd = shm_open("/blackboardsection", O_RDWR, 0666);
    sharedMemory = (BlackBoard_Section *)mmap(NULL, 
    sizeof(BlackBoard_Section), PROT_READ | PROT_WRITE, MAP_SHARED, sharedMemory_fd, 0);


    munmap(sharedMemory, sizeof(BlackBoard_Section));
    close(sharedMemory_fd);
    return 0;
    return 0;
}
