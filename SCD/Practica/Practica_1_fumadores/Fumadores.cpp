#include <iostream>
#include <time.h>     //incluye "time(....)"
#include <unistd.h>   //incluye "sleep(....)" y "usleep(....)"
#include <stdlib.h>   //incluye "rand(...)" y "srand"
#include <semaphore.h> //incluye los semáforos
#include <pthread.h>

pthread_t fumadores[3] ;

int quiero_seguir=0;

sem_t continuar; //éste semáforo va a permitir al estanquero partir de la 2 iteración con el p de los fumadores, seguir creando items
sem_t materiales[3];

pthread_t vendedor;


void fumar()				 //función que simula la acción de fumar
{					 //como un retardo aletaorio de la hebra
	usleep( 100 + (rand() % 1900) ); //retraso aleatorio entre 0'1 y 2 segundos
}

void * material_fumador(void * fumador){   // int fumador facilitado por el int material del void * Estanquero()
   while(true){

        unsigned long carencia = (unsigned long) fumador;

	sem_wait(&(materiales[carencia]));
	sem_post(&continuar);
	quiero_seguir++;
	fumar();
	std::cout << "Está fumando el fumador: " << carencia+1 << std::endl;
	
}
return NULL;
}

void * Estanquero(void *){
      while(true){
	int material=rand()%3;
	switch (material){
		case 0:
			std::cout << "Tengo tabaco para un fumador" << std::endl;
			break;
		case 1:
			std::cout << "Tengo cerillas para un fumador" << std::endl;
			break;
		case 2:
			std::cout << "Tengo papel para un fumador" << std::endl;
			break;
	}

        sem_post(&(materiales[material]));
	sem_wait(&continuar);

      }
	return NULL;

}


int main()
{
	srand (time (NULL) ); //inicializa la semilla aleatoria
	unsigned long fumador;
	
	sem_init(&continuar,0,0);
	for (unsigned long i=0; i<3; i++)
		sem_init(&(materiales[i]),0,0);

	pthread_create( &(vendedor), NULL, Estanquero, NULL);	
	for(unsigned long i = 0 ; i < 3 ; i++ )
		pthread_create( &(fumadores[i]), NULL, material_fumador, (void *)i);

		
	for(unsigned long i = 0 ; i < 3 ; i++ )
		pthread_join( fumadores[i], NULL);

        pthread_join(vendedor, NULL);	


}

