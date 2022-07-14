#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//---------------------------------------------------------------------------------------------
static int ARRAY_LENGTH = -1;
//---------------------------------------------------------------------------------------------
void fillNumbersArrayWithRandomNumbers(int *numbersArray) {
	srand(time(NULL));
	for( int i = 0 ; i < ARRAY_LENGTH ; i++) {
		*(numbersArray + i) = rand()%100;
	}
}
//---------------------------------------------------------------------------------------------
void swapElements(int *numbersArray, const int positionOfFirstElem, const int positionOfSecondElem) {
	const int firstElem = *(numbersArray+positionOfFirstElem);
	*(numbersArray+positionOfFirstElem) = *(numbersArray+positionOfSecondElem);
	*(numbersArray+positionOfSecondElem) = firstElem;
}
//---------------------------------------------------------------------------------------------
void bubbleSortDo(int *numbersArray, const int n, const int i, const int nmbOfCorrectlyFilledPositionsFromEnd) {
	// n-1 because if n-1 elements are correct, the last one is also correct
	if (nmbOfCorrectlyFilledPositionsFromEnd+1 == n-1) {
		return;
	}
	// n-1 because we are checking the i+1 element, so this prevents touching undefined places
	if (i == n-1-nmbOfCorrectlyFilledPositionsFromEnd) {
		//return bubbleSortDo(numbersArray, n, 0, nmbOfCorrectlyFilledPositionsFromEnd+1);
		bubbleSortDo(numbersArray, n, 0, nmbOfCorrectlyFilledPositionsFromEnd+1);
		return;
	}
	//if the next element is smalller - swap
	if (numbersArray[i] > numbersArray[i+1]) {
		swapElements(numbersArray, i, i+1);
	}
	///return bubbleSortDo(numbersArray, n, i+1, nmbOfCorrectlyFilledPositionsFromEnd);
	bubbleSortDo(numbersArray, n, i+1, nmbOfCorrectlyFilledPositionsFromEnd);
	return;
}
//---------------------------------------------------------------------------------------------
void bubbleSort(int *numbersArray, int n) {
	bubbleSortDo(numbersArray, n, 0, 0);
}
//---------------------------------------------------------------------------------------------
void printArray(const int *numbersArray) {
	for (int i = 0 ; i < ARRAY_LENGTH ; i++) {
		printf("%d", numbersArray[i]);
		if (i < ARRAY_LENGTH - 1) {
			printf(", ");
		}
	}
	printf("\n");
}
//---------------------------------------------------------------------------------------------
int main(int argc, char const *argv[])
{
	printf("Enter length of array:\n");

	if (!scanf("%d", &ARRAY_LENGTH) || ARRAY_LENGTH < 0) {
		printf("Bad input!\n");
		return 1;
	}

	//alloc array
	int * numbersArray = (int*)malloc(ARRAY_LENGTH*sizeof(ARRAY_LENGTH));

	fillNumbersArrayWithRandomNumbers(numbersArray);

	printf("Array with random numbers before sort:\n");
	printArray(numbersArray);

	bubbleSort(numbersArray, ARRAY_LENGTH);

	printf("Array after bubble sort:\n");
	printArray(numbersArray);

	free(numbersArray);
	numbersArray = NULL;

	return 0;
}