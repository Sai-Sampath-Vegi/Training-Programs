// This is main module of Is Prime Module.

#include <stdio.h>
#include <stdbool.h>

#define TEST_FAILED "Failed test on %d\n"
#define PRIME_TEST_FAILED printf(TEST_FAILED, currentPrimeNumber);
#define NON_PRIME_TEST_FAILED printf(TEST_FAILED, currentNonPrimeNumber);

bool isPrime(int);

int main()
{
	int nonPrimeTestCases[] = {1, -20, 0, 80, 50, 51, 91, 104, 105, 249};
	int primeTestCases[] = {2, 3, 53, 83, 89, 97, 103, 61, 191};
	bool isAllTestsPassed = true;

	int totalNonPrimeTestCasesSize = sizeof(nonPrimeTestCases);
	int totalNonPrimeTestCaseCount = totalNonPrimeTestCasesSize / sizeof(nonPrimeTestCases[0]);

	int totalPrimeTestCasesSize = sizeof(primeTestCases);
	int totalPrimeTestCaseCount = totalPrimeTestCasesSize / sizeof(primeTestCases[0]);

	int nonPrimeCounter = 0, primeCounter = 0;
	while (nonPrimeCounter != totalNonPrimeTestCaseCount && primeCounter != totalPrimeTestCaseCount){

		int currentNonPrimeNumber, currentPrimeNumber;
		bool isCurrentNonPrimeTestFailed = false, isCurrentPrimeTestFailed = false;

		if (nonPrimeCounter < totalNonPrimeTestCaseCount){
			currentNonPrimeNumber = nonPrimeTestCases[nonPrimeCounter];
			nonPrimeCounter++;
			if (isPrime(currentNonPrimeNumber)){
				NON_PRIME_TEST_FAILED;
				isCurrentNonPrimeTestFailed = true;
			}
		}

		if (primeCounter < totalPrimeTestCaseCount){
			currentPrimeNumber = primeTestCases[primeCounter];
			primeCounter++;
			if (!isPrime(currentPrimeNumber)){
				PRIME_TEST_FAILED;
				isCurrentPrimeTestFailed = true;
			}
		}

		if ((isCurrentNonPrimeTestFailed || isCurrentPrimeTestFailed) && isAllTestsPassed){
			isAllTestsPassed = false;
		}
	}

	if (isAllTestsPassed){
		printf("Test passed.\n");
	}
	return 0;
}