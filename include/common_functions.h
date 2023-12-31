#ifndef common_functions_H
#define common_functions_H

#include <stdio.h>
#include <unistd.h>
void log_information(char *created_fileName, char *type);
void makingLogForDifferentThing(char *created_fileName, char *type);
void listenerForSignals(int send_signal);

#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

void listenerForSignals(int send_signal) {
    printf("it is a function for receiving and handling signals\n");
    printf("the signal number is %d \t", send_signal);

    if (send_signal == SIGINT || send_signal == SIGUSR2) {
        printf("the received signal is %s\n", (send_signal == SIGINT) ? "SIGINT" : "SIGUSR2");
        printf("After this, the process will be terminated\n");
        kill(getpid(), SIGKILL);
        printf("the process is killed\n");
    }
}

void makingLogForDifferentThing(char *created_fileName, char *type) {
    FILE *file;
    if (!remove(created_fileName)) {
        printf("previous file is deleted\n");
    }
    if ((file = fopen(created_fileName, type))) {
        printf("a file is created for printing logs\n");
        log_information(created_fileName, type);
    } else {
        printf("unable to create file!\n");
    }
}

void log_information(char *created_fileName, char *type) {
    printf("log_information\n");
    FILE *file;

    // this line of code gets the process id
    int pid = getpid();

    time_t rawTime;
    struct tm *timeInfo;

    time(&rawTime);
    timeInfo = localtime(&rawTime);

    // Open the logfile
    file = fopen(created_fileName, type);

    // Error Checking
    if (file == NULL) {
        perror("an error has occurred about opening file");
        exit(EXIT_FAILURE);
    }

    // Write into the file
    fprintf(file, "Process_ID: %d \n time is: %02d:%02d:%02d\n", pid, timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec);

    fclose(file);
}

#endif