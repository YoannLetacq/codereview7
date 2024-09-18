#include "header.h"
#include <time.h>


int getAccountFromFile(FILE *ptr, struct Record *r)
{
    return fscanf(ptr, "%d %d %s %lld %d/%d/%d %s %d %lf %s",
                &r->id,
		        &r->userId,
		        r->name,
                &r->accountNbr,
                &r->deposit.month,
                &r->deposit.day,
                &r->deposit.year,
                r->country,
                &r->phone,
                &r->amount,
                r->accountType) != EOF;
}

void saveAccountToFile(FILE *ptr, struct User u, struct Record r)
{
    fprintf(ptr, "%d %d %s %lld %d/%d/%d %s %d %.2lf %s\n",
            r.id,
	        u.id,
	        u.name,
            r.accountNbr,
            r.deposit.month,
            r.deposit.day,
            r.deposit.year,
            r.country,
            r.phone,
            r.amount,
            r.accountType);
}

void stayOrReturn(int notGood, void (*f)(struct User), struct User u) {
    int option;

    if (notGood == 0) {
        system("clear");
        displayMessage("\n✖ Record not found.", 1);
    }

    while (1) {
        printf("\nEnter 0 to try again, 1 to return to main menu, and 2 to exit: ");
        
        // Validate input
        if (scanf("%d", &option) != 1) {
            clearInputBuffer();
            while (getchar() != '\n'); // Clear the input buffer
            displayMessage("Invalid input. Please enter a number.", 1);
            continue;
        }

        system("clear");

        switch (option) {
            case 0:
                f(u);
                return; 
            case 1:
                mainMenu(&u);
                return; 
            case 2:
                exit(EXIT_SUCCESS);
            default:
                displayMessage("Insert a valid operation.", 1);
                break;
        }
    }
}

void success(struct User u, void (*f)(struct User)) {
    int option;

    while (1) {
        displayMessage("✔ Success!", 0);
        printf("Enter 1 to go to the main menu and 0 to exit: ");
        
        // Validate input
        if (scanf("%d", &option) != 1) {
            while (getchar() != '\n'); 
            displayMessage("Invalid input. Please enter a number.", 1);
            continue;
        }

        system("clear");

        switch (option) {
            case 1:
                mainMenu(&u);
                return; 
            case 0:
                exit(EXIT_SUCCESS);
            default:
                displayMessage("Insert a valid operation!", 1);
                stayOrReturn(0, f, u); 
                return; 
        }
    }
}

void createNewAcc(struct User u)
{
    struct Record r;
    struct Record cr;
    FILE *pf = fopen(RECORDS, "a+");

    if (!pf){
        displayMessage("Error opening file.", 1);
        return;
    }

    system("clear");
    displayMessage("\t\t\t===== New record =====\n", 0);

    printf("\nEnter today's date(mm/dd/yyyy):");
    scanf("%d/%d/%d", &r.deposit.month, &r.deposit.day, &r.deposit.year);

    int accountExists;
    do {
        accountExists = 0;
        printf("\nEnter the account number: ");
        scanf("%lld", &r.accountNbr);

        // Check if the account number already exists in the file
        rewind(pf);  // Reset the file pointer to the beginning
        while (getAccountFromFile(pf, &cr)) {
            if (cr.accountNbr == r.accountNbr) {
                accountExists = 1;
                displayMessage("✖ This account number already exists. Please try a different number.", 1);
                pauseExecution();
                break;
            }
        }
    } while (accountExists);

    printf("\nEnter the country:");
    scanf("%s", r.country);
    printf("\nEnter the phone number:");
    scanf("%d", &r.phone);
    printf("\nEnter amount to deposit: $");
    scanf("%lf", &r.amount);
    printf("\nChoose the type of account:\n\t-> saving\n\t-> current\n\t-> fixed01(for 1 year)\n\t-> fixed02(for 2 years)\n\t-> fixed03(for 3 years)\n\n\tEnter your choice:");
    scanf("%s", r.accountType);

     // Use the user ID from the user structure
    r.userId = u.id;
    strcpy(r.name, u.name);  // Assign user name

    // Get the last ID and increment it
    int lastId = -1;
    rewind(pf);
    while (getAccountFromFile(pf, &cr)) {
        if (cr.id > lastId) {
            lastId = cr.id;
        }
    }
    r.id = lastId + 1;

    saveAccountToFile(pf, u, r);

    fclose(pf);
    success(u, createNewAcc);
}

