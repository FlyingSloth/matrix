#include "iostream"
#include "stdlib.h"
#include "pthread.h"
#include "matrix.h"
#include <ctime>
using namespace std;

int main(int argc, char** argv)
{
	Matrix m1;
	m1 = readMatrix(argv[1],m1);
	Matrix m2;
	m2 = readMatrix(argv[2],m2);
	Matrix sum;
	const clock_t start = clock();
	sum = summ(m1,m2);
	const double time = static_cast<double>(clock() - start);

	//cout << sum.cols << " " << sum.rows << "\n";
	for (int i = 0; i <sum.rows; i++)
	{
		for (int j = 0; j < sum.cols; j++)
			cout << sum.matr[j][i] << " ";
		cout << "\n";
	}
    cout << time << "\n";
	/*
	for (int i = 0; i <m1.cols; i++)
	{
		for (int j = 0; j < m1.rows; j++)
			cout << m1.matr[i][j] << " ";
		cout << "\n";
	}
	*/
	return 0;
}

