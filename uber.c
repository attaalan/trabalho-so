#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <semaphore.h>
#include <string.h>


#define quantidade_passageiro 6
#define quantidade_uber 1



sem_t sem_Veiculos; //representa os veiculos
sem_t sem_Fila[quantidade_passageiro]; //representa a fila de passageiros
sem_t sem_passageiro[quantidade_uber]; // representa numero de passageiros
sem_t sem_passageiro_check[quantidade_uber]; // representa se passageiro solicitou ou não uber
sem_t sem_passageiro_fila[quantidade_uber];
sem_t sem_escreve, sem_ler;
int visor;

void* veiculos(void * v){ // a ideia é cada veiculo ter um id também representando a região onde esta 
	int id = *(int*)v; 

	while(1) { 
		sem_wait(&sem_escreve);  // esperando por passageiros
		visor = id;  // mostra passageiros disponiveis
		sem_post(&sem_ler); //avisa que esta esperando por passageiro
		sem_wait(&sem_passageiro_fila[id]); // pega passageiro
		sleep(random()%3); // sleep de tempo até chegar ao destino
		sem_post(&sem_passageiro_check[id]); //avisa que chegou ao destino
	}	
}

	void * f_passageiros(void* v) { // cada passageiro gera um id aleatorio, cada id representa a região onde o passageiro esta

	while(1){
	int id = *(int*) v;
	int minha_Fila;
	char tp[15];
	int i;
	srand( (unsigned)time(NULL) );
	//usleep(random()%700+4);		
		//sleep(random()%3);
		if (id == 0){
			strcpy(tp,"Passageiro Barra");
			sleep(random()%5);
			//printf("%s chegou passageiro\n",tp);	
		} else if (id == 1){
			strcpy(tp,"Passageiro Campo Grande");
			sleep(random()%5);
			//printf("%s chegou na fila\n",tp);
				} else if (id == 2){
					strcpy(tp,"Passageiro Boca do Rio");
					sleep(random()%5);
					//printf("%s chegou na fila\n",tp);
						} else if (id == 3){
							strcpy(tp,"Passageiro Paralela");
							sleep(random()%5);
							//printf("%s chegou na fila\n",tp);
								} else if (id == 4){
									strcpy(tp,"Passageiro Ondina");
									sleep(random()%5);
									//printf("%s chegou na fila\n",tp);
										} else{ 
											if (id == 5){
												strcpy(tp,"Passageiro Imbui");
												sleep(random()%5);
											//printf("%s chegou na fila\n",tp);
											}
										}	
		// principal parte do código para implementar
		for(i=0;i<=quantidade_passageiro;i++){ 
			if(id==1){ // atende passageiro do campo grande por exemplo
				sem_wait(&sem_ler); 
				minha_Fila = visor;  
				sem_post(&sem_escreve); 
				sem_wait(&sem_passageiro[minha_Fila]); 
				sem_wait(&sem_passageiro_check[minha_Fila]);  
				sem_post(&sem_passageiro[minha_Fila]); 
				printf("%s foi atendido.\n",tp );		
			}
		}
	}
}
	


int main(){
	pthread_t thr_passageiros[quantidade_passageiro];
	int i, id_passageiro[quantidade_passageiro];
	for(i=0; i<quantidade_passageiro;i++){
		sem_init(&sem_Fila[i],0,1);				//fila de passageiros

}
	for (i=0;i< quantidade_passageiro;i++) {  //cria passageiros
			id_passageiro[i] = i;
			pthread_create(&thr_passageiros[i], NULL, f_passageiros, (void*) &id_passageiro[i]);
		}
	
}