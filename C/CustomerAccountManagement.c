// Manage customer account details.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define CUSTOMER_NAME_MAX_LENGTH 52
#define ACCOUNT_ID_MAX_LENGTH 16
#define ACCOUNT_ID_PREFIX_MAX_LENGTH 10

void openAnAccount();
void showAllAccounts(int);
char readMenuOption();
void updateAccountData(int);

struct customerAccountDetails
{
	char accountId[ACCOUNT_ID_MAX_LENGTH];
	char customerName[CUSTOMER_NAME_MAX_LENGTH];
	float currentBalance;
	char accountStatus;
};

typedef struct customerAccountDetails customerAccountDetails;

const char *ACCOUNTS_FILE_NAME = "CustomerAccountsDetails.dat";
const int CUSTOMER_ACCOUNT_SIZE = sizeof(customerAccountDetails);
const int ACTIVE_ACCOUNTS = 1029;
const int CLOSED_ACCOUNTS = 5019;
const int DEPOSIT_AMOUNT = 2941;
const int WITHDRAW_AMOUNT = 9420;
const int CHANGE_CUSTOMER_NAME = 8103;
const int DELETE_ACCOUNT = 7019;

char accountIdPrefix[ACCOUNT_ID_PREFIX_MAX_LENGTH];

int main()
{
	strcpy(accountIdPrefix, "");
	char option;
	do
	{
		option = readMenuOption();
		switch (option)
		{
		case '1':
			openAnAccount();
			break;
		case '2':
			showAllAccounts(ACTIVE_ACCOUNTS);
			break;
		case '3':
			showAllAccounts(CLOSED_ACCOUNTS);
			break;
		case '4':
			updateAccountData(DEPOSIT_AMOUNT);
			break;
		case '5':
			updateAccountData(WITHDRAW_AMOUNT);
			break;
		case '6':
			updateAccountData(CHANGE_CUSTOMER_NAME);
			break;
		case '7':
			updateAccountData(DELETE_ACCOUNT);
			break;
		case '8':
			printf("Exiting.\n");
		}
	} while (option != '8');
	return 0;
}

char readMenuOption()
{
	char option;
	printf("\nChoose an option from the following menu.\n");
	printf("1. Open an Account.\n");
	printf("2. Show all Active Accounts.\n");
	printf("3. Show all Closed Accounts.\n");
	printf("4. Deposit Amount.\n");
	printf("5. Withdraw Amount.\n");
	printf("6. Change Customer Name.\n");
	printf("7. Close an Account.\n");
	printf("8. Exit.\n");
	printf("Enter your option: ");
	scanf(" %c", &option);
	while (getchar() != '\n')
		;
	if (option > '8' || option < '1')
	{
		printf("Please enter a valid option.\n");
		return readMenuOption();
	}
	return option;
}

int findTotalAccountsCount()
{
	FILE *fpAccounts = fopen(ACCOUNTS_FILE_NAME, "r");
	fseek(fpAccounts, 0, SEEK_END);
	int accountsFileLenght = ftell(fpAccounts);
	int totalAccountsCount = accountsFileLenght / CUSTOMER_ACCOUNT_SIZE;
	rewind(fpAccounts);
	return totalAccountsCount;
}

int findAccountsCount(int mode)
{
	int accountsCounter = 0;
	FILE *fpAccounts = fopen(ACCOUNTS_FILE_NAME, "r");
	int totalAccountsCount = findTotalAccountsCount();
	customerAccountDetails account;
	for (int totalAccountsCounter = 0; totalAccountsCounter < totalAccountsCount; totalAccountsCounter++)
	{
		fread(&account, CUSTOMER_ACCOUNT_SIZE, 1, fpAccounts);
		if (account.accountStatus == ((mode == ACTIVE_ACCOUNTS) ? 'a' : 'c'))
		{
			accountsCounter++;
		}
	}
	return accountsCounter;
}

bool isAccountExists(char *accountId)
{
	FILE *fpAccounts = fopen(ACCOUNTS_FILE_NAME, "r");
	int totalAccountsCount = findTotalAccountsCount();
	customerAccountDetails account;
	bool isCustomerAccountExists = false;
	for (int totalAccountsCounter = 0; totalAccountsCounter < totalAccountsCount; totalAccountsCounter++)
	{
		fread(&account, CUSTOMER_ACCOUNT_SIZE, 1, fpAccounts);
		if (strcmp(account.accountId, accountId) == 0 && account.accountStatus == 'a')
		{
			isCustomerAccountExists = true;
			break;
		}
	}
	return isCustomerAccountExists;
}

