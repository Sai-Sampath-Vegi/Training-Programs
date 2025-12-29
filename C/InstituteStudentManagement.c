// Manages Student details in institute.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#define STUDENT_ID_LENGTH 16
#define STUDENT_NAME_MAX_LENGTH 52
#define STUDENT_ID_PREFIX_MAX_LENGTH 10

struct studentDetails
{
	char studentId[STUDENT_ID_LENGTH];
	char studentName[STUDENT_NAME_MAX_LENGTH];
	int studentAge;
	bool isStudentActive;
};

typedef struct studentDetails studentDetails;

void addNewStudent();
void showAllStudents(int);
char readMenuOption();
void updateStudentData(int);

const char *STUDENTS_FILE_NAME = "InstituteStudentsDetails.dat";
const char *STUDENT_ID_PREFIX = "STU-";
const int STUDENT_SIZE = sizeof(studentDetails);
const int ACTIVE_STUDENTS = 2043;
const int INACTIVE_STUDENTS = 3029;
const int CHANGE_STUDENT_AGE = 7015;
const int CHANGE_STUDENT_NAME = 8204;
const int REMOVE_STUDENT = 5016;

char studentIdPrefix[STUDENT_ID_PREFIX_MAX_LENGTH];

int main()
{
	strcpy(studentIdPrefix, "");
	char option;
	do
	{
		option = readMenuOption();
		switch (option)
		{
		case '1':
			addNewStudent();
			break;
		case '2':
			showAllStudents(ACTIVE_STUDENTS);
			break;
		case '3':
			showAllStudents(INACTIVE_STUDENTS);
			break;
		case '4':
			updateStudentData(CHANGE_STUDENT_AGE);
			break;
		case '5':
			updateStudentData(CHANGE_STUDENT_NAME);
			break;
		case '6':
			updateStudentData(REMOVE_STUDENT);
			break;
		case '7':
			printf("Exiting.\n");
		}
	} while (option != '7');
	return 0;
}

char readMenuOption()
{
	char option;
	printf("\nChoose an option from the following menu.\n");
	printf("1. Add a Student.\n");
	printf("2. Show all Active Students.\n");
	printf("3. Show all Inactive Students.\n");
	printf("4. Change Student Age.\n");
	printf("5. Change Student Name.\n");
	printf("6. Remove a Student.\n");
	printf("7. Exit.\n");
	printf("Enter your option: ");
	scanf(" %c", &option);
	while (getchar() != '\n')
		;
	if (option > '7' || option < '1')
	{
		printf("Please enter a valid option.\n");
		return readMenuOption();
	}
	return option;
}

int findTotalStudentsCount()
{
	FILE *fpStudents = fopen(STUDENTS_FILE_NAME, "r");
	fseek(fpStudents, 0, SEEK_END);
	int studentsFileLenght = ftell(fpStudents);
	int totalStudentsCount = studentsFileLenght / STUDENT_SIZE;
	rewind(fpStudents);
	return totalStudentsCount;
}

int findStudentsCount(int mode)
{
	int studentsCounter = 0;
	FILE *fpStudents = fopen(STUDENTS_FILE_NAME, "r");
	int totalStudentsCount = findTotalStudentsCount();
	studentDetails student;
	for (int totalStudentsCounter = 0; totalStudentsCounter < totalStudentsCount; totalStudentsCounter++)
	{
		fread(&student, STUDENT_SIZE, 1, fpStudents);
		if (student.isStudentActive == ((mode == ACTIVE_STUDENTS) ? true : false))
		{
			studentsCounter++;
		}
	}
	return studentsCounter;
}

bool isStudentExists(char *studentId)
{
	FILE *fpStudents = fopen(STUDENTS_FILE_NAME, "r");
	int totalStudentsCount = findTotalStudentsCount();
	studentDetails student;
	bool isStudentDetailsExists = false;
	for (int totalStudentsCounter = 0; totalStudentsCounter < totalStudentsCount; totalStudentsCounter++)
	{
		fread(&student, STUDENT_SIZE, 1, fpStudents);
		if (strcmp(student.studentId, studentId) == 0 && student.isStudentActive == true)
		{
			isStudentDetailsExists = true;
			break;
		}
	}
	return isStudentDetailsExists;
}

