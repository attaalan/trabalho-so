#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <semaphore.h>
#include <string.h>

#define n_passageiros 10
#define n_veiculos 3

sem_t sem_buffer;
sem_t sem_uber[n_veiculos];
sem_t sem_passageiro_atendido[n_veiculos];
sem_t sem_passageiro_esperando[n_veiculos];

sem_t mutex_escreve, mutex_le;
typedef struct {
    char destino[15];
	char partida[15];
} localizacao;
typedef struct { 
	int id_passageiro, regiao_local;
	char destino_passageiro[15];
}Passageiro;

typedef struct {
	int visor, local_atendi_uber;
} Veiculo;

Passageiro Cliente;
Veiculo Uber;
int atendimento = 1;

void * f_uber(void * v);
void * f_passageiro(void * v);

int main() {
	srand((unsigned) time(NULL));
	pthread_t thr_passageiros[n_passageiros], thr_ubers[n_veiculos];
	int i, id_passageiro[n_passageiros], id_uber[n_veiculos];

	sem_init(&sem_buffer, 0, 5);
	sem_init(&mutex_escreve, 0, 0);
	sem_init(&mutex_le, 0, 1);
	//sem_init(&atendimento, 0, 1);


	for (i = 0; i < n_veiculos; ++i) {		//inicia semaforos
		sem_init(&sem_uber[i], 0, 1);
		sem_init(&sem_passageiro_esperando[i], 0, 0);
		sem_init(&sem_passageiro_atendido[i], 0, 0);
	}

	for (i = 0; i < n_passageiros; ++i) {		//Cria passageiros
		id_passageiro[i] = i%5;
		pthread_create(&thr_passageiros[i], NULL, f_passageiro, (void *) &id_passageiro[i]);
	}

	for (i = 0; i < n_veiculos; ++i) {			//Cria Ubers
		id_uber[i] = i;
		pthread_create(&thr_ubers[i], NULL, f_uber, (void *) &id_uber[i]);	
	}

	for (i = 0; i < n_passageiros; ++i)	{
		pthread_join(thr_passageiros[i], NULL);
	}
	
	return 0;
}

void * f_uber(void *v) {
	srand((unsigned) time(NULL));
	int id = *(int*)v;
	int regiao_atendimento = random()%5;
	Uber.local_atendi_uber = regiao_atendimento;
	
	while(1) {
		sem_wait(&mutex_escreve);
		if (Cliente.regiao_local == regiao_atendimento) {	
			Uber.visor = id;
			atendimento--;
			sem_post(&mutex_le);
			sem_wait(&sem_passageiro_esperando[id]);
			//sleep(random()%16);
			printf("Uber %d terminou sua corrida.\n", id);
			sem_post(&sem_passageiro_atendido[id]);
		} else {
			sem_post(&mutex_le);
		}
	}
	return NULL;
}

void * f_passageiro(void* v) {
	int id = *(int*) v;
	char destino[15];
	char partida[15];
	int meu_uber;
	srand((unsigned) time(NULL));
	int regiao;
	
	switch (id) {
		case 0: strcpy(partida,"Barra");break;
		case 1: strcpy(partida,"Pituba");break;
		case 2: strcpy(partida,"Ondina");break;
		case 3: strcpy(partida,"Ribeira");break;
		case 4: strcpy(partida,"Paralela");break;
	}
	
	do {
		regiao = random()%5;
	} while (regiao == id);
		switch (regiao){
			case 0: strcpy(destino,"Barra");break;
			case 1: strcpy(destino,"Pituba");break;
			case 2: strcpy(destino,"Ondina");break;
			case 3: strcpy(destino,"Ribeira");break;
			case 4: strcpy(destino,"Paralela");break;
		}
	

	sleep(random()%3);
	while (1) {
		if (sem_trywait(&sem_buffer) == 0) {
			do {
				sem_wait(&mutex_le);
				Cliente.id_passageiro = id;
				strcpy(Cliente.destino_passageiro, destino);
				Cliente.regiao_local = random()%5;
				printf("Passageiro da %s, com destino à %s, pediu um Uber.\n", partida, destino);
				sem_post(&mutex_escreve);
				sem_wait(&mutex_le);  //VOlTA PRA CÁ
			} while	(atendimento);
			meu_uber = Uber.visor;
			sem_wait(&sem_uber[meu_uber]);		
			printf("Passageiro da %s entrou no Uber %d.\n", partida, meu_uber);
			sem_post(&sem_passageiro_esperando[meu_uber]);
			sem_post(&sem_buffer);
			sem_wait(&sem_passageiro_atendido[meu_uber]);
			sem_post(&sem_uber[meu_uber]);
			printf("Passageiro da %s chegou à %s.\n", id, destino);
		} else {
			printf("Nenhum Uber disponível para o passageiro da %d.\n", id);
			sleep(30);
		}
	}
						
	return NULL;
}