int findAccountIndex(char *accountId)
{
	FILE *fpAccounts = fopen(ACCOUNTS_FILE_NAME, "r");
	int totalAccountsCount = findTotalAccountsCount();
	customerAccountDetails account;
	int totalAccountsCounter = -1;
	if (isAccountExists(accountId))
	{
		for (totalAccountsCounter = 0; totalAccountsCounter < totalAccountsCount; totalAccountsCounter++)
		{
			fread(&account, CUSTOMER_ACCOUNT_SIZE, 1, fpAccounts);
			if (strcmp(account.accountId, accountId) == 0)
			{
				break;
			}
		}
	}
	return totalAccountsCounter;
}

void printAccountDetails(customerAccountDetails account)
{
	printf("\nPrinting account details.\n");
	printf("Account Id is %s\n", account.accountId);
	printf("Customer name is %s\n", account.customerName);
	printf("Current balance is %f\n\n", account.currentBalance);
}

void generateRandomId(char *accountId, int idLength)
{
	char *characterSet = "AB6lMUtVCaQrSqsRT1cb8HhI0iJkjKLmNW7v4X3NodDEeF9fg5GPOpu2xYzZyw";
	srand(time(NULL));
	int characterSetSize = strlen(characterSet);
	int generatedCharsCounter = sprintf(accountId, "%s-", accountIdPrefix);
	while (generatedCharsCounter < idLength - 1)
	{
		int generatedIndex = characterSetSize - (rand() % characterSetSize);
		char generatedChar = characterSet[generatedIndex];
		bool isAlreadyGenerated = false;
		for (int counter = 0; counter < generatedCharsCounter + 1; counter++)
		{
			if (accountId[counter] == generatedChar)
			{
				isAlreadyGenerated = true;
				break;
			}
		}
		if (!isAlreadyGenerated)
		{
			accountId[generatedCharsCounter++] = generatedChar;
		}
	}
	accountId[idLength] = '\0';
}

void openAnAccount()
{
	int totalAccountsCount = findTotalAccountsCount();
	if (totalAccountsCount == 0)
	{
		strcpy(accountIdPrefix, "");
		printf("Enter a prefix to apply to all account: ");
		scanf(" %10s", accountIdPrefix);
	} else if (strcmp(accountIdPrefix, "") == 0){
		FILE* fpAccounts = fopen(ACCOUNTS_FILE_NAME, "r");
		rewind(fpAccounts);
		customerAccountDetails account;
		fread(&account, CUSTOMER_ACCOUNT_SIZE, 1, fpAccounts);
		int prefixCharactersCounter = 0;
		while (account.accountId[prefixCharactersCounter] != '-'){
			accountIdPrefix[prefixCharactersCounter] = account.accountId[prefixCharactersCounter];
			prefixCharactersCounter++;
		}
	}
	printf("\n\nYou are customer %d.\n\n", totalAccountsCount + 1);
	customerAccountDetails newAccount;
	char accountId[ACCOUNT_ID_MAX_LENGTH];
	bool isAccountAdded = false;
	do
	{
		generateRandomId(accountId, ACCOUNT_ID_MAX_LENGTH);
		printf("Your Account Id is %s\n", accountId);
		bool isCustomerAccountExists = isAccountExists(accountId);
		if (!isCustomerAccountExists)
		{
			strcpy(newAccount.accountId, accountId);
			printf("Enter your name: ");
			scanf(" %51[^\n]", newAccount.customerName);
			do
			{
				printf("Enter current balance of your account: ");
				scanf("%f", &newAccount.currentBalance);
				if (newAccount.currentBalance < 0)
				{
					printf("Enter a valid amount.\n");
				}
			} while (newAccount.currentBalance < 0);
			newAccount.accountStatus = 'a';
			FILE *fpAccounts = fopen(ACCOUNTS_FILE_NAME, "a");
			fwrite(&newAccount, CUSTOMER_ACCOUNT_SIZE, 1, fpAccounts);
			fclose(fpAccounts);
			isAccountAdded = true;
		}
		else
		{
			printf("Account with generated Id %s already existed. Generating new Id.\n", accountId);
		}
	} while (!isAccountAdded);
}

