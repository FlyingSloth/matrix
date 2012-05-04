#include "fstream"
#include "iostream"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "getopt.h"
//#include "pthread.h"
#include "omp.h"
#include "time.h"
using namespace std;


void gen(char* output, int cols, int rows);
void help()
{
	printf(" -o, --out   -  path to output file\n");
	printf(" -c, --cols  -  matrix columns\n");
	printf(" -r, --rows  -  matrix rows\n");
};


int main(int argc, char *argv[])
{
	if (argc == 1) help();
	else
	{
		static struct option long_options[] = {
		                   {"out",  			1, 0, 'o'},
		                   {"cols",  			1, 0, 'c'},
		                   {"rows",  		    1, 0, 'r'},
		                   {0, 				    0, 0,  0 }
		               };


			int c;

		    char  foname[1024];
		    int cols = 0;
		    int rows = 0;

		    int option_index = 0;

		    strcpy(foname, "$none$");

		    while( (c=getopt_long (argc, argv, "o:c:r:", long_options, &option_index)) !=-1)
		    {
		        switch(c)
		        {
		            case 'o': // имя выходного файла
		                strcpy(foname,optarg);
		                break;
		            case 'c': // количество столбцов
		                cols = atoi(optarg);
		                break;
		            case 'r': // количество строк
		                rows = atoi(optarg);
		                break;
		            default:
		            break;
		        }
		    };
		    gen(foname,cols,rows);
        }
        return 0;
};

void gen(char* output, int cols, int rows)
{
    FILE* out = fopen(output,"w");
	srand(time(NULL));
	#pragma omp parallel for
    for (int i = 0; i < rows; i++)
    {
		#pragma omp parallel for
    	for (int j = 0; j < cols; j++)
    	{
    		fprintf(out, "%f ", (double)(rand()%100+1)/(rand()%100+1));
    	}	
    	fprintf(out, "\n");
	}
};

