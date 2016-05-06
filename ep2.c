/* ********************************
    EP2 - Programação Concorrente
    Prof. Marcel Jackowski

    Danilo Aleixo Gomes de Souza
    n USP: 7972370
  
********************************** */

#include "StringOps.h"
#include "agulha.h"
#include <pthread.h>

#define TRUE 1
#define FALSE 0


int main(int argc, char *argv[])
{

	int *v, i;
	int m, n, r, s;
	double x = 0.5;
	pid_t pids[4];

	if(argc >= 4)
	{
		/* Atualiza os inteiros com as entradas */
		m = atoi(argv[1]);
		n = atoi(argv[2]);
		r = atoi(argv[3]);
		s = atoi(argv[4]);

		/* flags especiais */		
		if(argv[5] != NULL) 
			if(argv[5][1] == 'x') /* --x <valor de x> */
				x = atof(argv[6]);



	    printf("P0: Pai começou a rodar\n");
	    if (!(pids[0] = fork())) {
	    	// P1
	        printf("\tP1 sendo criado\n");
	       	/*v = criaVetor(m);
			printf("\t\tP1: Vetor:          "); imprimeVetor(v, m);
			heapsort(v, m);
			printf("\t\tP1: Vetor ordenado: "); imprimeVetor(v, m);*/
	        exit(0);
	    } 
	    else if (!(pids[1] = fork())) {
	        // P2
	        printf("\tP2 sendo criado\n");
	        /*Fibonacci(n, 0); */
	        exit(0); 

	    } 
	    else if (!(pids[2] = fork())) {
	        // P3
	        printf("\tP3 sendo criado\n");
	        agulhaBuffon(r);
	        exit(0);
	    
	    } 
	    else if (!(pids[3] = fork())) {
	        // P4
	        printf("\tP4 sendo criado\n");
	        /*integralSecX(x, s);*/
	        exit(0);
	    } 
	    else {
	        // Volta para o Pai
	        printf("P0: Esperando os filhos retornarem\n");

	        for(i = 0; i < 4; i++)
	        {
	        	wait(pids[i]);
	            printf("\tO processo P%d foi encerrado\n",i+1);
	        }
	    }
	    printf("P0: Pai sera encerrado\n");

	}
	else {
		printf("\n\nModo de uso\n\n");
        printf("ep1 <argumento heapsort> <argumento Fibonacci> <argumento> <argumento integral de sec x> <flags>\n\n");
        printf("Flags:\n");
        printf("-x <valor de x>\n");
        printf("para modificar o valor de x, no calculo da integral de sec x\n\n");
	}
	


  	return 0;
}

