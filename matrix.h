#include "iostream"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "pthread.h"
#include "fstream"
#include "sstream"
using namespace std;

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
	double* tmp = new double[1000];
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

	m.cols = col;
	m.rows = row;
	
	m.matr = new double*[m.cols];
	for (int i = 0; i < m.cols; i++)
		m.matr[i] = new double[m.rows];
		
	for (int i = 0; i < m.rows; i++)
		for (int j = 0; j < m.cols; j++)
		{
			m.matr[i][j] = tmp[j+i*m.cols];
		}
	delete[] tmp;

	return m;
	/*for (int i = 0; i < m.rows; i++)
		for (int j = 0; j < m.cols; j++)
		{
			printf("[%d][%d] %f ", i, j, m.matr[i][j]);
			cout << "\n";
		}*/
}

