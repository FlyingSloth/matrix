#include "iostream"
#include "stdlib.h"
#include "pthread.h"
#include "matrix.h"
using namespace std;

int main(int argc, char** argv)
{
	Matrix m1;
	m1 = readMatrix(argv[1],m1);
	cout << m1.cols << " " << m1.rows << "\n";
	for (int i = 0; i <m1.rows; i++)
	{
		for (int j = 0; j < m1.cols; j++)
			cout << m1.matr[i][j] << " ";
		cout << "\n";
	}
}

