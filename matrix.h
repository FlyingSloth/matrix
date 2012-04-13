#include "iostream"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "omp.h"
#include "fstream"
#include "sstream"
using namespace std;

long maxval = 42000000000;
struct Matrix
{
	double** matr;
	int cols, rows;
};

Matrix readMatrix(char* adr, Matrix m)
{
	FILE *M;
	M = fopen(adr, "r");
	m.cols = 0;
	m.rows = 0;
	char* c;
	char* buf;
	double* tmp = new double[maxval];
	int col = 0;
	int prevcol = 0;
	int row = 0;
	int count = 0;
	size_t len = 0;

	while(getline(&c,&len,M) != -1)
	{
		prevcol = col;
		col = 0;
		buf = strtok(c, " ");
		while(buf != NULL)
		{
			tmp[count] = atof(buf);
			//cout << tmp[count] << " ";
			count++;
			col ++;
			buf = strtok(NULL, " ");
		}
		row ++;
		if ((prevcol != col) && (row != 1)) {cout << "Wrong array size!\n"; exit(1);};
		//cout << "\n";
	}

	m.cols = col;
	m.rows = row;
	
	//cout << col << " " << row << "\n";
	m.matr = new double*[m.cols];
	for (int i = 0; i < m.cols; i++)
		m.matr[i] = new double[m.rows];
		
	for (int i = 0; i < m.cols; i++)
		for (int j = 0; j < m.rows; j++)
		{
			m.matr[i][j] = tmp[i+j*m.cols];
			//cout << "[" << i << "][" << j<< "]" << m.matr[i][j] << "\n";
		}
	delete[] tmp;
	/*
	for (int i = 0; i < m.rows; i++)
		for (int j = 0; j < m.cols; j++)
		{
			printf("[%d][%d] %f ", i, j, m.matr[i][j]);
			cout << "\n";
		}
	*/
	return m;
	
};

Matrix summ(Matrix m1, Matrix m2)
{
	if ((m1.cols != m2.cols) || (m1.rows != m2.rows))
		{cout << "Array sizes don't match\n"; exit(1);};
	
	Matrix sum;
	sum.matr = new double*[m1.cols];
	for (int i = 0; i < m1.cols; i++)
		sum.matr[i] = new double[m1.rows];
		
	#pragma omp parallel for
	for (int i = 0; i < m1.cols; i++)
		#pragma omp parallel for
		for (int j = 0; j < m1.rows; j++)
			sum.matr[i][j] = m1.matr[i][j] + m2.matr[i][j];
	return sum;
};

