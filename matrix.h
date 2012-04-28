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
	M = fopen(adr, "r+");
	if ( !M ) 
	{
		printf("Matrix at \" %s \" not found\n", adr);
		return false; 
	}
	m.cols = 0;
	m.rows = 0;
	char* c;
	char* buf;
	double* tmp = new double[maxval];//буфер для матрицы
	int col = 0;
	int prevcol = 0; //для сравнения, едина ли длина всех строк матрицы
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
			//printf("\"%s\"\n",buf);
		}
		row ++;
		if ((prevcol != col) && (row != 1)) 
		{
			printf("Wrong array size!\n");
			return false;
		}; // не совпали строки по длине(
	}

	m.cols = col-1;
	m.rows = row;
	
	m.matr = new double*[m.cols];
	for (int i = 0; i < m.cols; i++)
		m.matr[i] = new double[m.rows];
		
	for (int i = 0; i < m.cols; i++)
		for (int j = 0; j < m.rows; j++)
			m.matr[i][j] = tmp[i+j*(col)];

	/*for (int j = 0; j < m.rows; j++)
	{
		for (int i = 0; i < m.cols; i++)
		{
			printf("\"%f\" ",m.matr[i][j]);
		}
		printf("\n");
	}
		printf("read it!!!\n");
		printf("cols: %d, rows: %d\n", m.cols, m.rows);*/
	delete[] tmp;
	return true;
};

void summ(Matrix m1, Matrix m2, Matrix &sum, FILE *file)
{
	if ((m1.cols != m2.cols) || (m1.rows != m2.rows))
		{printf("Array sizes don't match\n"); exit(1);};
	
	sum.matr = new double*[m1.cols];
	for (int i = 0; i < m1.cols; i++)
		sum.matr[i] = new double[m1.rows];
		
	#pragma omp parallel for
	for (int i = 0; i < m1.cols; i++)
		#pragma omp parallel for
		for (int j = 0; j < m1.rows; j++)
		{
			sum.matr[i][j] = m1.matr[i][j] + m2.matr[i][j];
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

void multiply(Matrix m1, Matrix m2, Matrix &mult, FILE *file)
{
	double tmp = 0.0;
	
	//if (m1.cols != m2.rows)
		//{printf("Array sizes don't match\n"); exit(1);};
		
	printf("m1 c:%d r:%d m2 c:%d r:%d\n", m1.cols, m1.rows, m2.cols, m2.rows);
	mult.matr = new double*[m2.cols];
		for (int i = 0; i < m2.cols; i++)
			mult.matr[i] = new double[m1.rows];
	
	mult.cols = m2.cols;
	mult.rows = m1.rows;
	
	/*printf("cols: %d, rows: %d\n", mult.cols, mult.rows);
	
	for (int i = 0; i < m1.rows; i++)
	{
		for (int j = 0; j < m1.cols; j++)
			printf("[%d,%d] = %f", i, j, m1.matr[j][i]);
		printf("\n");
	}*/
	
	int i,j,k;
	for (k = 0; k < m2.cols; k++)
	{
		for (j = 0; j < m1.rows; j++)
		{
			for (i = 0; i < m1.cols; i++)
			{
				tmp += m1.matr[i][j]*m2.matr[k][i];
				//printf("%d %d %d %f %f %f\n",k,j,i, m1.matr[j][i],m2.matr[i][k], tmp);
			}
			mult.matr[k][j] = tmp;
			tmp = 0;
			//printf("%f ",mult.matr[k][j]);
		}
//		printf("\n");
	}
	
	for (int j = 0; j < mult.rows; j++)
	{
		for (int i = 0; i < mult.cols; i++)
		{
			printf("\"%f\" ",mult.matr[i][j]);
		}
		printf("\n");
	}
}

