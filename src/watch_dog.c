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
#include <sys/wait.h>
#include <stdbool.h>
#include <errno.h>
#include "../include/const.h"

void handling_signlas(int signo);
bool checkTime(int the_hour_of_beging, int the_minutes_of_beging, int the_second_of_beging, int durationSeconds);
void handling_signlas(int signo);

int main(int argc, char const *argv[])
{
  FILE *file;
  char *token, buffer[100];
  int parentID_process, last_active_h, last_active_minutes, last_active_second;
  bool pActivityStatus;

  int inactive_pCounter = 0;
  char log_addr
      [5][29] = {"./droneLogData.txt", "./serverLogData.txt", "./windowLogData.txt", "./masterLogData.txt", "./keyboardLogData.txt"};
  char ppid_list
      [5][20] = {"", "", "", "", ""};

  while (1)
  {
    sleep(DURATION_OF_CHECK); // wait for 60 seconds then check the activity staatus of the process
    while (inactive_pCounter < INACTIVE_NUM_PROCESS)
    {
      /*Opening the logfile of the target process to read its PID */
      file = fopen(log_addr
                       [inactive_pCounter],
                   "r");
      /*Error Checking*/
      if (file < 0)
      {

        printf("there is an error in opening maxCommand =%d\n", errno);
        exit(1);
      }

      // fseek function relatee to the cursor
      fseek(file, 0, SEEK_SET);
      // the function reading bytes from file
      fread(buffer, 50, 1, file);

      fclose(file);
      // file closed

      // this function is for extraction file from target process
      token = strtok(buffer, ",");
      strcpy(ppid_list
                 [inactive_pCounter],
             token);
      parentID_process = atoi(token);

      token = strtok(NULL, ",");
      last_active_h = atoi(token);

      token = strtok(NULL, ",");
      last_active_minutes = atoi(token);

      token = strtok(NULL, ",");
      last_active_second = atoi(token);

      // last activity printing
      printf("last activity time of this prcoess %d is hour : minutes : seconds:%d:%d:%d \n", parentID_process, last_active_h, last_active_minutes, last_active_second);

      printf("the status of activity in the last 50 seconds is\n");
      pActivityStatus = checkTime(last_active_h, last_active_minutes, last_active_second, TIME_LIMITATION_FOR_INACTIVE_TIME);

      if (pActivityStatus)
      { /*If the process was inactive in the last 60 seconds*/
        /*Print information of the inactive process */
        printf("Process number %d was deactive more than 50 sec... :(\n\n\n", parentID_process);

        inactive_pCounter = inactive_pCounter + 1;
        // one number is added in inactive process
        if (inactive_pCounter == INACTIVE_NUM_PROCESS)
        {

          printf("all of the process is killed :|\n");
          for (int i = 0; i < INACTIVE_NUM_PROCESS; i++)
          {

            printf("the list of killed process is%s\n", ppid_list
                                                            [i]);
            kill(atoi(ppid_list
                          [i]),
                 SIGINT);
          }
        }
      }
      else
      {
        /*if atleast one process was active in the last 60 seconds, ignore the cheking process */
        printf("Process number %d is active more than :)\n\n\n", parentID_process);
        inactive_pCounter = 0;
        break;
      }
    }
  }
  return 0;
}

// hanlding signal function
void handling_signlas(int signo)
{
  printf("handling singlas functions\n\n");
  if (signo == SIGINT)
  {
    printf("the (((SIGINT))) signal is recieved!\n");
    kill(getpid(), SIGKILL);
  }
}

bool checkTime(int the_hour_of_beging, int the_minutes_of_beging, int the_second_of_beging, int durationSeconds)
{
  time_t time_var = time(NULL);
  struct tm get_time = *localtime(&time_var);

  bool activity_status = (get_time.tm_hour - the_hour_of_beging) * 3600 +
                             (get_time.tm_min - the_minutes_of_beging) * 60 + 
                             (get_time.tm_sec - the_second_of_beging) >
                         durationSeconds;
  if (activity_status)
  {
    printf("end of time\n");
    return true;
  }
  return false;
}
