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
	Matrix m1;
	Matrix m2;
	Matrix result;
	char* m[2];
	FILE* out = NULL;
	
	if (argc == 1)
		help(argv[0]);
	else
	{
		static struct option long_options[] = {
		                   {"out",  			1, 0, 'o'},
		                   {"summ",  			1, 0, 's'},
		                   {"read",  			1, 0, 'r'},
		                   {"mult",  			1, 0, 'm'},
		                   {"help",  			1, 0, 'h'},
		                   {0, 				    0, 0,  0 }
		               };


		int c;

		    char  foname[1024];
		    int i = 0;
		    char* tok;
		    int option_index = 0;
		    strcpy(foname, "$none$");

		    while( (c=getopt_long (argc, argv, "o:s:r:m:", long_options, &option_index)) !=-1)
		    {
		        switch(c)
		        {
		            case 'o': // имя выходного файла
		                strcpy(foname,optarg);
		                out = fopen(foname,"w");
		                break;
		            case 's': // сумма
		                tok = strtok(optarg, " ");
		                while (tok != NULL)
		                {
		                	m[i] = tok;
		                	i++;
		                	tok = strtok(NULL," ");
		                }
		                if (readMatrix(m[0],m1) && readMatrix(m[1],m2))
						{
							summ(m1,m2,result,out);
						}
		                break;
	                case 'm': // произведение
			            tok = strtok(optarg, " ");
			            while (tok != NULL)
			            {
			            	m[i] = tok;
			            	i++;
			            	tok = strtok(NULL," ");
			            }
			            if (readMatrix(m[0],m1) && readMatrix(m[1],m2))
						{
							multiply(m1,m2,result,out);
						}
			            break;
		            case 'r': // чтение матрицы
		                readMatrix(optarg,m1);
		                break;
		            case 'h': // справка
		                help(argv[0]);
		                break;
		            default:
		            break;
		        }
		    };
        }
	return 0;
}

void help(char* arg)
{
	printf("\t\t\t=== Usage ===\n");
	printf("--help, -h  - this message\n");
	printf("--out, -o - path to output(result) file\n");
	printf("--summ, -s  - summarise, ex:\" %s \" -s \"matrix1 matrix2\". Returns summary of 2 matrixes\n",arg);	
}

