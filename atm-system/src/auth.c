#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

void loginMenu(struct User *u)
{
    struct termios oflags, nflags;

    system("clear");
    printf("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\t User Login: ");
    scanf("%s", u->name);

    FILE *fp = fopen(USERS, "r");
    if (!fp){
      perror("Erro opening users file");
      exit(EXIT_FAILURE);  
    }

    struct User tempUser;
    int userExists = 0;
    while (fscanf(fp, "%d %s %s", &tempUser.id, tempUser.name, tempUser.password) != EOF) {
        if (strcmp(tempUser.name, u->name) == 0) {
            userExists = 1;
            break;
        }
    }
    fclose(fp);

    if (!userExists) {
        displayMessage("Username does not exist.", 1);
        fflush(stdout);
        clearInputBuffer();
        return;
    }
    
    // disabling echo
    tcgetattr(fileno(stdin), &oflags);
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;
    tcsetattr(fileno(stdin), TCSANOW, &nflags);

    printf("\n\n\n\n\n\t\t\t\tEnter the password to login: ");
    scanf("%49s", u->password);

    // restore terminal
    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0)
    {
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }

    // Verify login credentials and set user ID
    fp = fopen(USERS, "r");
    if (!fp) {
        perror("Error opening users file");
        exit(EXIT_FAILURE);
    }

    int loginSuccess = 0;
    while (fscanf(fp, "%d %s %s", &tempUser.id, tempUser.name, tempUser.password) != EOF) {
        if (strcmp(tempUser.name, u->name) == 0 && strcmp(tempUser.password, u->password) == 0) {
            u->id = tempUser.id;  // Set the correct user ID after successful login
            loginSuccess = 1;
            break;
        }
    }
    fclose(fp);

    if (loginSuccess) {
        displayMessage("Login successful.", 0);
        fflush(stdout);
        clearInputBuffer();
    } else {
        displayMessage("Wrong password.", 1);
        fflush(stdout);
        clearInputBuffer();
    }
}

const char *getPassword(struct User *u)
{
    FILE *fp;
    if ((fp = fopen(USERS, "r")) == NULL)
    {
        perror("Error opening users file");
        exit(EXIT_FAILURE);
    }

    struct User userChecker;
    while (fscanf(fp, "%d %s %s", &userChecker.id, userChecker.name, userChecker.password) != EOF)
    {
        if (strcmp(userChecker.name, u->name) == 0)
        {
            fclose(fp);
            char *buff = malloc(strlen(userChecker.password)+1); //returns a copy of the password to avoid returning a local variable 
            if (!buff) {
                perror("Memorry allocation failed");
                exit(EXIT_FAILURE);
            }
            strcpy(buff, userChecker.password);
            return buff;
        }
    }

    fclose(fp);
    return NULL;
}

int registerMenu(struct User *newUser){
    FILE *fp;
    if ((fp = fopen(USERS, "r")) == NULL){
        perror("Error opening users file");
        exit(EXIT_FAILURE);
    }

    displayMessage("Enter username: ", 0);
    scanf("%49s", newUser->name);

    struct User userChecker;
    int userExists = 0;
    int maxUserId = -1;

    while (fscanf(fp, "%d %s %s", &userChecker.id, userChecker.name, userChecker.password) != EOF){
        if (strcmp(userChecker.name, newUser->name) == 0){
            userExists = 1;
            break;
        }
        if (userChecker.id > maxUserId){
            maxUserId = userChecker.id;
        }
    }
    fclose(fp);

    if (userExists){
        return 0;
    }

    displayMessage("Enter password: ", 0);
    scanf("%49s", newUser->password);

    fp = fopen(USERS, "a");
    if (!fp) {
        perror("Error opening users file");
        exit(EXIT_FAILURE);
    }

    newUser->id = maxUserId +1;

    fprintf(fp, "%d %s %s\n", newUser->id, newUser->name, newUser->password);
    fclose(fp);

    return 1;
}

