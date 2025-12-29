// Manage Item details.

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>

#define ITEM_ID_LENGTH 16
#define ITEM_NAME_MAX_LENGTH 52
#define ITEM_ID_PREFIX_MAX_LENGTH 10

struct storeItemDetails
{
	char itemId[ITEM_ID_LENGTH];
	char itemName[ITEM_NAME_MAX_LENGTH];
	float itemPrice;
	float itemDiscount;
	bool isItemAvailable;
};

typedef struct storeItemDetails storeItemDetails;

void addAnItem();
void showAllItems(int);
char readMenuOption();
void updateItemData(int);

const char* ITEMS_FILE_NAME = "StoreItemsDetails.dat";
const int ITEM_SIZE = sizeof(storeItemDetails);
const int AVAILABLE_ITEMS = 8901;
const int ITEMS_UNDER_DISCOUNT = 8100;
const int UNAVAILABLE_ITEMS = 9301;
const int INCREASE_ITEM_PRICE = 9230;
const int DECREASE_ITEM_PRICE = 7103;
const int CHANGE_DISCOUNT = 6019;
const int CHANGE_ITEM_NAME = 8409;
const int REMOVE_ITEM = 4002;

char itemIdPrefix[ITEM_ID_PREFIX_MAX_LENGTH];

int main()
{
	strcpy(itemIdPrefix, "");
	char option;
	do
	{
		option = readMenuOption();
		switch (option)
		{
		case '1':
			addAnItem();
			break;
		case '2':
			showAllItems(AVAILABLE_ITEMS);
			break;
		case '3':
			showAllItems(UNAVAILABLE_ITEMS);
			break;
		case '4':
			showAllItems(ITEMS_UNDER_DISCOUNT);
			break;
		case '5':
			updateItemData(INCREASE_ITEM_PRICE);
			break;
		case '6':
			updateItemData(DECREASE_ITEM_PRICE);
			break;
		case '7':
			updateItemData(CHANGE_DISCOUNT);
			break;
		case '8':
			updateItemData(CHANGE_ITEM_NAME);
			break;
		case '9':
			updateItemData(REMOVE_ITEM);
			break;
		case '0':
			printf("Exiting.\n");
		}
	} while (option != '0');
	return 0;
}

char readMenuOption()
{
	char option;
	printf("\nChoose an option from the following menu.\n");
	printf("1. Add an Item to Store.\n");
	printf("2. Show all Available Items in Store.\n");
	printf("3. Show all Unavailable Items in Store.\n");
	printf("4. Show all Items on Discount in Store.\n");
	printf("5. Increase Item Price.\n");
	printf("6. Decrease Item Price.\n");
	printf("7. Change Discount of an Item.\n");
	printf("8. Change Item Name in Store.\n");
	printf("9. Remove an Item from Store.\n");
	printf("0. Exit.\n");
	printf("Enter your option: ");
	scanf(" %c", &option);
	while (getchar() != '\n');
	if (option > '9' || option < '0')
	{
		printf("Please enter a valid option.\n");
		return readMenuOption();
	}
	return option;
}

int findTotalItemsCount()
{
	FILE *fpItems = fopen(ITEMS_FILE_NAME, "r");
	fseek(fpItems, 0, SEEK_END);
	int itemsFileLenght = ftell(fpItems);
	int totalItemCount = itemsFileLenght / ITEM_SIZE;
	rewind(fpItems);
	return totalItemCount;
}

int findItemsCount(int mode)
{
	int itemsCounter = 0;
	FILE *fpItems = fopen(ITEMS_FILE_NAME, "r");
	int totalItemCount = findTotalItemsCount();
	storeItemDetails item;
	for (int totalItemsCounter = 0; totalItemsCounter < totalItemCount; totalItemsCounter++)
	{
		fread(&item, ITEM_SIZE, 1, fpItems);
		if ((item.isItemAvailable == ((mode == AVAILABLE_ITEMS) ? true : false)) || ((item.isItemAvailable == true) && (item.itemDiscount != 0)))
		{
			itemsCounter++;
		}
	}
	return itemsCounter;
}