void showAllAccounts(int mode)
{
	FILE *fpAccounts = fopen(ACCOUNTS_FILE_NAME, "r");
	int totalAccountsCount = findTotalAccountsCount();
	int accountsToBeDisplayedCount = findAccountsCount(mode);
	customerAccountDetails account;
	char *status = (mode == ACTIVE_ACCOUNTS) ? "active" : "closed";
	if (accountsToBeDisplayedCount != 0)
	{
		printf("\n\nPrinting %d %s account details.\n\n", accountsToBeDisplayedCount, status);
		int showntotalAccountsCounter = 0;
		for (int totalAccountsCounter = 0; totalAccountsCounter < totalAccountsCount; totalAccountsCounter++)
		{
			fread(&account, CUSTOMER_ACCOUNT_SIZE, 1, fpAccounts);
			if (account.accountStatus == ((mode == ACTIVE_ACCOUNTS) ? 'a' : 'c'))
			{
				printf("\nAccount %d details.\n", showntotalAccountsCounter + 1);
				printAccountDetails(account);
				showntotalAccountsCounter++;
			}
		}
		printf("\n\nEnd of accounts.\n\n");
	}
	else
	{
		printf("\n\nNo %s accounts found.\n\n", status);
	}
}

void updateAccountData(int mode)
{
	FILE *fpAccounts = fopen(ACCOUNTS_FILE_NAME, "r+");
	char accountId[ACCOUNT_ID_MAX_LENGTH];
	int totalAccountsCanBeModified = findAccountsCount(ACTIVE_ACCOUNTS);
	if (totalAccountsCanBeModified != 0)
	{
		customerAccountDetails account;
		char *status;
		if (mode == DEPOSIT_AMOUNT)
		{
			status = "deposit amount";
		}
		else if (mode == WITHDRAW_AMOUNT)
		{
			status = "withdraw amount";
		}
		else if (mode == CHANGE_CUSTOMER_NAME)
		{
			status = "change name";
		}
		else if (mode == DELETE_ACCOUNT)
		{
			status = "delete account";
		}
		printf("Enter your account Id to %s: ", status);
		scanf(" %15[^\n]", accountId);
		bool isCustomerAccountExists = isAccountExists(accountId);
		if (isCustomerAccountExists)
		{
			int accountIndex = findAccountIndex(accountId);
			fseek(fpAccounts, accountIndex * CUSTOMER_ACCOUNT_SIZE, SEEK_SET);
			fread(&account, CUSTOMER_ACCOUNT_SIZE, 1, fpAccounts);
			printAccountDetails(account);
			bool isDataModified = false;
			if (mode == DEPOSIT_AMOUNT)
			{
				float amountToDeposit;
				do
				{
					printf("Enter amount to deposit: ");
					scanf("%f", &amountToDeposit);
					if (amountToDeposit >= 0)
					{
						account.currentBalance += amountToDeposit;
						isDataModified = true;
						printf("Amount deposited successfully.\n");
					}
					else
					{
						printf("Please enter valid amount to deposit.\n");
					}
				} while (amountToDeposit < 0);
			}
			else if (mode == WITHDRAW_AMOUNT)
			{
				if (account.currentBalance != 0)
				{
					float amountToWithdraw;
					printf("Enter amount to withdraw: ");
					scanf("%f", &amountToWithdraw);
					if (amountToWithdraw <= account.currentBalance)
					{
						account.currentBalance -= amountToWithdraw;
						isDataModified = true;
						printf("Amount withdrawn successfully.\n");
					}
					else
					{
						printf("Insufficient balance. Please try another amount.\n");
					}
				}
				else
				{
					printf("Insufficient balance to withdraw.\n");
				}
			}
			else if (mode == CHANGE_CUSTOMER_NAME)
			{
				char newCustomerName[CUSTOMER_NAME_MAX_LENGTH];
				printf("Enter new name to change from your old name %s: ", account.customerName);
				scanf(" %51[^\n]", newCustomerName);
				strcpy(account.customerName, newCustomerName);
				isDataModified = true;
				printf("Name modified successfully.\n");
			}
			else if (mode == DELETE_ACCOUNT)
			{
				char confirmation;
				printf("Do you want to confirm deleting account. Enter yes to confirm deleting: ");
				scanf(" %c", &confirmation);
				while (getchar() != '\n')
					;
				if (confirmation == 'y' || confirmation == 'Y')
				{
					account.accountStatus = 'c';
					isDataModified = true;
					printf("Account deleted successfully.\n");
				}
				else
				{
					printf("Account deletion cancelled.\n");
				}
			}
			if (isDataModified)
			{
				fseek(fpAccounts, -CUSTOMER_ACCOUNT_SIZE, SEEK_CUR);
				fwrite(&account, CUSTOMER_ACCOUNT_SIZE, 1, fpAccounts);
				fclose(fpAccounts);
			}
		}
		else
		{
			printf("Account with %s as Id not found. Please try with another Id.\n", accountId);
		}
	}
	else
	{
		printf("\n\nNo customer accounts found to make changes.\n\n");
	}
}
