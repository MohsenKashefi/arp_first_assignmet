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

// parameters for drone movement
double Mass = 1.0, AF = 0.1, dt = 0.1, Force_x = 0, Force_y = 0;

// condtions of drone in initializing
double x_p = 10.0, y_p = 10.0, x_velocity = 0.0, y_velocity = 0.001;

//this function is for calculating drone movement
void computionOfdroneMovement();

void Drone_Movement_function(Spot *locOfDrone, char directionOfDrone)
{

    switch (directionOfDrone)
    {
    case 'u':
        Force_x += -1;
        ("keyboardLogData.txt", "w+");
        break;
    case 'j':
        Force_x += 1;
        log_information("keyboardLogData.txt", "w+");
        break;
    case 'k':
        Force_y += -1;
        log_information("keyboardLogData.txt", "w+");
        break;
    case 'h':
        Force_y += 1;
        log_information("keyboardLogData.txt", "w+");
        break;
    case 'i':
        Force_x += -sqrt(2) / 2;
        Force_y += sqrt(2) / 2;
        break;
    case 'y':
        Force_x += sqrt(2) / 2;
        Force_y += -sqrt(2) / 2;
        log_information("keyboardLogData.txt", "w+");
        break;
    case 'l':
        Force_x += sqrt(2) / 2;
        Force_y += sqrt(2) / 2;
        log_information("keyboardLogData.txt", "w+");
        break;
    case 'g':
        Force_x += -sqrt(2) / 2;
        Force_y += -sqrt(2) / 2;
        log_information("keyboardLogData.txt", "w+");
        break;
    case '0':
        Force_x = 0;
        Force_y = 0;
        x_velocity = 0;
        y_velocity = 0;
        log_information("keyboardLogData.txt", "w+");
        break;
    }
    // make changes
    computionOfdroneMovement();

    // Update location
    locOfDrone->x = x_p;
    locOfDrone->y = y_p;
}
int main()
{
    makingLogForDifferentThing("./keyboardLogData.txt", "w+");
    log_information("keyboardLogData.txt", "w+");
    signal(SIGINT, listenerForSignals);

    sharedMemory_fd = shm_open("/blackboardsection", O_RDWR, 0666);
    sharedMemory = (BlackBoard_Section *)mmap(NULL, sizeof(BlackBoard_Section), 
    PROT_READ | PROT_WRITE, MAP_SHARED, sharedMemory_fd, 0);

    while (1)
    {
        // Move drone based on user input
        char ch = sharedMemory->pressed_key;
        Drone_Movement_function(&(sharedMemory->drone), ch);
        usleep(100000);
        // set unsleep rate
    }

    munmap(sharedMemory, sizeof(BlackBoard_Section));
    close(sharedMemory_fd);
    return 0;
}

//this function is related to calculation of drone movement
void computionOfdroneMovement()
{
    
    x_velocity += (Force_x - AF * (x_velocity)) / Mass * dt;
    y_velocity += (Force_y - AF * (y_velocity)) / Mass * dt;

    // this line of code is related to updating position
    x_p += x_velocity * dt + 0.5 * ((x_velocity - AF * x_velocity) / Mass) * dt * dt;
    y_p += y_velocity * dt + 0.5 * ((y_velocity - AF * y_p) / Mass) * dt * dt;

    //related to boundry
    //about reversing drone 
    if (x_p < MIN_OF_X || x_p > MAX_OF_X)
    {
    
        x_velocity = -(x_velocity); 
        
    }
    if (y_p < MIN_OF_Y || y_p > MAX_OF_Y)
    {
        y_velocity = -(y_velocity); 
    }
}