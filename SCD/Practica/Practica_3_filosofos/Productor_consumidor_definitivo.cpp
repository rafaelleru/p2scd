#include "mpi.h"
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include "math.h"

#define Buffer 5
#define Productor  4
#define Consumidor 6
#define ITERS 20
#define TAM 5

using namespace std;

void buffer(){
	int dato[TAM], hilo, pos=0, peticion;
 	MPI_Status status;
 	for (unsigned int i=0;i<ITERS*2;i++){  /*El bucle recorre ITERS*2 dado que tiene que recibir ITERS mensajes de los productores y*/
   		if (pos==0){			/*y debe enviar de forma síncrona ITERS mensajes a los consumidores*/	
			hilo=0;		//El consumidor no puede consumir (estado inicial gracias a pos=0)
			MPI_Probe(MPI_ANY_SOURCE,0, MPI_COMM_WORLD,&status);
		}
   		else if (pos==TAM){ 
			hilo=1;		// El productor no puede producir (buffer lleno, no puede recibir más datos)
			MPI_Probe(MPI_ANY_SOURCE,4, MPI_COMM_WORLD,&status);
		}
   		else{  			//Ambas guardas son ciertas (buffer ni vacío ni lleno, puede hacer ambas cosas)
			MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      			if (status.MPI_SOURCE>=0 && status.MPI_SOURCE<=4) //Si el proceso con el cual estoy entrando en éste condicional 
				hilo=0;                                   // es un consumidor, entonces hilo=0 
			else 
				hilo=1;                                   //sino, entonces es un productor, luego hilo=1
   		}
   		switch(hilo){
     			case 0:
				MPI_Recv(&dato[pos],1,MPI_INT,status.MPI_SOURCE,0,MPI_COMM_WORLD,&status);

       				cout<< "Buffer recibe del Prodcutor "<<flush<< "el valor" << dato[pos];  
       				pos++;
       			break;
     			case 1:
       				MPI_Recv( &peticion, 1, MPI_INT, status.MPI_SOURCE , 4, MPI_COMM_WORLD,&status);

       				MPI_Ssend( &dato[pos-1], 1, MPI_INT, status.MPI_SOURCE, 6 , MPI_COMM_WORLD);
       				cout<< "Buffer envía al Consumidor "<<flush<< " el número: "<< dato[pos-1];  
       				pos--;
       			break;
   		}     
 	}
}   

void productor(int rank){
	int num;  
	for (unsigned int i=rank;i<ITERS;i+=5){ //Itero el bucle de 5 en 5 para que, al tener 5 productores, cada uno genere 4 valores
  		num=i;				// de forma salteada (de 5 en 5, al ser 20 múltiplo de 5)
		cout<< "El Productor " << rank << " produce valor "<< num << endl << flush;
  		sleep(rand() % 2 );
  		MPI_Ssend( &value, 1, MPI_INT, Buffer, 0, MPI_COMM_WORLD ); // El productor unicamente envía mensajes síncronos al buffer
 	}								    // Dichos mensajes son los valores a recibir en el consumidor.
}

void consumidor(int rank){
 	int num,pedir=1; // creo una variable petición para pedir al buffer los valores que van a querer cada uno de nuestros consumidores
        float raiz;
 	MPI_Status status; // creamos una variable MPI para identificar de dónde procederán los mensajes y qué etiqueta tienen.
 	for (unsigned int i=0;i<ITERS/4;i++){ //Itero ahora un total de ITERS/4 veces porque 20/4 es una división entera, 5 mensajes o datos 
  		MPI_Ssend( &pedir, 1 , MPI_INT , Buffer, 4 , MPI_COMM_WORLD );  // que el consumidor va a poder pedir y va a poder recibir
  		MPI_Recv( &num, 1, MPI_INT, Buffer, 6 , MPI_COMM_WORLD,&status );
  		cout<< "El Consumidor " << rank-6 << " va a recibir el número "<<num<<endl<<flush;
  		sleep(rand() % 2 );
  		raiz=sqrt(value);
 	}
}


int main(int argc, char *argv[]) {
  	int rank,size; 
  	cout.flush();
	
  	MPI_Init( &argc, &argv );
  	MPI_Comm_rank( MPI_COMM_WORLD, &rank );  /*Inicializamos MPI*/
  	MPI_Comm_size( MPI_COMM_WORLD, &size );

  	srand ( time(NULL) ); /*Inicializamos la semilla aleatoria*/
  	if (size!=10){     /*Filtro de entrada para evitar que el programa posiblemente se quede colgado*/
		cout<< "Debes introducir exactamente 10 procesos si quieres realizar éste ejercicio."<<endl;
		return 0;
	} 
  	if (rank >= 0 && rank <=4)     /*Si el proceso que estoy utilizando es uno de los 5 primeros...*/
		productor(rank);	/*Entonces ese proceso es un productor*/
   	else if (rank>=6 && rank <=9)  /*Si por el contrario, el proceso es uno de los 4 últimos...*/
		consumidor(rank);	/*Entonces ese proceso es un consumidor*/
    	else 
		buffer(); 		/* Y, si es el proceso intermedio, entonces ese proceso, es nuestro buffer*/
  	MPI_Finalize( );
  
	return 0;
}
