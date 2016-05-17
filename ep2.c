/* ********************************
    EP2 - Programação Concorrente
    Prof. Marcel Jackowski

    Danilo Aleixo Gomes de Souza
    n USP: 7972370
  
********************************** */

#include "StringOps.h"
#include "agulha.h"
#include <pthread.h>
#include <time.h>

#define TRUE 1
#define FALSE 0

/* VARIAVEIS GLOBAIS */
int n, p, t, r, s;
int alunosQueFestaram = 0;

/* */
pthread_mutex_t mutex_alunosQueFestaram = PTHREAD_MUTEX_INITIALIZER;


void *aluno(void *numeroAluno)
{
	int tempoNaFesta = 0;

	printf("Aluno %d esta na porta.\n", (int) numeroAluno);

	/*Verifica se o segurança ta dando um role */
	printf("Aluno %d esta na festa.\n", (int) numeroAluno);

	tempoNaFesta = rand() % r;
	usleep(tempoNaFesta * 1000); /* tempo que passa na festa */


	/* aluno saiu da festa */
	pthread_mutex_lock(&mutex_alunosQueFestaram);
	alunosQueFestaram++;
	pthread_mutex_unlock(&mutex_alunosQueFestaram);
	printf("Aluno %d vai embora, numero total: %d\n", (int) numeroAluno, alunosQueFestaram);

	return(NULL);
}
void *seguranca(void *arg)
{
	int tempoRonda = 0;
	printf("to aqui\n");
	while(alunosQueFestaram != n)
	{
		printf("Seguranca em ronda\n");

		tempoRonda = rand() % s;
		usleep(tempoRonda * 1000); /* tempo da ronda */

		printf("Seguranca na porta\n");		
	}
	printf("A festa acabou\n");
	return(NULL);
}


int main(int argc, char *argv[])
{

	int i, j;
	int intervaloChegadaAluno = 0;
	pthread_t *thread_alunos, thread_seguranca;
	int *tid_alunos, tid_seguranca, *numeroAluno;

	/* inicializa mutexes */
	pthread_mutex_init(&mutex_alunosQueFestaram, NULL);

	/* alimenta o rand com uma seed*/
	srand( (unsigned)time(NULL) );


	if(argc >= 5)
	{
		/* Atualiza os inteiros com as entradas */
		n = atoi(argv[1]);
		p = atoi(argv[2]);
		t = atoi(argv[3]);
		r = atoi(argv[4]);
		s = atoi(argv[5]);

		/* aloca o vetor de threads e de ids de alunos */
		thread_alunos = mallocSafe(n * sizeof(pthread_t));
		tid_alunos = mallocSafe(n * sizeof(int));

		/* numeroAluno sera passado para thread com o numero de cada aluno */
		numeroAluno = mallocSafe(n * sizeof(int));
		for(i = 0; i < n; i++)
			numeroAluno[i] = i+1;


		/* cria thread para os alunos e thread seguranca */
		for(i = 0; i < n; i++)
		{
			intervaloChegadaAluno = rand() % t; /* gera o intervalo maximo entre alunos aleatorio de 1 a t */
			tid_alunos = pthread_create(&thread_alunos[i], NULL, aluno, (void *) numeroAluno[i]);
			usleep(intervaloChegadaAluno * 1000); /* espera até o proximo aluno chegar na festa */
		}
		tid_seguranca = pthread_create(&thread_seguranca, NULL, seguranca, NULL);


		pthread_join(thread_seguranca, NULL);



		printf("Acabou o programa\n");

	}
	else {
		printf("\n\nModo de uso\n\n");
        printf("./ep2 <convidados> <minimo-alunos> <intervalo> <tempo-maximo> <tempo-ronda>\n");
        printf("Onde:\n");
        printf("<convidados>: 			numero total de convidados da festa\n");
        printf("<minimo-alunos>: 		numero minimo de alunos na festa para o seguranca esvaziar a sala\n");
        printf("<intervalo>: 			intervalo maximo de tempo dentre chegadas de convidados em ms\n");
        printf("<tempo-maximo>: 		tempo maximo de participacao na festa para cada aluno em ms\n");
        printf("<tempo-ronda>: 			tempo maximo de ronda do seguranca em ms\n");
        printf("\n");
	}
	


  	return 0;
}

