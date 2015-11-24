#include "mpi.h"
#include <stdlib.h>
#include <iostream>
#include <time.h>
 
using namespace std;

void Filosofo(int id, int nprocesos){  //Proceso filosofo
	int izq=(id+1)%nprocesos; //entero representativo de un tenedor izquierdo.
	int der=(id-1+nprocesos)%nprocesos; //entero representativo de un tenedor derecho.
	
	while(1){ // ejecutamos nuestro bucle infinito
		if(id == 3){  //Ordenamos que un filósofo cualquiera (3) invierta el orden de coger los cubiertos para no tener interbloqueo
			cout<<"Filosofo "<<id<< " solicita tenedor derecho "<<der <<endl<<flush;
			MPI_Ssend(&id,1,MPI_INT,der,id, MPI_COMM_WORLD);  //todos los mensajes han de ser de envío síncrono.
			cout<<"Filosofo "<<id<< " solicita tenedor izquierdo "<<izq <<endl<<flush;
			MPI_Ssend(&id,1,MPI_INT,izq,id,MPI_COMM_WORLD);
		}
		else{
			//Solicita tenedor izquierdo

			cout<<"Filosofo "<<id<< " solicita tenedor izquierdo "<<izq<<endl<<flush;
			MPI_Ssend(&id,1,MPI_INT,izq,id,MPI_COMM_WORLD );		
				
			//Solicita tenedor derecho

			cout<<"Filosofo "<<id<< " solicita tenedor derecho "<<der<<endl<<flush;
			MPI_Ssend(&id,1,MPI_INT,der,id,MPI_COMM_WORLD);
		}	
		
		cout<<"Filosofo "<<id<< " está C O M I E N D O"<<endl<<flush;
		sleep((rand() % 3)+1); //comiendo
		
		if(id == 0){  //Ordenamos que el filósofo 3 invierta el orden de soltar los cubiertos para no tener interbloqueo
			MPI_Ssend(&id, 1, MPI_INT, der, id, MPI_COMM_WORLD);
			cout<<"Filosofo "<<id<< " suelta tenedor derecho "<<der<<endl<<flush;
			MPI_Ssend(&id, 1, MPI_INT, izq, id, MPI_COMM_WORLD);
			cout<<"Filosofo "<<id<< " suelta tenedor izquierdo "<<izq<<endl<<flush;
		}
		else{	
			//suelta el tenedor izquierdo
			cout<<"Filosofo "<<id<< " suelta tenedor izquierdo "<<izq <<endl<<flush;
			MPI_Ssend(&id, 1,MPI_INT,izq, id , MPI_COMM_WORLD);
		
		
			//suelta el tenedor derecho
			cout<<"Filosofo "<<id<< " suelta tenedor derecho "<<der <<endl<<flush;
			MPI_Ssend (&id, 1, MPI_INT, der, id, MPI_COMM_WORLD);
		
		}
	
		cout<<"Filosofo "<<id<< " está P E N S A N D O"<<endl<<flush;
		sleep((rand()%3)+1 );//pensando
	}	
}

void Tenedor(int id, int nprocesos){// Proceso tenedor
	int buf; 
	MPI_Status status; 
	int Filo;
	while(1){
		// Espera un peticion desde cualquier filosofo vecino ...

		MPI_Recv(&Filo,1,MPI_INT,MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
		
		// Recibe la peticion del filosofo ...
			
		cout<<"Tenedor "<<id<<" recibe peticion de: "<<Filo<<endl<<flush;
		
		// Espera a que el filosofo suelte el tenedor...
		MPI_Recv(&buf,1,MPI_INT,status.MPI_SOURCE,status.MPI_TAG,MPI_COMM_WORLD,&status);
		cout<<"Tenenedor "<<id<<" recibe liberacion de: "<<Filo<<endl<<flush;
	}
}


int main(int argc,char** argv ){
	int rank,size;

	MPI_Init( &argc, &argv );
	MPI_Comm_rank( MPI_COMM_WORLD, &rank ); //Iniciamos MPI.
	MPI_Comm_size( MPI_COMM_WORLD, &size );

	srand(time(0)); //Inicializamos la semilla aleatoria.
	if( size!=10){ //Filtro de entrada para evitar que nuestro programa se quede colgado.
		cout<<"El numero de procesos debe ser 10"<<endl<<flush;
		MPI_Finalize( );
		return 0;
	}
	if ((rank%2) == 0) 
		Filosofo(rank,size); // Los procesos pares son Filosofos
	else 
		Tenedor(rank,size);// Los procesos impares son Tenedores
	MPI_Finalize();
	return 0;
}