void checkAllAccounts(struct User u)
{
    struct Record r;
    FILE *pf = fopen(RECORDS, "r");

    if (!pf) {
        displayMessage("Error opening file.", 1);
        pauseExecution();
        return;
    }

    system("clear");
    printf("\033[0;33m");
    printf("\t\t====== All accounts from user, %s =====\n\n", u.name);
    
    int found = 0;
    while (getAccountFromFile(pf, &r)) {
        if (strcmp(r.name, u.name) == 0) {
            found = 1;
            printf("_____________________\n");
            printf("\nAccount number:%lld\nDeposit Date:%d/%d/%d \ncountry:%s \nPhone number:%d \nAmount deposited: $%.2f \nType Of Account:%s\n",
                   r.accountNbr,
                   r.deposit.day,
                   r.deposit.month,
                   r.deposit.year,
                   r.country,
                   r.phone,
                   r.amount,
                   r.accountType);
        }
    }
    printf("\033[0m");
    fclose(pf);

    if (!found) {
        printf("\033[1;31m\n No account found for %s.\033[0m\\n", u.name);
    }
    success(u, checkAllAccounts);
}

double calculateInterest(double amount, double rate, int years){
    return amount * (rate/100) * years;
}

void displayAccountDetails(struct Record account){
    printf("\033[0;33m");
    printf("\n==== Account Details ====\n");
    printf("Account Number: %lld\n", account.accountNbr);
    printf("Account Type: %s\n", account.accountType);
    printf("Deposit Amount: $%.2f\n", account.amount);
    printf("Deposit Date: %02d/%02d/%04d\n", account.deposit.month, account.deposit.day, account.deposit.year);
    printf("Country: %s\n", account.country);
    printf("Phone Number: %d\n", account.phone);
    printf("\033[0m");

    double interestRate = 0.0;
    int interestDuration = 0;
    int nextYear = account.deposit.year;

    switch(account.accountType[0]){
        case 's':
            if (strcmp(account.accountType, "saving") == 0){
                interestRate = 7.0;
                double monthlyInterest = calculateInterest(account.amount, interestRate, 1)/12;
                printf("You'll get $%.2f of interest on day %d of every month.\n", monthlyInterest, account.deposit.day);
                break;
            }
            goto default_case;
        case 'f':
            if (strcmp(account.accountType, "fixed01") == 0){
                interestRate = 4.0;
                interestDuration = 1;
            } else if (strcmp(account.accountType, "fixed02") == 0){
                interestRate = 5.0;
                interestDuration = 2;
            } else if (strcmp(account.accountType, "fixed03") == 0){
                interestRate = 8.0;
                interestDuration = 3;
            } else {
                goto default_case;
            }
            nextYear += interestDuration;
            double totalInterest = calculateInterest(account.amount, interestRate, interestDuration);
            printf("You'll get $%.2f of interest on %02d/%02d/%04d\n", totalInterest, account.deposit.month, account.deposit.day, nextYear);
            break;
        case 'c':
            if (strcmp(account.accountType, "current") == 0){
                displayMessage("You won't get any interest because the account is of type current.\n", 1);
                return;
            }
            goto default_case;
        default:
            default_case:
                displayMessage("unknown account type\n", 1);
                return;
    } 
}

void checkOwnedAccounts(struct User u){
    struct Record r;
    FILE *pf = fopen(RECORDS, "r");

    if (!pf){
        displayMessage("Error opening file.", 1);
        pauseExecution();
        return;
    }

    system("clear");
    printf("\033[0;33m");
    printf("\t\t====== List of owned accounts for %s =====\n\n", u.name);

    struct Record accounts[100];
    int accountCount = 0;

    while (getAccountFromFile(pf, &r)){
        if (strcmp(r.name, u.name) == 0){
           accounts[accountCount++] = r;
            printf("[%d] Account Number: %lld\n", accountCount, r.accountNbr);
        }
    }
     printf("\033[0m");

    if (accountCount == 0){
        printf("\033[1;31m\nNo accounts found for %s.\033[0m\n", u.name);
        fclose(pf);
        stayOrReturn(0, checkOwnedAccounts, u);
        return;
    }

    int selectedIndex;
    printf("\nEnter the number of the account to view details (1-%d): ", accountCount);
    scanf("%d", &selectedIndex);

    if (selectedIndex < 1 || selectedIndex > accountCount){
        displayMessage("Invalid selection.", 1);
        fclose(pf);
        stayOrReturn(0, checkOwnedAccounts, u);
        return;
    }
    system("clear");
    displayAccountDetails(accounts[selectedIndex - 1]);
    fclose(pf);
    success(u, checkOwnedAccounts);
   
}

