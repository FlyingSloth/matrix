#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "omp.h"
#include "fstream"
#include "sstream"
using namespace std;

unsigned long maxval = 50000000;

struct Matrix
{
	double** matr;
	int cols, rows;
};

bool readMatrix(char* adr, Matrix &m)
{
	FILE *M;
	M = fopen(adr, "r");
	if ( M == NULL) return false; 
	m.cols = 0;
	m.rows = 0;
	char* c;
	char* buf;
	double* tmp = new double[maxval];//буфер для матрицы
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
			count++;
			col ++;
			buf = strtok(NULL, " ");
		}
		row ++;
		if ((prevcol != col) && (row != 1)) {cout << "Wrong array size!\n"; exit(1);};
	}

	m.cols = col-1;
	m.rows = row;
	
	m.matr = new double*[m.cols];
	for (int i = 0; i < m.cols; i++)
		m.matr[i] = new double[m.rows];
		
	for (int i = 0; i < m.cols; i++)
		for (int j = 0; j < m.rows; j++)
		{
			m.matr[i][j] = tmp[i+j*m.cols];
		}
	delete[] tmp;
	return true;
	
};

void summ(Matrix m1, Matrix m2, Matrix &sum, FILE *file = NULL)
{
	if ((m1.cols != m2.cols) || (m1.rows != m2.rows))
		{cout << "Array sizes don't match\n"; exit(1);};
	
	sum.matr = new double*[m1.cols];
	for (int i = 0; i < m1.cols; i++)
		sum.matr[i] = new double[m1.rows];
		
	#pragma omp parallel for
	for (int i = 0; i < m1.cols; i++)
		#pragma omp parallel for
		for (int j = 0; j < m1.rows; j++)
		{
			sum.matr[j][i] = m1.matr[j][i] + m2.matr[j][i];
		}
	sum.cols = m1.cols;
	sum.rows = m1.rows;

	if (file == NULL)
	{
		for (int i = 0; i <sum.rows; i++)
			{
				for (int j = 0; j < sum.cols; j++)
					printf("%f ", sum.matr[j][i]);
				printf("\n");
			}
	}
	else
	{
		for (int i = 0; i <sum.rows; i++)
			{
				for (int j = 0; j < sum.cols; j++)
					fprintf(file, "%f ", sum.matr[j][i]);
				fprintf(file, "\n");
			}
	}
};

