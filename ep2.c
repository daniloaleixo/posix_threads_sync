/* ********************************
    EP2 - Programação Concorrente
    Prof. Marcel Jackowski
    Danilo Aleixo Gomes de Souza
    n USP: 7972370
  
********************************** */

#include "StringOps.h"
#include <pthread.h>
#include <time.h>
#include <semaphore.h>

#define TRUE 1
#define FALSE 0
#define SHARED 1

/* VARIAVEIS GLOBAIS */
int n, p, t, r, s;
int alunosQueFestaram = 0, segurancaRonda = FALSE;

/* VARIAVEIS PTHREAD */
pthread_mutex_t mutex_alunosQueFestaram = PTHREAD_MUTEX_INITIALIZER, mutex_segurancaEmRonda = PTHREAD_MUTEX_INITIALIZER, 
				mutex_segurancaPorta = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t aindaTemAlunos, segurancaNaRonda;
sem_t sem_alunosNaSala;


void *aluno(void *numeroAluno)
{
	int tempoNaFesta = 0, nAlunosNaSala;

	printf("Aluno %d esta na porta.\n", (int) numeroAluno);

	
	/* Verifica se o segurança ta em ronda */
	pthread_mutex_lock(&mutex_segurancaEmRonda);
	while(!segurancaRonda)
	{
		if(!segurancaRonda) pthread_cond_wait(&segurancaNaRonda, &mutex_segurancaEmRonda);
	}
	pthread_mutex_unlock(&mutex_segurancaEmRonda);


	/* coloca o aluno na festa */
	sem_post(&sem_alunosNaSala);
	printf("Aluno %d esta na festa.\n", (int) numeroAluno);


	/* tempo que passa na festa */
	tempoNaFesta = rand() % r;
	usleep(tempoNaFesta * 1000); 


	/* aluno saiu da festa */
	pthread_mutex_lock(&mutex_segurancaPorta);
	pthread_mutex_lock(&mutex_alunosQueFestaram);
	alunosQueFestaram++;
	sem_wait(&sem_alunosNaSala);
	sem_getvalue(&sem_alunosNaSala, &nAlunosNaSala);
	pthread_mutex_unlock(&mutex_alunosQueFestaram);


	/* verifica se tem mais alunos na sala */
	if(nAlunosNaSala == 0)	
	{
		pthread_cond_broadcast(&aindaTemAlunos);
	}
	pthread_mutex_unlock(&mutex_segurancaPorta);


	printf("Aluno %d vai embora.\n", (int) numeroAluno);

	return(NULL);
}


void *seguranca(void *arg)
{
	int tempoRonda = 0, nAlunosNaSala = 0;

	while(alunosQueFestaram != n)
	{
		/* seguranca vai fazer a ronda */
		pthread_mutex_lock(&mutex_segurancaEmRonda);
		segurancaRonda = TRUE;
		printf("Seguranca em ronda\n");
		pthread_cond_broadcast(&segurancaNaRonda);
		pthread_mutex_unlock(&mutex_segurancaEmRonda);
		
		
		tempoRonda = rand() % s;
		usleep(tempoRonda * 1000); /* tempo da ronda */


		/* seguranca termina a ronda e vai para a porta */
		pthread_mutex_lock(&mutex_segurancaPorta);
		pthread_mutex_lock(&mutex_segurancaEmRonda);
		segurancaRonda = FALSE;
		printf("Seguranca na porta\n");
		pthread_mutex_unlock(&mutex_segurancaEmRonda);
		
		/* Verifica se tem os alunos na festa, se tiver expulsa eles */
		sem_getvalue(&sem_alunosNaSala, &nAlunosNaSala);
		if(nAlunosNaSala == 0)
			printf("Seguranca inspeciona a sala\n");
		else{
			if(nAlunosNaSala >= p)
			{
				printf("Seguranca expulsa os alunos\n");
				/* Expulsa todos os alunos */
				while(nAlunosNaSala > 0)
				{
					pthread_cond_wait(&aindaTemAlunos, &mutex_segurancaPorta);
					sem_getvalue(&sem_alunosNaSala, &nAlunosNaSala);
				}				
			}
		}
		pthread_mutex_unlock(&mutex_segurancaPorta);
	}
	return(NULL);
}


int main(int argc, char *argv[])
{

	int i;
	int intervaloChegadaAluno = 0;
	pthread_t *thread_alunos, thread_seguranca;
	int *tid_alunos, tid_seguranca, *numeroAluno;

	/* inicializa mutexes, conds e semaforos */
	pthread_mutex_init(&mutex_alunosQueFestaram, NULL);
	pthread_mutex_init(&mutex_segurancaEmRonda, NULL);
	pthread_mutex_init(&mutex_segurancaPorta, NULL);
	sem_init(&sem_alunosNaSala, SHARED, 0);
	pthread_cond_init(&aindaTemAlunos, NULL);
	pthread_cond_init(&segurancaNaRonda, NULL);

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
		tid_seguranca = pthread_create(&thread_seguranca, NULL, seguranca, NULL);
		for(i = 0; i < n; i++)
		{
			intervaloChegadaAluno = rand() % t; /* gera o intervalo maximo entre alunos aleatorio de 1 a t */
			tid_alunos = pthread_create(&thread_alunos[i], NULL, aluno, (void *) numeroAluno[i]);
			usleep(intervaloChegadaAluno * 1000); /* espera até o proximo aluno chegar na festa */
		}


		for(i = 0; i < n; i++)
			pthread_join(thread_alunos[i], NULL);	
		pthread_join(thread_seguranca, NULL);

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
	
	printf("Termino da festa\n");	

  	return 0;
}