void updateAccount(struct User u){
    FILE *pf = fopen(RECORDS, "r+");
    FILE *tempFile = fopen("temp.dat", "w");
    if (!pf || !tempFile) {
        printf("Error opening file .\n");
        if (pf) fclose(pf);
        if (tempFile) fclose(tempFile);
        return;
    }

    long long int accountNbr;
    printf("Enter the account number to update: ");
    scanf("%lld", &accountNbr);

    struct Record r;
    int found = 0;

    while (getAccountFromFile(pf, &r)) {
        if (r.accountNbr == accountNbr && strcmp(u.name, r.name) == 0) {
            found = 1;
            int choice;
            int changesMade = 0;

            do {
                displayMessage("Select the piece of information to update: ", 0);
                displayMessage("1. Country", 0);
                displayMessage("2. Phone Number", 0);
                displayMessage("3. Exit to main menu.", 0);
                printf("Enter your choice: ");
                scanf("%d", &choice);

                switch (choice){
                    case 1:
                        printf("Enter new country: ");
                        scanf("%s", r.country);
                        changesMade = 1;
                        break;
                    case 2:
                        printf("Enter new phone number: ");
                        scanf("%d", &r.phone);
                        changesMade = 1;
                        break;
                    case 3:
                        if (!changesMade) {
                            displayMessage("No changes made. Exiting to main menu.", 1);
                        } else {
                            displayMessage("Saving changes...", 0);
                            fprintf(tempFile, "%d %d %s %lld %d/%d/%d %s %d %.2lf %s\n",
                                    r.id,
                                    r.userId,
                                    r.name,
                                    r.accountNbr,
                                    r.deposit.month,
                                    r.deposit.day,
                                    r.deposit.year,
                                    r.country,
                                    r.phone,
                                    r.amount,
                                    r.accountType);
                        }
                        fclose(pf);
                        fclose(tempFile);
                        if (changesMade) {
                            if(remove(RECORDS) != 0){
                                perror("Error deleting original file");
                            } else if (rename("temp.dat", RECORDS) != 0) {
                                perror("Error renaming temporary file");
                            } else {
                                displayMessage("Changes saved successfully.", 0);
                            }
                        } else {
                            remove("temp.dat");
                        }
                        return;
                    default:
                        displayMessage("Invalid choice.", 1);
                        break;
                }
            } while (choice !=3);
            
            if (!changesMade){
                fprintf(tempFile, "%d %d %s %lld %d/%d/%d %s %d %.2lf %s\n",
                    r.id,
                    r.userId,
                    r.name,
                    r.accountNbr,
                    r.deposit.month,
                    r.deposit.day,
                    r.deposit.year,
                    r.country,
                    r.phone,
                    r.amount,
                    r.accountType);
            }
        } else {
            fprintf(tempFile, "%d %d %s %lld %d/%d/%d %s %d %.2lf %s\n",
                    r.id,
                    r.userId,
                    r.name,
                    r.accountNbr,
                    r.deposit.month,
                    r.deposit.day,
                    r.deposit.year,
                    r.country,
                    r.phone,
                    r.amount,
                    r.accountType);
        }
    }

    if (!found) {
        displayMessage("Record not found.", 1);
        fclose(pf);
        stayOrReturn(0, updateAccount, u);
        return;
    }

    fclose(pf);
    fclose(tempFile);

    if (remove(RECORDS) != 0) {
        perror("Error deleting original file");
        return;
    }

    if (rename("temp.dat", RECORDS) != 0) {
        perror("Error renaming temporary file");
        return;
    }

    if (found) {
        displayMessage("Account updated successfully.", 0);
    } else {
        displayMessage("Account not found.", 1);
    }
    success(u, updateAccount);
    
}

