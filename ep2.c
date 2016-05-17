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
#include <semaphore.h>

#define TRUE 1
#define FALSE 0
#define SHARED 1

/* VARIAVEIS GLOBAIS */
int n, p, t, r, s;
int alunosQueFestaram = 0, segurancaRonda = FALSE;
/* */
pthread_mutex_t mutex_alunosQueFestaram = PTHREAD_MUTEX_INITIALIZER, mutex_segurancaEmRonda = PTHREAD_MUTEX_INITIALIZER, m1, m2;
pthread_cond_t aindaTemAlunos, segurancaExpulsando;
sem_t sem_segurancaRonda, sem_alunosNaSala;

clock_t begin, end;
double time1;


void *aluno(void *numeroAluno)
{
	int tempoNaFesta = 0, nAlunosNaSala;
	time1 = clock() - begin;
	printf("%lf \tAluno %d esta na porta.\n", time1, (int) numeroAluno);

	
	/* Verifica se o segurança ta em ronda */
	pthread_mutex_lock(&m2);
	while(!segurancaRonda)
	{
		/* DEPURACAO */printf(">>to aqui e o seguranca nao esta mais em ronda %d\n", segurancaRonda);
		pthread_cond_wait(&segurancaExpulsando, &m2);
	}
	pthread_mutex_unlock(&m2);


	/* coloca o aluno na festa */
	sem_post(&sem_alunosNaSala);
	/*DEPURACAO sem_getvalue(&sem_alunosNaSala, &i);
	printf("sem: %d\n", i);*/
	time1 = clock() - begin;
	printf("%lf \tAluno %d esta na festa.\n", time1, (int) numeroAluno);

	/* tempo que passa na festa */
	tempoNaFesta = rand() % r;
	usleep(tempoNaFesta * 1000); 


	/* aluno saiu da festa */
	pthread_mutex_lock(&mutex_alunosQueFestaram);
	alunosQueFestaram++;
	sem_wait(&sem_alunosNaSala);
	pthread_mutex_unlock(&mutex_alunosQueFestaram);

	/* verifica se tem mais alunos na sala */
	sem_getvalue(&sem_alunosNaSala, &nAlunosNaSala);
	if(nAlunosNaSala == 0)	
	{
		/* DEPURACAO printf("nao tem mais alunos\n");*/
		pthread_cond_broadcast(&aindaTemAlunos);
	}

	time1 = clock() - begin;
	printf("%lf \tAluno %d vai embora.\n", time1, (int) numeroAluno);
	/* DEPURACAO printf("Aluno %d vai embora, numero total: %d\n", (int) numeroAluno, alunosQueFestaram); */

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
		time1 = clock() - begin;
		printf("%lf \tSeguranca em ronda\n", time1);
		pthread_mutex_unlock(&mutex_segurancaEmRonda);
		
		
		tempoRonda = rand() % s;
		usleep(tempoRonda * 1000); /* tempo da ronda */


		/* seguranca termina a ronda e vai para a porta */
		pthread_mutex_lock(&mutex_segurancaEmRonda);
		segurancaRonda = FALSE;
		time1 = clock() - begin;
		printf("%lf \tSeguranca na porta\n", time1);
		pthread_mutex_unlock(&mutex_segurancaEmRonda);
		
		

		/* TODO

		VErifica se tem os alunos na festa, se tiver expulsa eles */
		pthread_mutex_lock(&m1);
		sem_getvalue(&sem_alunosNaSala, &nAlunosNaSala);
		if(nAlunosNaSala == 0)
		{
			time1 = clock() - begin;
			printf("%lf \tSeguranca inspeciona a sala\n", time1);
		}
		else{
			if(nAlunosNaSala >= p)
			{
				time1 = clock() - begin;
				printf("%lf \tSeguranca expulsa os alunos\n", time1);
				/* Expulsa todos os alunos */
				while(nAlunosNaSala > 0)
				{
					pthread_cond_wait(&aindaTemAlunos, &m1);
					sem_getvalue(&sem_alunosNaSala, &nAlunosNaSala);
				}				
				pthread_cond_broadcast(&segurancaExpulsando);
			}
		}
		pthread_mutex_unlock(&m1);


	}
	time1 = clock() - begin;
	printf("%lf \tTermino da festa\n", time1);
	return(NULL);
}


int main(int argc, char *argv[])
{
	int i, j;
	int intervaloChegadaAluno = 0;
	pthread_t *thread_alunos, thread_seguranca;
	int *tid_alunos, tid_seguranca, *numeroAluno;

	/* inicializa mutexes, conds e semaforos */
	pthread_mutex_init(&mutex_alunosQueFestaram, NULL);
	pthread_mutex_init(&mutex_segurancaEmRonda, NULL);
	pthread_mutex_init(&m1, NULL); pthread_mutex_init(&m2, NULL);
	sem_init(&sem_alunosNaSala, SHARED, 0);
	sem_init(&sem_segurancaRonda, SHARED, 0);
	pthread_cond_init(&aindaTemAlunos, NULL);
	pthread_cond_init(&segurancaExpulsando, NULL);

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
		begin = clock();
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
	


  	return 0;
}

