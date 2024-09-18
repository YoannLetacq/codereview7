#include "header.h"

const char *USERS = "./data/users.txt";
const char *RECORDS = "./data/records.txt";

//Clear input
void clearInputBuffer(void){
    int c;
    while ((c = getchar()) != '\n' && c !=EOF);
}

//Pause error messages to let the user have time to read it
void pauseExecution(void){
    printf("\nPress Enter to continue...\n");
    fflush(stdout);
    clearInputBuffer();
}

//display message in the terminal
void displayMessage(const char *message, int isError){
    if (isError){
        fprintf(stderr, "\033[1;31m %s \033[0m\n", message);
        fflush(stderr);
    } else {
        printf("%s\n", message);
        fflush(stdout);
    }
}