bool isItemExists(char *itemId)
{
	FILE *fpItems = fopen(ITEMS_FILE_NAME, "r");
	int totalItemCount = findTotalItemsCount();
	storeItemDetails item;
	bool isCustomerItemExists = false;
	for (int totalItemsCounter = 0; totalItemsCounter < totalItemCount; totalItemsCounter++)
	{
		fread(&item, ITEM_SIZE, 1, fpItems);
		if (strcmp(item.itemId, itemId) == 0 && item.isItemAvailable)
		{
			isCustomerItemExists = true;
			break;
		}
	}
	return isCustomerItemExists;
}

int findItemIndex(char *itemId)
{
	FILE *fpItems = fopen(ITEMS_FILE_NAME, "r");
	int totalItemCount = findTotalItemsCount();
	storeItemDetails item;
	int totalItemsCounter = -1;
	if (isItemExists(itemId))
	{
		for (totalItemsCounter = 0; totalItemsCounter < totalItemCount; totalItemsCounter++)
		{
			fread(&item, ITEM_SIZE, 1, fpItems);
			if (strcmp(item.itemId, itemId) == 0)
			{
				break;
			}
		}
	}
	return totalItemsCounter;
}

void printItemDetails(storeItemDetails item)
{
	printf("\nPrinting item details.\n\n");
	printf("Item Id is %s\n", item.itemId);
	printf("Item name is %s\n", item.itemName);
	printf("Item original price is %f\n", item.itemPrice);
	if (item.itemDiscount != 0)
	{
		printf("Item discount is %f%%\n", item.itemDiscount);
		float itemDiscount = (item.itemPrice / 100) * item.itemDiscount;
		printf("Item discount is -%f\n", itemDiscount);
		float finalPrice = item.itemPrice - itemDiscount;
		printf("Item price after applying discount is %f\n", finalPrice);
	}
	printf("\n");
}

void generateRandomId(char *itemId, int idLength)
{
	char *characterSet = "AB6lMUtVCaQrSqsRT1cb8HhI0iJkjKLmNW7v4X3NodDEeF9fg5GPOpu2xYzZyw";
	srand(time(NULL));
	int characterSetSize = strlen(characterSet);
	int generatedCharsCounter = sprintf(itemId, "%s-", itemIdPrefix);
	while (generatedCharsCounter < idLength - 1)
	{
		int generatedIndex = characterSetSize - (rand() % characterSetSize);
		char generatedChar = characterSet[generatedIndex];
		bool isAlreadyGenerated = false;
		for (int counter = 0; counter < generatedCharsCounter + 1; counter++)
		{
			if (itemId[counter] == generatedChar)
			{
				isAlreadyGenerated = true;
				break;
			}
		}
		if (!isAlreadyGenerated)
		{
			itemId[generatedCharsCounter++] = generatedChar;
		}
	}
	itemId[idLength] = '\0';
}

