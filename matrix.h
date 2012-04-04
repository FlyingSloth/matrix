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

bool readMatrix(char* adr, Matrix m)
{
	FILE *M;
	M = fopen(adr, "r");
	m.cols = 0;
	m.rows = 0;
	char* c;
	char* buf;
	double tmp;
	int col = 0;
	int prevcol = 0;
	int row = 0;
	size_t len = 0;

	while(getline(&c,&len,M) != -1)
	{
		prevcol = col;
		col = 1;
		buf = strtok(c, " ");
		while(buf != NULL)
		{
			col ++;
			buf = strtok(NULL, "");
		}
		row ++;
		if ((prevcol != col) && (row != 1)) return false;
	}
	m.cols = col;
	m.rows = row;
	
	double** elem = new double*[m.cols];
	for (int i = 0; i < m.cols; i++)
		elem[i] = new double[m.rows];
	while(getline(&c,&len,M) != -1)
	{
		prevcol = col;
		col = 0;
		buf = strtok(c, " ");
		while(buf != NULL)
		{
			std::stringstream ss(buf);
			ss >> tmp;
			elem[col][row] = tmp;
			cout << "elem ";
			col ++;
			buf = strtok(NULL, "");
		}
		cout << "\n";
		row ++;
		if ((prevcol != col) && (row != 1)) return false;
		cout << "c: " << col << " r: " << row << "\n";
	}
	char* cc;
	for (int i = 0; i < m.rows; i++)
		for (int j = 0; j < m.cols; j++)
		{
			printf("%f ",elem[i][j]);
			//cout << (char*)(elem[j][i]) << " ";
			cout << "\n";
		}
}

