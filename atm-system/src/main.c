#include "header.h"

void initMenu(struct User *u)
{
    int r = 0;
    int option;

   
    while (!r)
    {
        system("clear");
        printf("\033[0;33m");
        printf("\n\n\t\t======= ATM =======\n");
        printf("\n\t\t-->> Feel free to login / register :\n");
        printf("\n\t\t[1]- login\n");
        printf("\n\t\t[2]- register\n");
        printf("\n\t\t[3]- exit\n");
        printf("\033[0m");
        printf("\nEnter your choice: ");
        fflush(stdout);

        if (scanf("%d", &option) != 1){
            displayMessage("Invalid input. Please enter a number.", 1);
            clearInputBuffer();
            pauseExecution();
            continue;
        }
        clearInputBuffer();

        switch (option)
        {
        case 1:
            loginMenu(u);
            const char *storedPassword = getPassword(u);
            if (storedPassword && strcmp(u->password, storedPassword) == 0){
                r = 1;
            } else {
                displayMessage("Login failed. Please try again.", 1);
            }
            free((void*)storedPassword);
            pauseExecution();
            break;
        case 2:
            if (registerMenu(u) == 1){
                displayMessage("Registration successful.", 0);
                r = 1;
            } else {
                displayMessage("Username already taken. Please choose another one.", 1);
                fflush(stdout);
                clearInputBuffer();
            }
            pauseExecution();
            break;
        case 3:
            exit(EXIT_SUCCESS);
        default:
            displayMessage("Insert a valid operation.", 1);
            pauseExecution();
        }
    }
    mainMenu(u);
}

void mainMenu(struct User *u)
{
    int option;

    while (1){
        system("clear");
        printf("\033[0;33m");
        printf("\n\n\t\t======= ATM =======\n\n");
        printf("\n\t\t-->> Feel free to choose one of the options below <<--\n");
        printf("\n\t\t[1]- Create a new account\n");
        printf("\n\t\t[2]- Update account information\n");
        printf("\n\t\t[3]- Check accounts\n");
        printf("\n\t\t[4]- Check list of owned account\n");
        printf("\n\t\t[5]- Make Transaction\n");
        printf("\n\t\t[6]- Remove existing account\n");
        printf("\n\t\t[7]- Transfer ownership\n");
        printf("\n\t\t[8]- Return to Initial Menu\n");
        printf("\n\t\t[9]- Exit\n");
        printf("\033[0m");
        printf("\nEnter your choice: ");
        fflush(stdout);

        if (scanf("%d", &option) != 1){
            displayMessage("Invalid inpout. Please choose a number.", 1);
            clearInputBuffer();
            pauseExecution();
            continue;
        }
        clearInputBuffer();

        switch (option)
        {
        case 1:
            createNewAcc(*u);
            break;
        case 2:
            updateAccount(*u);
            break;
        case 3:
            checkAllAccounts(*u);
            break;
        case 4:
            checkOwnedAccounts(*u);
            break;
        case 5:
            makeTransaction(*u);
            break;
        case 6:
            removeAccount(*u);
            break;
        case 7:
            transferOwnership(*u);
            break;
        case 8:
            initMenu(u);
            return;
        case 9:
            displayMessage("Thank you for using my ATM program. Ciao.", 0);
            exit(EXIT_SUCCESS);
        default:
            displayMessage("Invalid operation", 1);
            pauseExecution();
        }
    }
}

int main()
{
    struct User u = {0};

    initMenu(&u);

    return 0;
}
