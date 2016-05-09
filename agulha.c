#include "agulha.h"

double agulhaBuffon(int r)
{
	double d, theta, expr, res;
	int i, t = 0;
	int L = 1, G = 1;

	srand( (unsigned) time(NULL) );

	printf("\t\tP3: Vou executar %d interacoes\n", r);


	for(i = 0; i < r; i++)
	{

		d = ((double)rand()/(double)RAND_MAX) * G;
		theta = ((double)rand()/(double)RAND_MAX)  *  2 * M_PI;	

		expr = d + L * sin(theta);


		if(expr < 0 || expr > G)
			t++;
	}

	res = t/(double)r;

	printf("\t\tP3: Probabilidade estimada: %f\n", res);
}