#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <errno.h>

#include "../include/common_functions.h"
#include "../include/blackboard_section.h"
#include "../include/const.h"

int sharedMemory_fd;
BlackBoard_Section *sharedMemory;

// this function is related to handling signals
void handle_signals(int signo)
{
    if (signo == SIGINT)
    {
        printf("the (((SIGINT))) signla is recieved!\n");

        munmap(sharedMemory, sizeof(BlackBoard_Section));
        close(sharedMemory_fd);
        // sh memory is closed

        kill(getpid(), SIGKILL);
    }
}

// this function is for designing border of app based on width and height size
void design_border_for_app()
{

    attron(COLOR_PAIR(3)); // this is the blue color
    for (int i = 0; i < BORDERW; ++i)
    {
        //draw top border
        mvaddch(0, i, '#');  
        //draw bottom border         
        mvaddch(BORDERH - 1, i, '#'); 
    }
    attroff(COLOR_PAIR(3));

    attron(COLOR_PAIR(3));
    for (int i = 0; i < BORDERH; ++i)
    {
        //draw left border 
        mvaddch(i, 0, '#');
        //draw right border           
        mvaddch(i, BORDERW - 1, '#'); 
    }
    attroff(COLOR_PAIR(3));
}

// Function to initialize colors in NCurses
void initColors()
{
    if (has_colors())
    {
        start_color();
        init_pair(1, COLOR_BLUE, COLOR_BLACK);
        init_pair(2, COLOR_GREEN, COLOR_BLACK);
        init_pair(3, COLOR_YELLOW, COLOR_BLACK);
        init_pair(4, COLOR_RED, COLOR_BLACK);
    }
}

// draw character
void design_char_of_app(ChatacterOfApp *character_Of_app)
{
    wattron(stdscr, COLOR_PAIR(character_Of_app->displayColor));
    mvaddch(character_Of_app->row, character_Of_app->column, character_Of_app->displaySymbol);
    wattroff(stdscr, COLOR_PAIR(character_Of_app->displayColor));
}


void get_pressed_key(BlackBoard_Section *sh)
{
    //get input for moving drone
    char character = getch();
    sh->pressed_key = character;
}

int main()
{

    makingLogForDifferentThing("./windowLogData.txt", "w+");
    log_information("windowLogData.txt", "w+");

    signal(SIGINT, handle_signals);
    //resizing
    int first_resize = TRUE;

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);  
    timeout(200); 

    initColors();

    sharedMemory_fd = shm_open("/blackboardsection", O_RDWR, 0666);
    sharedMemory = (BlackBoard_Section *)mmap(NULL, sizeof(BlackBoard_Section), PROT_READ | PROT_WRITE, MAP_SHARED, sharedMemory_fd, 0);

    // in this loop we call designing drawing borders
    while (1)
    {
        clear(); // function for clearing the screen

        // this function draw border for window
        design_border_for_app();

        // Draw drone for window
        ChatacterOfApp drone_character = {
            .row = sharedMemory->drone.x,
            .column = sharedMemory->drone.y,
            .displaySymbol = '*',
            .displayColor = 1,
            .isActive = 1};
        design_char_of_app(&drone_character);

        // Refresh the screen
        refresh();

        // the function is related to moving dron with the user input
        get_pressed_key(sharedMemory);
    }

    munmap(sharedMemory, sizeof(BlackBoard_Section));
    close(sharedMemory_fd);
    endwin();
    return 0;
}
