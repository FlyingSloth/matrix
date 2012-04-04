#include "iostream"
#include "stdlib.h"
#include "pthread.h"
#include "matrix.h"
using namespace std;

int main(int argc, char** argv)
{
	Matrix m1;
	readMatrix(argv[1],m1);
}