void removeAccount(struct User u){
    long long int accountNbr;
    printf("Enter the number account to remove: ");
    scanf("%lld", &accountNbr);

    FILE *pf = fopen(RECORDS, "r");
    FILE *tempFile = fopen("temp.dat", "w");
    if (!pf || !tempFile){
        displayMessage("Error opening file.", 1);
        if (pf) fclose(pf);
        if (tempFile) fclose(tempFile);
        stayOrReturn(0, removeAccount, u);
        return;
    }

    struct Record r;
    int found = 0; 

       // Read from the original file and write to the temporary file
    while (fscanf(pf, "%d %d %s %lld %d/%d/%d %s %d %lf %s",
                  &r.id, &r.userId, r.name, &r.accountNbr,
                  &r.deposit.month, &r.deposit.day, &r.deposit.year,
                  r.country, &r.phone, &r.amount, r.accountType) != EOF) {
        if (r.accountNbr == accountNbr && strcmp(r.name, u.name) == 0) {
            found = 1;
            continue; // Skip writing this record to the temporary file
        }
        // Write all other records to the temporary file
        fprintf(tempFile, "%d %d %s %lld %d/%d/%d %s %d %.2lf %s\n",
                r.id, r.userId, r.name, r.accountNbr,
                r.deposit.month, r.deposit.day, r.deposit.year,
                r.country, r.phone, r.amount, r.accountType);
    }

    fclose(pf);
    fclose(tempFile);

    // Remove the original file and rename the temporary file
    if (remove(RECORDS) != 0) {
        perror("Error deleting original file");
        return;
    }

    if (rename("temp.dat", RECORDS) != 0) {
        perror("Error renaming temporary file");
        return;
    }

    if (found) {
        displayMessage("Account removed successfully.", 0);
    }

    int option;
    while (1) {
        if (found) {
            displayMessage("Do you want to remove another account or return to the main menu?", 0);
        } else {
            displayMessage("Account not found. Would you like to try removing another account or return to the main menu?", 1);
        }

        printf("Enter 1 to remove another account or 2 to return to the main menu: ");
        
        // Validate input
        if (scanf("%d", &option) != 1) {
            while (getchar() != '\n'); 
            displayMessage("Invalid input. Please enter a number.", 1);
            continue;
        }

        switch (option) {
            case 1:
                removeAccount(u); 
                return;
            case 2:
                mainMenu(&u); 
                return;
            default:
                displayMessage("Invalid choice. Please enter 1 or 2.", 1);
                break;
        }
    }
}

void transferOwnership(struct User u){
    long long int accountNbr;
    printf("Enter the account number whose ownership is to be transferred: ");
    scanf("%lld", &accountNbr);
    getchar(); 

    FILE *pf = fopen(RECORDS, "r");
    FILE *tempFile = fopen("temp.dat", "w");
    if (!pf || !tempFile) {
        displayMessage("Error opening file.", 1);
        pauseExecution();
        if (pf) fclose(pf);
        if (tempFile) fclose(tempFile);
        stayOrReturn(0, transferOwnership, u);
        return;
    }

    // Check if the new owner exists and get their ID
    char newOwner[50];
    printf("Enter new owner's name: ");
    scanf("%s", newOwner);

    FILE *usersFile = fopen(USERS, "r");
    if (!usersFile) {
        displayMessage("Error opening users file.", 1);
        pauseExecution();
        fclose(pf);
        fclose(tempFile);
        return;
    }

    int newOwnerId = -1;
    struct User userChecker;
    while (fscanf(usersFile, "%d %s %s", &userChecker.id, userChecker.name, userChecker.password) != EOF) {
        if (strcmp(userChecker.name, newOwner) == 0) {
            newOwnerId = userChecker.id;
            break;
        }
    }
    fclose(usersFile);

    if (newOwnerId == -1) {
        displayMessage("New owner does not exist.", 1);
        pauseExecution();
        fclose(pf);
        fclose(tempFile);
        return;
    }

    struct Record r;
    int recordFound = 0;
    int ownerChanged = 0;

    // Read from the original file and write to the temporary file
    while (getAccountFromFile(pf, &r)) {
        if (r.accountNbr == accountNbr && strcmp(r.name, u.name) == 0) {
            recordFound = 1;
            // Update the record with the new owner's name and ID
            strcpy(r.name, newOwner);
            r.userId = newOwnerId;
            ownerChanged = 1;
        }
        // Write the (possibly updated) record to the temporary file
        fprintf(tempFile, "%d %d %s %lld %d/%d/%d %s %d %.2lf %s\n",
                r.id, r.userId, r.name, r.accountNbr,
                r.deposit.month, r.deposit.day, r.deposit.year,
                r.country, r.phone, r.amount, r.accountType);
    }

    fclose(pf);
    fclose(tempFile);

    // Remove the original file and rename the temporary file
    if (remove(RECORDS) != 0) {
        displayMessage("Error deleting original file", 1);
        pauseExecution();
        return;
    }

    if (rename("temp.dat", RECORDS) != 0) {
        displayMessage("Error renaming temporary file", 1);
        pauseExecution();
        return;
    }

    if (!recordFound) {
        displayMessage("Account not found or does not belong to you.", 1);
        pauseExecution();
        return;
    }

    // Call success or handle differently based on whether ownership was changed
    if (ownerChanged) {
        displayMessage("Ownership transferred successfully.", 0);
        pauseExecution();
        success(u, transferOwnership);
    } else {
        displayMessage("Ownership transfer wasn't completed.", 1);
        pauseExecution();
        mainMenu(&u);
    }   
}

