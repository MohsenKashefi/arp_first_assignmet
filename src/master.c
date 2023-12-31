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

#include "../include/common_functions.h"

void log_information(char *created_fileName, char *type);
void makingLogForDifferentThing(char *created_fileName, char *type);
void listenerForSignals(int send_signal);

int create_parent_child_process(const char *app, char *arguments[]);
int main() {
  // log data into file
   makingLogForDifferentThing("./masterLogData.txt","w+");
  log_information("masterLogData.txt","w+");

  signal(SIGINT, listenerForSignals);

  // codes related to process commands
    char * arg_window[] = { "/usr/bin/konsole", "-e", "./bin/window", NULL };
    char * argument_widnow2[] = { "/usr/bin/konsole", "-e", "./bin/watch_dog", NULL };
  

  //this code is related to creating process for components
  pid_t process_id_server = create_parent_child_process("./bin/server", NULL);
  pid_t process_id_keyboard = create_parent_child_process("./bin/keyboardManager", NULL);
  pid_t prpcess_id_drone = create_parent_child_process("./bin/drone", NULL);
  pid_t process_id_windows = create_parent_child_process("/usr/bin/konsole", argument_widnow2);
  pid_t process_id_windows2 = create_parent_child_process("/usr/bin/konsole", arg_window);
    
 //these codes are related to waiting for child process
  int status_of_prcoess;
  waitpid(process_id_server, &status_of_prcoess, 0);
  waitpid(prpcess_id_drone, &status_of_prcoess, 0);
  waitpid(process_id_keyboard, &status_of_prcoess, 0);
  waitpid(process_id_windows, &status_of_prcoess, 0);
  waitpid(process_id_windows2, &status_of_prcoess, 0);

  printf ("the status of process is %d\n", status_of_prcoess);
  return 0;
}


int create_parent_child_process(const char *app, char *arguments[]) {
    pid_t childs_process_id = fork();

    if (childs_process_id < 0) {
        perror("the mission has got truoubled...");
        return -1;
    } else if (childs_process_id != 0) {
        // This is the parent process
        return childs_process_id;
    } else {

        if (execvp(app, arguments) == -1) {
            perror("Exec failed");
            exit(EXIT_FAILURE);
        }
    }
}
