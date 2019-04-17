#include <iostream>
#include <time.h>

using namespace std;

#define COUNTER 1000000
#define SIZE 4
#define MAX_VALUE 50

void showTask() {
	cout << "TASK:" << endl;
	cout << "15. XOR upon each element;" << endl;
	cout << "TERMS:" << endl;
	cout << "\t- array is filled randomly;" << endl;
	cout << "\t- array is " << SIZE << "x" << SIZE << ";" << endl;
	cout << "\t- elements are 0 to " << MAX_VALUE << ";" << endl;
	cout << "COUNTER VALUE:\t" << COUNTER << endl;
	cout << endl;
}

void showArray(int array[SIZE][SIZE]) {
	for (int i = 0; i < SIZE; i++) {
		cout << "\t";
		for (int j = 0; j < SIZE; j++) {
			cout << "\t" << array[i][j];
		}
		cout << endl;
	}
	cout << endl;
}

int main() {

	int arr1[SIZE][SIZE], arr2[SIZE][SIZE], arr3[SIZE][SIZE]; //arr3 will contain results
	time_t seed;
	srand((unsigned)time(&seed));

	try {
		do {
			showTask();

			double x;
			cout << "Enter any number to perform the task:" << endl;
			cin >> x;
			if (std::cin.fail()) throw std::cin.failbit;

			for (int i = 0; i < SIZE; i++)
				for (int j = 0; j < SIZE; j++) {
					arr1[i][j] = rand() % MAX_VALUE;
					arr2[i][j] = rand() % MAX_VALUE;
				}

			cout << "Array 1:" << endl;
			showArray(arr1);

			cout << "Array 2:" << endl;
			showArray(arr2);

			int counter = COUNTER + 1;
			int intSize = sizeof(int);
			int doubleIntSize = sizeof(int) * 2;
			int MMXCounter = SIZE * SIZE * intSize * 8 / 64;

			cout << "Calculations in C are started..." << endl;
			clock_t startC = clock();
			while (--counter) {
				for (int i = 0; i < SIZE; i++)
					for (int j = 0; j < SIZE; j++) {
						arr3[i][j] = arr1[i][j] ^ arr2[i][j];
					}
			}
			clock_t endC = clock();

			counter = COUNTER + 1;

			cout << "Calculations in ASM are started..." << endl;
			int asmCounter = SIZE * SIZE;
			clock_t startASM = clock();
			while (--counter) {
				__asm
				{
					mov ecx, asmCounter
					xor esi, esi
					start:
					mov eax, arr1[esi]
					mov edx, arr2[esi]

					xor edx, eax
					mov arr3[esi], dx

					add esi, intSize
					loop start
				}
			}
			clock_t endASM = clock();

			counter = COUNTER + 1;

			cout << "Calculations in ASM using MMX are started..." << endl;
			clock_t startMMX = clock();
			while (--counter) {
				_asm
				{
					mov ecx, MMXCounter
					xor esi, esi
					startM :
					movq mm0, arr1[esi]
						movq mm1, arr2[esi]

						pxor mm0, mm1
						movq arr3[esi], mm0

						add esi, doubleIntSize
						loop startM
						emms
				}
			}
			clock_t endMMX = clock();
			

			printf("Result array:\n");
			showArray(arr3);
			printf("\n");

			printf("ASM with MMX results:\n");
			printf("Time taken: %.6lf sec\n", (double)(endMMX - startMMX) / CLOCKS_PER_SEC);
			double MMXPercentage = 1 * 100;
			printf("Percentage: %.2lf%\n", MMXPercentage);

			printf("ASM results:\n");
			printf("Time taken: %.6lf sec\n", (double) (endASM - startASM) / CLOCKS_PER_SEC);
			double asmPercentage = (double)(endASM - startASM) / (endMMX - startMMX) * 100;
			printf("Percentage: %.2lf%\n", asmPercentage);

			printf("C results:\n");
			printf("Time taken: %.6lf sec\n", (double) (endC - startC) / CLOCKS_PER_SEC);
			double CPercentage = (double) (endC - startC) / (endMMX - startMMX) * 100;
			printf("Percentage: %.2lf%\n", CPercentage);

			system("pause");
			system("CLS");
		} while (true);
	}
	catch (...) {
		cout << "Execution stopped." << endl;
	}

	system("pause");
	return 0;
}