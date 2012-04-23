#include "iostream"
#include "stdlib.h"
#include "pthread.h"
#include "matrix.h"
#include "getopt.h"
#include <time.h>
using namespace std;

void help(char* arg);

int main(int argc, char** argv)
{
	Matrix sum;
	char* m[2];
	FILE* out;
	bool toFile = false;
	
	if (argc == 1)
		help(argv[0]);
	else
	{
		static struct option long_options[] = {
		                   {"out",  			1, 0, 'o'},
		                   {"summ",  			1, 0, 's'},
		                   {"help",  			1, 0, 'h'},
		                   {0, 				    0, 0,  0 }
		               };


		int c;

		    char  foname[1024];
		    int i = 0;
		    char* tok;
		    int option_index = 0;
		    strcpy(foname, "$none$");

		    while( (c=getopt_long (argc, argv, "o:s:", long_options, &option_index)) !=-1)
		    {
		        switch(c)
		        {
		            case 'o': // имя выходного файла
		                strcpy(foname,optarg);
		                out = fopen(foname,"w");
		                toFile = true;
		                break;
		            case 's': // сумма
		                tok = strtok(optarg, " ");
		                while (tok != NULL)
		                {
		                	m[i] = tok;
		                	i++;
		                	tok = strtok(NULL," ");
		                }
		                break;
		            case 'h': // справка
		                help(argv[0]);
		                break;
		            default:
		            break;
		        }
		    };
        }
        

	Matrix m1;
	Matrix m2;
	
	if (readMatrix(m[0],m1) && readMatrix(m[1],m2))
	{
		if (toFile)
			summ(m1,m2,sum,out);
		else summ(m1,m2,sum);
	}
	return 0;
}

void help(char* arg)
{
	cout << "\t\t\t=== Usage ===\n";
	cout << "--help, -h  - this message\n";
	cout << "--out, -o - path to output(result) file\n";
	cout << "--summ, -s  - summarise, ex:" << arg << " -s \"matrix1 matrix2\". Returns summary of 2 matrixes\n";	
}

