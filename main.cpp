#include <iostream>
#include <math.h>
#include <time.h>

#define COUNTER 10000000

int main() {
	std::cout << "TASK:" << std::endl;
	std::cout << "15.	F(x) = ((x+1)^2 - 1)/x, x != 0" << std::endl;
	std::cout << std::endl;

	try {
		do {
			double x;
			std::cout << "Input x:" << std::endl;
			std::cin >> x;
			if (std::cin.bad() || x == 0) throw std::cin.badbit;

			int counter = COUNTER + 1;

			double res1 = 0;
			clock_t start1 = clock();
			while (--counter) {
				res1 = (pow((x + 1), 2) - 1) / x;
			}
			clock_t end1 = clock();

			//std::cout << "math.h result: " << tmp1 <<  std::endl;
			printf("math.h result: %.6lf\n", res1);
			printf("Time taken: %.6lf sec\n", (double)(end1 - start1) / CLOCKS_PER_SEC);
			//td::cout << "Time taken: " << (double)(end1 - start1) / CLOCKS_PER_SEC << std::endl;

			double tmp2, res2 = 0, one = 1;
			counter = COUNTER + 1;
			clock_t start2 = clock();
			__asm finit;
			while (--counter) {
				__asm
				{
						fld x
						fadd one

						fstp tmp2
						fld tmp2
						fmul tmp2

						fsub one
						fdiv x

						fstp res2
				}
			}
			__asm fwait;
			clock_t end2 = clock();

			//std::cout << "Math coprocessor result: " << res << std::endl;
			printf("Math coprocessor result: %.6lf\n", res2);
			printf("Time taken: %.6lf sec\n", (double)(end2 - start2) / CLOCKS_PER_SEC);
			//std::cout << "Time taken: " << (double)(end2 - start2) / CLOCKS_PER_SEC << std::endl;

			system("pause");
			system("CLS");
		} while (true);
	}
	catch (...) {
		std::cout << "Execution stopped." << std::endl;
	}
 
    system("pause");
    return 0;
}

