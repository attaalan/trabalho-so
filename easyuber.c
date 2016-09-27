#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<sys/ipc.h>
#include<semaphore.h>
#include<string.h>

#define Quantidade_passageiro 6
#define Quantidade_veiculos 3

sem_t sem_passageiros; //representa os passageiros
sem_t sem_passageiro[Quantidade_passageiro]; //representa a fila de passageiros
sem_t sem_uber[Quantidade_veiculos]; // representa os uber
sem_t sem_uber_check[Quantidade_veiculos]; // represta se uber atendou cliente até o destino ou nao
sem_t sem_passageiros_Fila[Quantidade_veiculos]; //representa o num de passageiros na fila

sem_t sem_escreve, sem_ler;
int visor, amb=0, viat=0;

void* Uber(void * v){
	int id = *(int*)v;

	while(1) {
		sem_wait(&sem_escreve);  // esperando por passageiro
		visor = id;  //mostra que está livre
		sem_post(&sem_ler); //avisa que esta livre
		sem_wait(&sem_passageiros_Fila[id]); //levando passageiro até o destino
		sleep(random()%3);
		sem_post(&sem_uber_check[id]); //avisa que deixou passageiro destino final
		sleep(random()%3);
	}	
	
}

void * Passageiro(void* v) { //veiculos
	

	while(1){	
	int id = *(int*) v;
	int minha_Fila;
	char tp[15];
	int i,acum=0;
	
	//sleep(random()%3);
	usleep(random()%700+4);
		
		if (id == 0){
			strcpy(tp,"Passageiro Barra");
	
		} else if (id == 1){
			strcpy(tp,"Passageiro Imbui");

				} else if (id == 2){
					strcpy(tp,"passageiro Boca Do Rio");
		
						} else if (id == 3){
							strcpy(tp,"Passageiro Campo Grande");
				
								} else if (id == 4){
									strcpy(tp,"Passageiro Nazaré");
					
										} else if (id == 5){
											strcpy(tp,"Passageiro Paralela");
				

											}

		//printf("%s chegou na fila\n",tp);
		
		for(i=0;i<Quantidade_passageiro;i++){

			//if (sem_trywait(&sem_passageiro[i]) != 0) {
				if(id<=1){ // se passageiro for da barra ou imbui seleciona, #### PRECISA MODIFICAR ESSA CONDIÇÃO ####
					sem_wait(&sem_passageiro[i]);
				//printf("%s chegou na fila\n",tp);
				sem_wait(&sem_ler); //espera indicar que existe ubers livres na região
				minha_Fila = visor;  //mostra quais ubers estão livres na região
				sem_post(&sem_escreve); //solicita o uber
				sem_wait(&sem_uber[minha_Fila]); //espera um uber ser selecionado 
				printf("%s está no uber %d \n",tp,minha_Fila ); 
				sem_post(&sem_passageiros_Fila[minha_Fila]); //avisa que passageiro esta na fila de pedidos
				sem_post(&sem_passageiro[i]); //uber chega e passageiro deixa a fila de pedidos
				//usleep(1000*id);
				sem_wait(&sem_uber_check[minha_Fila]);  //espera uber levar até destino final
				sem_post(&sem_uber[minha_Fila]); //chega ao destino final
				printf("%s chegou ao Destino Final.\n",tp );
			
			
			
				}else{

						sem_wait(&sem_passageiro[i]);
				//printf("%s chegou na fila\n",tp);
				sem_wait(&sem_ler); //espera indicar que existe ubers livres na região
				minha_Fila = visor;  //mostra quais ubers estão livres na região
				sem_post(&sem_escreve); //solicita o uber
				sem_wait(&sem_uber[minha_Fila]); //espera um uber ser selecionado 
				printf("%s está no uber %d \n",tp,minha_Fila ); 
				sem_post(&sem_passageiros_Fila[minha_Fila]); //avisa que passageiro esta na fila de pedidos
				sem_post(&sem_passageiro[i]); //uber chega e passageiro deixa a fila de pedidos
				//usleep(1000*id);
				sem_wait(&sem_uber_check[minha_Fila]);  //espera uber levar até destino final
				sem_post(&sem_uber[minha_Fila]); //chega ao destino final
				printf("%s chegou ao Destino Final.\n",tp );
			
			
				}
				//}
			
	}
}
}
	int main() {
		pthread_t thr_passageiro[Quantidade_passageiro], thr_uber[Quantidade_veiculos];
		int i, id_passageiro[Quantidade_passageiro], id_uber[Quantidade_veiculos];
for(i=0; i<Quantidade_passageiro;i++){
		sem_init(&sem_passageiro[i],0,1);				//fila

}		
		sem_init(&sem_escreve,0,1);				
		sem_init(&sem_ler,0,0);

		for (i = 0;i < Quantidade_veiculos;i++){			//inicia semaforo
			sem_init(&sem_uber[i],0,1);
			sem_init(&sem_passageiros_Fila[i],0,0);
			sem_init(&sem_uber_check[i],0,0);
		}

		for (i=0;i< Quantidade_passageiro;i++) {  //cria passageiros
			id_passageiro[i] = i;
			pthread_create(&thr_passageiro[i], NULL, Passageiro, (void*) &id_passageiro[i]);
		}
		for (i=0;i< Quantidade_veiculos;i++) {  //cria veiculos do uber
			id_uber[i] = i;
			pthread_create(&thr_uber[i], NULL, Uber, (void*) &id_uber[i]);
		}

		for (i=0; i <Quantidade_passageiro; i++)
			pthread_join(thr_passageiro[i],NULL);

	}