int findstudentIndex(char *studentId)
{
	FILE *fpStudents = fopen(STUDENTS_FILE_NAME, "r");
	int totalStudentsCount = findTotalStudentsCount();
	studentDetails student;
	int totalStudentsCounter = -1;
	if (isStudentExists(studentId))
	{
		for (totalStudentsCounter = 0; totalStudentsCounter < totalStudentsCount; totalStudentsCounter++)
		{
			fread(&student, STUDENT_SIZE, 1, fpStudents);
			if (strcmp(student.studentId, studentId) == 0)
			{
				break;
			}
		}
	}
	return totalStudentsCounter;
}

void printStudentDetails(studentDetails student)
{
	printf("\nPrinting student details.\n");
	printf("student Id is %s\n", student.studentId);
	printf("Student name is %s\n", student.studentName);
	printf("Student age is %d\n\n", student.studentAge);
}

void generateRandomId(char *studentId, int idLength)
{
	char *characterSet = "AB6lMUtVCaQrSqsRT1cb8HhI0iJkjKLmNW7v4X3NodDEeF9fg5GPOpu2xYzZyw";
	srand(time(NULL));
	int characterSetSize = strlen(characterSet);
	int generatedCharsCounter = sprintf(studentId, "%s-", studentIdPrefix);
	while (generatedCharsCounter < idLength - 1)
	{
		int generatedIndex = characterSetSize - (rand() % characterSetSize);
		char generatedChar = characterSet[generatedIndex];
		bool isAlreadyGenerated = false;
		for (int counter = 0; counter < generatedCharsCounter + 1; counter++)
		{
			if (studentId[counter] == generatedChar)
			{
				isAlreadyGenerated = true;
				break;
			}
		}
		if (!isAlreadyGenerated)
		{
			studentId[generatedCharsCounter++] = generatedChar;
		}
	}
	studentId[idLength] = '\0';
}

void addNewStudent()
{
	int totalStudentsCount = findTotalStudentsCount();
	if (totalStudentsCount == 0)
	{
		strcpy(studentIdPrefix, "");
		printf("Enter a prefix to apply to all items: ");
		scanf(" %10s", studentIdPrefix);
	} else if (strcmp(studentIdPrefix, "") == 0){
		FILE* fpStudents = fopen(STUDENTS_FILE_NAME, "r");
		rewind(fpStudents);
		studentDetails student;
		fread(&student, STUDENT_SIZE, 1, fpStudents);
		int prefixCharactersCounter = 0;
		while (student.studentId[prefixCharactersCounter] != '-'){
			studentIdPrefix[prefixCharactersCounter] = student.studentId[prefixCharactersCounter];
			prefixCharactersCounter++;
		}
	}
	printf("You are adding student %d details\n", totalStudentsCount + 1);
	studentDetails newStudent;
	char studentId[STUDENT_ID_LENGTH];
	bool isStudentAdded = false;
	do
	{
		generateRandomId(studentId, STUDENT_ID_LENGTH);
		printf("New student Id is %s\n", studentId);
		bool isStudentIdExists = isStudentExists(studentId);
		if (!isStudentIdExists)
		{
			strcpy(newStudent.studentId, studentId);
			printf("Enter student name: ");
			scanf(" %51[^\n]", newStudent.studentName);
			do
			{
				printf("Enter student age: ");
				scanf("%d", &newStudent.studentAge);
				if (newStudent.studentAge < 0 || newStudent.studentAge > 100)
				{
					printf("Enter a valid age.\n");
				}
			} while (newStudent.studentAge < 0 || newStudent.studentAge > 100);
			newStudent.isStudentActive = true;
			FILE *fpStudents = fopen(STUDENTS_FILE_NAME, "a");
			fwrite(&newStudent, STUDENT_SIZE, 1, fpStudents);
			fclose(fpStudents);
			isStudentAdded = true;
		}
		else
		{
			printf("Student with generated Id %s already existed. Generating new Id.\n", studentId);
		}
	} while (!isStudentAdded);
}

