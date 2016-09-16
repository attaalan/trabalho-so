#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<sys/ipc.h>
#include<semaphore.h>

#define Tamanho_Fila 10
#define Numero_Cabines 5
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
		if (Tipos_Veiculos == 1){
			usleep(random()1000*1); //passa na cabine
		}else if (Tipos_Veiculos==2){
			usleep(random()1000*2);
		}
		else if (Tipos_Veiculos==3){

		}
		// aqui tem que ter um if (Tipos_Veiculos == 1){} de acordo com o tipo de veiculo o tmepo muda
		usleep(random()1000*1); //passa na cabine
		sempost($sem_Cabine_check[id]) //avisa que carro passou
	}

}

void * f_veiculo(void* v) { //veiculos
	int id = *(int*) v;
	int minha_Fila;

	usleep(random()%700+4);
	if (sem_trywait(&sem_Fila)==0) {
		printf("Veiculo tipo %d chegou na fila");
		sem_wait(&sem_ler); //espera cabine indicar que estar dormindo
		minha_Fila = visor;  //olha qual cabine esta dormindo
		sem_post(&sem_escreve); //aisa que esta indo ser atendido
		sem_wait(&sem_Cabine)[minha_Fila]); //espera a cabine acordar
		printf("(Veiculo %d está na cabine %d \n",id,minha_Fila ); 
		sem_post(&sem_Veiculos_Fila[minha_Fila]); //avisa que veiculo esta na fila
		sem_post(&sem_Fila) //deixa a fila
		sem_wait(&sem_Cabine_check[minha_Fila]);  //espera cabine abrir cancela
		sem_post(&sem_Cabine[minha_Fila]); //deixa cabine
		printf("Veiculo %d foi atendido.\n",id );
	} else
		printf("veiculo %d n  tem lugar na fila)\n",id ); //nao tem lugar
		return NULL;
	}

	int main() {
		pthread_t tht_veiculos[Tamanho_Fila], thr_cabines[Numero_Cabines];
		int i, id_vc[Tamanho_Fila], id_cab[Numero_Cabines];

		sem_init(&sem_Fila,0,10);				//fila
		sem_init(&sem_escreve,0,1);				
		sem_init(&sem_ler,0,0);

		for (i =0;i < Numero_Cabines;i++){			//inicia semaforo
			sem_init(&sem_Cabine[i],0,1);
			sem_init(&sem_Veiculos_Fila[i],0,0);
			sem_init(&sem_Cabine_check[i],0,0)
		}

		for (i=0;i< Tamanho_Fila;i++) {  //cria fila
			id_vc[i] = i;
			pthread_create(&thr_veiculos[i], NULL, f_veiculo, (void*) &id_vc[i]);
		}
		for (i=0;i< Tamanho_Cabines;i++) {  //cria cabines
			id_cab[i] = i;
			pthread_create(&thr_cabines[i], NULL, f_cabine, (void*) &id_vc[i]);
		}

		for (i=0; i < Tamanho_Fila;i++)
			pthread_join(thr_veiculos[i],NULL);
		}
		return 0;
	}


	}

}
