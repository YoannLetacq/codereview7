#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

extern const char *USERS;
extern const char *RECORDS;

struct Date
{
    int month, day, year;
};

// all fields for each record of an account
struct Record
{
    int id;
    int userId;
    char name[100];
    char country[100];
    int phone;
    char accountType[10];
    long long int accountNbr;
    double amount;
    struct Date deposit;
    struct Date withdraw;
};

struct User
{
    int id;
    char name[50];
    char password[50];
};

// authentication functions
void loginMenu(struct User *u);
int registerMenu(struct User *newUser);
const char *getPassword(struct User *u);

// system function
void saveAccountToFile(FILE *ptr, struct User u, struct Record r);
int getAccountFromFile(FILE *ptr, struct Record *r);
void success(struct User u, void (*f)(struct User));
void createNewAcc(struct User u);
void checkAllAccounts(struct User u);
void checkOwnedAccounts(struct User u);
void updateAccount(struct User u);
void removeAccount(struct User u);
void transferOwnership(struct User u);
void makeTransaction(struct User u);

//menu navigation
void mainMenu(struct User *u);
void stayOrReturn(int notGood, void (*f)(struct User), struct User u);

//display messages in the terminal
void displayMessage(const char *message, int isError);

//Clear input
void clearInputBuffer();

//Pause error messages to let the user have time to read it
void pauseExecution();


#endif // HEADER_H