void showAllStudents(int mode)
{
	FILE *fpStudents = fopen(STUDENTS_FILE_NAME, "r");
	int totalStudentsCount = findTotalStudentsCount();
	int studentsToBeDisplayed = findStudentsCount(mode);
	studentDetails student;
	char *status;
	if (mode == ACTIVE_STUDENTS)
	{
		status = "active";
	}
	else if (mode == INACTIVE_STUDENTS)
	{
		status = "inactive";
	}
	if (studentsToBeDisplayed != 0)
	{
		int studentsToBeDisplayedCounter = 0;
		printf("\n\nPrinting %d %s students details.\n\n", studentsToBeDisplayed, status);
		for (int studentsCounter = 0; studentsCounter < totalStudentsCount; studentsCounter++)
		{
			fread(&student, STUDENT_SIZE, 1, fpStudents);
			bool isEligibleToDisplay = false;
			if (((mode == ACTIVE_STUDENTS) && (student.isStudentActive)) || ((mode == INACTIVE_STUDENTS) && (!student.isStudentActive)))
			{
				isEligibleToDisplay = true;
			}
			if (isEligibleToDisplay)
			{
				printf("\nPrinting students %d details.\n\n", studentsToBeDisplayedCounter + 1);
				printStudentDetails(student);
				studentsToBeDisplayedCounter++;
			}
		}
		printf("\n\nEnd of students.\n\n");
	}
	else
	{
		printf("\n\nNo %s students found.\n\n", status);
	}
}

void updateStudentData(int mode)
{
	FILE *fpStudents = fopen(STUDENTS_FILE_NAME, "r+");
	char studentId[STUDENT_ID_LENGTH];
	int totalStudentsCountCanBeModified = findStudentsCount(ACTIVE_STUDENTS);
	if (totalStudentsCountCanBeModified != 0)
	{
		studentDetails student;
		char *status;
		if (mode == CHANGE_STUDENT_AGE)
		{
			status = "change student age";
		}
		else if (mode == CHANGE_STUDENT_NAME)
		{
			status = "change student name";
		}
		else if (mode == REMOVE_STUDENT)
		{
			status = "remove student";
		}
		printf("Enter your student Id to %s: ", status);
		scanf(" %15[^\n]", studentId);
		bool isStudentDetailsExists = isStudentExists(studentId);
		if (isStudentDetailsExists)
		{
			int studentIndex = findstudentIndex(studentId);
			fseek(fpStudents, studentIndex * STUDENT_SIZE, SEEK_SET);
			fread(&student, STUDENT_SIZE, 1, fpStudents);
			printStudentDetails(student);
			bool isDataModified = false;
			if (mode == CHANGE_STUDENT_AGE)
			{
				do
				{
					printf("Enter new age of %s to change age: ", student.studentName);
					scanf("%d", &student.studentAge);
					if (student.studentAge >= 0 && student.studentAge <= 100)
					{
						isDataModified = true;
						printf("Age of %s changed successfully.\n", student.studentName);
					}
					else
					{
						printf("Please enter valid age to change student age.\n");
					}
				} while (student.studentAge < 0 || student.studentAge > 100);
			}
			else if (mode == CHANGE_STUDENT_NAME)
			{
				char newStudentName[STUDENT_NAME_MAX_LENGTH];
				printf("Enter new name to change from student old name %s: ", student.studentName);
				scanf(" %51[^\n]", newStudentName);
				strcpy(student.studentName, newStudentName);
				isDataModified = true;
				printf("Name modified successfully.\n");
			}
			else if (mode == REMOVE_STUDENT)
			{
				char confirmation;
				printf("Do you want to confirm deleting student. Enter yes to confirm deleting: ");
				scanf(" %c", &confirmation);
				while (getchar() != '\n')
					;
				if (confirmation == 'y' || confirmation == 'Y')
				{
					student.isStudentActive = false;
					isDataModified = true;
					printf("student deleted successfully.\n");
				}
				else
				{
					printf("student deletion cancelled.\n");
				}
			}
			if (isDataModified)
			{
				fseek(fpStudents, -STUDENT_SIZE, SEEK_CUR);
				fwrite(&student, STUDENT_SIZE, 1, fpStudents);
				fclose(fpStudents);
			}
		}
		else
		{
			printf("Student with %s as Id not found. Please try with another Id.\n", studentId);
		}
	}
	else
	{
		printf("\n\nNo students found to make changes.\n\n");
	}
}