void makeTransaction(struct User u){
    FILE *pf = fopen(RECORDS, "r");
    FILE *tempFile = fopen("temp.dat", "w");

    if (!pf || !tempFile) {
        displayMessage("Error opening files.", 1);
        pauseExecution();
        if (pf) fclose(pf);
        if (tempFile) fclose(tempFile);
        return;
    }

    long long int accountNbr;
    printf("Enter the account number for the transaction: ");
    scanf("%lld", &accountNbr);
    clearInputBuffer();

    struct Record r;
    int found = 0;

    while (getAccountFromFile(pf, &r)) {
        if (r.accountNbr == accountNbr && strcmp(r.name, u.name) == 0) {
            found = 1;

            //if the 5 first characters of the accountType are "fixed" no action possible
            if (strncmp(r.accountType, "fixed", 5) == 0) {
                displayMessage("Transactions are not allowed for fixed accounts.", 1);
                fclose(pf);
                fclose(tempFile);
                remove("temp.dat");
                pauseExecution();
                return;
            }

            int option;
            double amount;
            printf("1. Deposit\n2. Withdraw\nChoose an option: ");
            scanf("%d", &option);
            clearInputBuffer();
            printf("Enter the amount: ");
            scanf("%lf", &amount);
            clearInputBuffer();

            if (option == 1) {
                r.amount += amount;
                printf("Deposited $%.2lf. New balance: $%.2lf\n", amount, r.amount);
            } else if (option == 2) {
                if (r.amount < amount) {
                    displayMessage("Insufficient balance.", 1);
                    fclose(pf);
                    fclose(tempFile);
                    remove("temp.dat");
                    pauseExecution();
                    return;
                } else {
                    r.amount -= amount;
                    printf("Withdrew $%.2lf. New balance: $%.2lf\n", amount, r.amount);
                }
            } else {
                displayMessage("Invalid option.", 1);
                fclose(pf);
                fclose(tempFile);
                remove("temp.dat");
                pauseExecution();
                return;
            }
        }
        fprintf(tempFile, "%d %d %s %lld %d/%d/%d %s %d %.2lf %s\n",
                r.id,
                r.userId,
                r.name,
                r.accountNbr,
                r.deposit.month,
                r.deposit.day,
                r.deposit.year,
                r.country,
                r.phone,
                r.amount,
                r.accountType);
    }

    fclose(pf);
    fclose(tempFile);

    if (!found) {
        displayMessage("Account not found.", 1);
        remove("temp.dat");
        pauseExecution();
        return;
    }

    if (remove(RECORDS) != 0) {
        displayMessage("Error deleting original file", 1);
        pauseExecution();
        return;
    }

    if (rename("temp.dat", RECORDS) != 0) {
        displayMessage("Error renaming temporary file", 1);
        pauseExecution();
        return;
    }

    displayMessage("Transaction completed successfully.", 0);
    pauseExecution();
    success(u, makeTransaction);
}