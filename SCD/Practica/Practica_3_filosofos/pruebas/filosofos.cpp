#include "mpi.h"
#include <iostream>
#include <time.h>
#include <stdlib.h>
 
 using namespace std;

void Filosofo(int id, int nprocesos){  //Proceso filosofo
	int izq=(id+1)%nprocesos;
	int der=(id-1+nprocesos)%nprocesos;
	
	while(1){
		if(id == 0){  //Para que no haya interbloqueo el filosofo 0 coge primero el tenedor derecho
			cout<<"Filosofo "<<id<< " solicita tenedor derecha numero "<<der <<endl<<flush;
			MPI_Ssend(&id, 1, MPI_INT, der, id, MPI_COMM_WORLD);
			cout<<"Filosofo "<<id<< " solicita tenedor izquierda numero "<<izq <<endl<<flush;
			MPI_Ssend(&id, 1, MPI_INT, izq, id, MPI_COMM_WORLD);
		}
		else{
			//Solicita tenedor izquierdo
			cout<<"Filosofo "<<id<< " solicita tenedor izquierda numero "<<izq <<endl<<flush;
			MPI_Ssend(&id,1, MPI_INT,izq ,id, MPI_COMM_WORLD );		
				
			//Solicita tenedor derecho
			cout<<"Filosofo "<<id<< " solicita tenedor derecha numero "<<der <<endl<<flush;
			MPI_Ssend(&id, 1,MPI_INT, der,id,MPI_COMM_WORLD);
		}	
		
		cout<<"Filosofo "<<id<< " COMIENDO"<<endl<<flush;
		sleep((rand() % 3)+1); //comiendo
		
		if(id == 0){  //Para que no haya interbloqueo el filosofo 0 coge primero el tenedor derecho
			cout<<"Filosofo "<<id<< " suelta tenedor derecha numero "<<der <<endl<<flush;
			MPI_Ssend(&id, 1, MPI_INT, der, id, MPI_COMM_WORLD);
			cout<<"Filosofo "<<id<< " suelta tenedor izquierda numero "<<izq <<endl<<flush;
			MPI_Ssend(&id, 1, MPI_INT, izq, id, MPI_COMM_WORLD);
		}
		else{	
			//suelta el tenedor izquierdo
			cout<<"Filosofo "<<id<< " suelta tenedor izquierda numero "<<izq <<endl<<flush;
			MPI_Ssend(&id, 1,MPI_INT , izq, id , MPI_COMM_WORLD);
		
		
			//suelta el tenedor derecho
			cout<<"Filosofo "<<id<< " suelta tenedor derecha numero  "<<der <<endl<<flush;
			MPI_Ssend (&id, 1, MPI_INT, der, id, MPI_COMM_WORLD);
		
		}
	
		cout<<"Filosofo "<<id<< " PENSANDO"<<endl<<flush;
		sleep((rand()%3)+1 );//pensando
	}	
}

void Tenedor(int id, int nprocesos){// Proceso tenedor
	int buf; 
	MPI_Status status; 
	int Filo;
	while(1){
		// Espera un peticion desde cualquier filosofo vecino ...
		MPI_Recv(&Filo,1, MPI_INT,MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD, &status);
		
		// Recibe la peticion del filosofo ...
			
		cout<<"Tenedor "<<id<<" recibe peticion de "<<Filo<<endl<<flush;
		
		// Espera a que el filosofo suelte el tenedor...
		MPI_Recv(&buf, 1, MPI_INT,status.MPI_SOURCE, status.MPI_TAG, MPI_COMM_WORLD, &status);
		cout<<"Tenenedor "<<id<<" recibe liberacion de "<<Filo<<endl<<flush;
	}
}


int main(int argc,char** argv ){
	int rank,size;
	srand(time(0));
	MPI_Init( &argc, &argv );
	MPI_Comm_rank( MPI_COMM_WORLD, &rank );
	MPI_Comm_size( MPI_COMM_WORLD, &size );
	if( size!=10){
		if (rank == 0) cout<<"El numero de procesos debe ser 10"<<endl<<flush;
		MPI_Finalize( );
		return 0;
	}
	if ((rank%2) == 0)
		Filosofo(rank,size); // Los pares son Filosofos
	else 
		Tenedor(rank,size);// Los impares son Tenedores
	MPI_Finalize();
	return 0;
}