void addAnItem()
{
	int totalItemCount = findTotalItemsCount();
	if (totalItemCount == 0)
	{
		strcpy(itemIdPrefix, "");
		printf("Enter a prefix to apply to all items: ");
		scanf(" %10s", itemIdPrefix);
	} else if (strcmp(itemIdPrefix, "") == 0){
		FILE* fpItems = fopen(ITEMS_FILE_NAME, "r");
		rewind(fpItems);
		storeItemDetails item;
		fread(&item, ITEM_SIZE, 1, fpItems);
		int prefixCharactersCounter = 0;
		while (item.itemId[prefixCharactersCounter] != '-'){
			itemIdPrefix[prefixCharactersCounter] = item.itemId[prefixCharactersCounter];
			prefixCharactersCounter++;
		}
	}
	printf("\nYou are adding item %d details\n", totalItemCount + 1);
	storeItemDetails newItem;
	char itemId[ITEM_ID_LENGTH];
	bool isItemAdded = false;
	do
	{
		generateRandomId(itemId, ITEM_ID_LENGTH);
		printf("\nNew Item Id is %s\n\n", itemId);
		bool isItemAlreadyExists = isItemExists(itemId);
		if (!isItemAlreadyExists)
		{
			strcpy(newItem.itemId, itemId);
			printf("Enter item name: ");
			scanf(" %51[^\n]", newItem.itemName);
			do
			{
				printf("Enter item price: ");
				scanf("%f", &newItem.itemPrice);
				if (newItem.itemPrice < 0)
				{
					printf("Enter a valid price.\n");
				}
			} while (newItem.itemPrice < 0);
			char confirmation;
			printf("Is this item has discount. Enter yes if has discount: ");
			scanf(" %c", &confirmation);
			if (confirmation == 'y' || confirmation == 'Y')
			{
				do
				{
					printf("Enter discount on item %s(if any): ", newItem.itemName);
					scanf("%f", &newItem.itemDiscount);
					if (newItem.itemDiscount < 0 || newItem.itemDiscount > 100)
					{
						printf("Enter valid discount.\n");
					}
				} while (newItem.itemDiscount < 0);
			}
			else
			{
				newItem.itemDiscount = 0;
			}
			newItem.isItemAvailable = true;
			FILE *fpItems = fopen(ITEMS_FILE_NAME, "a");
			fwrite(&newItem, ITEM_SIZE, 1, fpItems);
			fclose(fpItems);
			isItemAdded = true;
			printf("Item added successfully.\n");
		}
		else
		{
			printf("Item with generated Id %s is already Exiting. Generating new Id.\n");
		}
	} while (!isItemAdded);
}

void showAllItems(int mode)
{
	FILE *fpItems = fopen(ITEMS_FILE_NAME, "r");
	int totalItemCount = findTotalItemsCount();
	int itemsToBeDisplayedCount = findItemsCount(mode);
	storeItemDetails item;
	char *status;
	if (mode == AVAILABLE_ITEMS)
	{
		status = "available";
	}
	else if (mode == UNAVAILABLE_ITEMS)
	{
		status = "unavailable";
	}
	else if (mode == ITEMS_UNDER_DISCOUNT)
	{
		status = "under discount";
	}
	if (itemsToBeDisplayedCount != 0)
	{
		int itemsToBeDisplayedCounter = 0;
		printf("\n\nPrinting %d %s items details.\n\n", itemsToBeDisplayedCount, status);
		for (int itemCounter = 0; itemCounter < totalItemCount; itemCounter++)
		{
			fread(&item, ITEM_SIZE, 1, fpItems);
			bool isEligibleToDisplay = false;
			if (((mode == AVAILABLE_ITEMS) && (item.isItemAvailable)) || ((mode == UNAVAILABLE_ITEMS) && (!item.isItemAvailable)) || ((mode == ITEMS_UNDER_DISCOUNT) && (item.itemDiscount != 0)))
			{
				isEligibleToDisplay = true;
			}
			if (isEligibleToDisplay)
			{
				printf("\nPrinting item %d details.\n\n", itemsToBeDisplayedCounter + 1);
				printItemDetails(item);
				itemsToBeDisplayedCounter++;
			}
		}
		printf("\n\nEnd of items.\n\n");
	}
	else
	{
		printf("\n\nNo %s items found.\n\n", status);
	}
}

