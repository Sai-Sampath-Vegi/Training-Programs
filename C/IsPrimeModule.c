// This is a module of is prime program.

#include <math.h>
#include <stdbool.h>

bool isPrime(int number)
{
	bool isPrime = true;
 
    int divisor;
    if (number % 2 != 0){
	    int sqrtOfNumber = sqrt(number);
	    for (divisor = 3; divisor <= sqrtOfNumber; divisor += 2)
	    {
	        if (number % divisor == 0){
	            break;
	        }
	    }
	}

    if (number != 2 && (number < 2 || number % 2 == 0 || (number % divisor == 0 && number != 3))){
       isPrime = false;
    }
    return isPrime;

}
