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

void print(Matrix result, FILE *file)
{
	if (file == NULL)
	{
		for (int i = 0; i < result.rows; i++)
			{
				for (int j = 0; j < result.cols; j++)
					printf("%f ", result.matr[j][i]);
				printf("\n");
			}
	}
	else
	{
		for (int i = 0; i <result.rows; i++)
			{
				for (int j = 0; j < result.cols; j++)
					fprintf(file, "%f ", result.matr[j][i]);
				fprintf(file, "\n");
			}
	}
}

bool readMatrix(char* adr, Matrix &m, bool flagPrint = false, FILE *file = NULL)
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
	
	if (flagPrint)
		print(m, file);
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

	print(sum, file);
};

void multiply(Matrix m1, Matrix m2, Matrix &mult, FILE *file)
{
	double tmp = 0.0;
	
	if (m1.cols != m2.rows)
		{printf("Array sizes don't match\n"); exit(1);};

	mult.matr = new double*[m2.cols];
		for (int i = 0; i < m2.cols; i++)
			mult.matr[i] = new double[m1.rows];
	
	mult.cols = m2.cols;
	mult.rows = m1.rows;

	int i,j,k;
	for (k = 0; k < m2.cols; k++)
	{
		for (j = 0; j < m1.rows; j++)
		{
			for (i = 0; i < m1.cols; i++)
			{
				tmp += m1.matr[i][j]*m2.matr[k][i];
			}
			mult.matr[k][j] = tmp;
			tmp = 0;
		}
	}
	
	print(mult, file);
}

Matrix uminus(Matrix m, FILE *file)
{
	#pragma omp parallel for
	for (int j = 0; j < m.rows; j++)
	{
		for (int i = 0; i < m.cols; i++)
		{
			m.matr[i][j]*=-1;
		}
	}
	
	print(m, file);
}

void minors(Matrix m, Matrix* mbuf)
{
	if (m.cols != m.rows)
	{
		printf("Wrong array size! Cols != Rows\n");
		exit(1);
	}
	//mbuf = new Matrix[m.cols];

	int c = 0;
	for (int co = 0; co < m.cols; co++)
	{
		mbuf[co].cols = m.cols-1;
		mbuf[co].rows = m.rows-1;
		mbuf[co].matr = new double*[mbuf[co].cols];
		for (int i = 0; i < mbuf[co].cols; i++)
		{
			mbuf[co].matr[i] = new double[mbuf[co].rows];
		}
	}
	for (int i = 0; i < m.cols; i++)
	{
		for (int k = 0; k < mbuf[i].rows; k++)
		{
			c = 0;
			for (int l = 0; l < mbuf[i].cols; l++)
			{
				if (c < i)
				{
					mbuf[i].matr[l][k] = m.matr[l][k+1];
				}
				if (c>=i)
				{
					mbuf[i].matr[l][k] = m.matr[l+1][k+1];
				}
			c++;
			}
		}
	}
	/*for (int i = 0; i < m.cols; i++)
	{
		print(mbuf[i], NULL);
		printf("\n");
	}*/
}

double determ(Matrix m,int coef = 1)
{
	double det = 0;
	Matrix mbuf[m.cols];
	if (m.cols != 2)
	{
		minors(m,mbuf);
		for (int i = 0; i < m.cols; i++)
		{
			det += coef*determ(mbuf[i],coef)*m.matr[i][0];
			printf("c %d  m %f \n", coef,m.matr[i][0]);
			coef*=-1;
		}
	}
	if (m.cols == 2) 
	{
		det = (m.matr[0][0]*m.matr[1][1]-m.matr[0][1]*m.matr[1][0]);
		printf("%f\n", det);
	}
	return det;
	/*
	for (int i = 0; i < m.cols; i++)
	{
		print(mbuf[i], NULL);
		printf("\n");
	}*/
	//return 0;
	/*else
	{
		return (m.matr[0][0]*m.matr[1][1]-m.matr[0][1]*m.matr[1][0]);
	}*/
}