void updateItemData(int mode)
{
	FILE *fpItems = fopen(ITEMS_FILE_NAME, "r+");
	char itemId[ITEM_ID_LENGTH];
	int totalItemCountCanBeModified = findItemsCount(AVAILABLE_ITEMS);
	if (totalItemCountCanBeModified != 0)
	{
		storeItemDetails item;
		char* status;
		if (mode == INCREASE_ITEM_PRICE)
		{
			status = "increase item price";
		}
		else if (mode == DECREASE_ITEM_PRICE)
		{
			status = "decrease item price";
		}
		else if (mode == CHANGE_DISCOUNT)
		{
			status = "apply/remove discount";
		}
		else if (mode == CHANGE_ITEM_NAME)
		{
			status = "change item name";
		}
		else if (mode == REMOVE_ITEM)
		{
			status = "remove item";
		}
		printf("Enter your item Id to %s: ", status);
		scanf(" %15[^\n]", itemId);
		bool isItemExistsToModify = isItemExists(itemId);
		if (isItemExistsToModify)
		{
			int itemIndex = findItemIndex(itemId);
			fseek(fpItems, itemIndex * ITEM_SIZE, SEEK_SET);
			fread(&item, ITEM_SIZE, 1, fpItems);
			printItemDetails(item);
			bool isDataModified = false;
			if (mode == INCREASE_ITEM_PRICE)
			{
				float priceToIncrease;
				do
				{
					printf("Enter price to increase: ");
					scanf("%f", &priceToIncrease);
					if (priceToIncrease >= 0)
					{
						item.itemPrice += priceToIncrease;
						isDataModified = true;
						printf("Price increased successfully.\n");
					}
					else
					{
						printf("Please enter valid price to increase.\n");
					}
				} while (priceToIncrease < 0);
			}
			else if (mode == DECREASE_ITEM_PRICE)
			{
				float priceToDecrease;
				bool isPriceDecreased = false;
				do
				{
					printf("Enter price to decrease: ");
					scanf("%f", &priceToDecrease);
					if (priceToDecrease < item.itemPrice)
					{
						item.itemPrice -= priceToDecrease;
						isDataModified = true;
						printf("Price decreased successfully.\n");
						isPriceDecreased = true;
					}
					else
					{
						printf("Enter valid price to decrease.\n");
					}
				} while (!isPriceDecreased);
			}
			else if (mode == CHANGE_DISCOUNT)
			{
				float discount;
				bool isDiscountApplied = false;
				do
				{
					printf("Enter 0 to remove discount, positive number to increase discount, negative number to decrease discount.\n");
					printf("Enter discount change to be applied: ");
					scanf("%f", &discount);
					if (discount == 0)
					{
						item.itemDiscount = 0;
						isDiscountApplied = true;
					}
					else if (((discount > 0) && ((item.itemDiscount + discount) < 100)) || (((discount < 0) && ((item.itemDiscount - discount) >= 0) && (item.itemDiscount >= discount))))
					{
						item.itemDiscount += discount;
						isDiscountApplied = true;
					}
					if (isDiscountApplied)
					{
						isDataModified = true;
					}
					else
					{
						printf("Enter valid discount to apply.\n");
					}
				} while (!isDiscountApplied);
			}
			else if (mode == CHANGE_ITEM_NAME)
			{
				char newItemName[ITEM_NAME_MAX_LENGTH];
				printf("Enter new name to change from item's old name %s: ", item.itemName);
				scanf(" %51[^\n]", newItemName);
				strcpy(item.itemName, newItemName);
				isDataModified = true;
				printf("Item name modified successfully.\n");
			}
			else if (mode == REMOVE_ITEM)
			{
				char confirmation;
				printf("Do you want to confirm deleting item. Enter yes to confirm deleting: ");
				scanf(" %c", &confirmation);
				while (getchar() != '\n')		;
				if (confirmation == 'y' || confirmation == 'Y')
				{
					item.isItemAvailable = false;
					isDataModified = true;
					printf("Item deleted successfully.\n");
				}
				else
				{
					printf("Item deletion cancelled.\n");
				}
			}
			if (isDataModified)
			{
				fseek(fpItems, -ITEM_SIZE, SEEK_CUR);
				fwrite(&item, ITEM_SIZE, 1, fpItems);
				fclose(fpItems);
			}
		}
		else
		{
			printf("Item with %s as Id not found. Please try again with another Id.\n", itemId);
		}
	}
	else
	{
		printf("\nNo items found to make changes.\n");
	}
}
