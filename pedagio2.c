#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<sys/ipc.h>
#include<semaphore.h>
#include<string.h>

#define Tamanho_Fila 6
#define Numero_Cabines 1
#define Tipos_Veiculos 7

sem_t sem_Veiculos; //representa os veiculos
sem_t sem_Fila; //representa a fila de veiculos
sem_t sem_Cabine[Numero_Cabines]; // representa as cabines do pedagio
sem_t sem_Cabine_check[Numero_Cabines]; // represta se carro passou ou nao
sem_t sem_Veiculos_Fila[Numero_Cabines]; //representa o num de carros na fila

sem_t sem_escreve, sem_ler;
int visor, amb=0, viat=0;

void* f_cabine(void * v){
	int id = *(int*)v;

	while(1) {
		sem_wait(&sem_escreve);  // espera veiculo
		visor = id;  //mostra que está livre
		sem_post(&sem_ler); //avisa que esta acordado
		sem_wait(&sem_Veiculos_Fila[id]); //espera veiculo passar
		sleep(random()%3);
		sem_post(&sem_Cabine_check[id]); //avisa que carro passou
		sleep(random()%3);
	}	
	
}

void * f_veiculo(void* v) { //veiculos
	

	while(1){
	int id = *(int*) v;
	int minha_Fila;
	char tp[15];
	int i,acum=0;
	
	//sleep(random()%3);
	usleep(random()%700+4);
	if (sem_trywait(&sem_Fila) == 0) {
		
		if (id == 0){
			strcpy(tp,"AMBULANCIA");
			//printf("%s chegou na fila\n",tp);	
		} else if (id == 1){
			strcpy(tp,"POLICIA");
			//printf("%s chegou na fila\n",tp);
				} else if (id == 2){
					strcpy(tp,"ONIBUS");
					//printf("%s chegou na fila\n",tp);
						} else if (id == 3){
							strcpy(tp,"CAMINHAO");
							//printf("%s chegou na fila\n",tp);
								} else if (id == 4){
									strcpy(tp,"CARRO");
									//printf("%s chegou na fila\n",tp);
										} else if (id == 5){
											strcpy(tp,"MOTO");
											//printf("%s chegou na fila\n",tp);

											}

		//printf("%s chegou na fila\n",tp);
		
		//for(i=0;i<=Tamanho_Fila;i++){
			if(id<=1){
				acum++;
				printf("%s chegou na fila\n",tp);
		sem_wait(&sem_ler); //espera cabine indicar que estar dormindo
		minha_Fila = visor;  //olha qual cabine esta dormindo
		sem_post(&sem_escreve); //aisa que esta indo ser atendido
		sem_wait(&sem_Cabine[minha_Fila]); //espera a cabine acordar
		printf("%s está na cabine %d \n",tp,minha_Fila ); 
		sem_post(&sem_Veiculos_Fila[minha_Fila]); //avisa que veiculo esta na fila
		sem_post(&sem_Fila); //deixa a fila
		usleep(1000*id);
		sem_wait(&sem_Cabine_check[minha_Fila]);  //espera cabine abrir cancela
		sem_post(&sem_Cabine[minha_Fila]); //deixa cabine
		printf("%s foi atendido.\n",tp );
		acum--;

			//}
		}
		if (acum==0){
		printf("%s chegou na fila\n",tp);
		sem_wait(&sem_ler); //espera cabine indicar que estar dormindo
		minha_Fila = visor;  //olha qual cabine esta dormindo
		sem_post(&sem_escreve); //aisa que esta indo ser atendido
		sem_wait(&sem_Cabine[minha_Fila]); //espera a cabine acordar
		printf("%s está na cabine %d \n",tp,minha_Fila ); 
		sem_post(&sem_Veiculos_Fila[minha_Fila]); //avisa que veiculo esta na fila
		sem_post(&sem_Fila); //deixa a fila
		usleep(1000*id);
		sem_wait(&sem_Cabine_check[minha_Fila]);  //espera cabine abrir cancela
		sem_post(&sem_Cabine[minha_Fila]); //deixa cabine
		printf("%s foi atendido.\n",tp );
	}
	} else
		printf("veiculo %d n  tem lugar na fila)\n",id ); //nao tem lugar
	
	}
}

	int main() {
		pthread_t thr_veiculos[Tamanho_Fila], thr_cabines[Numero_Cabines];
		int i, id_vc[Tamanho_Fila], id_cab[Numero_Cabines];

		sem_init(&sem_Fila,0,10);				//fila
		sem_init(&sem_escreve,0,1);				
		sem_init(&sem_ler,0,0);

		for (i = 0;i < Numero_Cabines;i++){			//inicia semaforo
			sem_init(&sem_Cabine[i],0,1);
			sem_init(&sem_Veiculos_Fila[i],0,0);
			sem_init(&sem_Cabine_check[i],0,0);
		}

		for (i=0;i< Tamanho_Fila;i++) {  //cria clientes
			id_vc[i] = i;
			pthread_create(&thr_veiculos[i], NULL, f_veiculo, (void*) &id_vc[i]);
		}
		for (i=0;i< Numero_Cabines;i++) {  //cria cabines
			id_cab[i] = i;
			pthread_create(&thr_cabines[i], NULL, f_cabine, (void*) &id_cab[i]);
		}

		for (i=0; i <Tamanho_Fila; i++)
			pthread_join(thr_veiculos[i],NULL);
	}
	


