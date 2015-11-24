#include "mpi.h"
#include <iostream>                   
#include <time.h>
#include <stdlib.h>

#define Camarero 10

using namespace std;

void Filosofo(int id, int nprocesos){ // Codigo proc. Filosofo
   int izq=(id+1)%nprocesos;
   int der=(id-1+nprocesos)%nprocesos;
   int sentarse;
   int levantarse;

   while(1){
      cout << "\n Filosofo -->("<<id<< ") PENSANDO"<<endl<<flush;
      sleep((rand()%3)+1 );//pensando
      cout << "\n El filosofo -->("<<id<<") solicita sentarse al camarero";
      MPI_Ssend(&sentarse,1, MPI_INT,10, 2, MPI_COMM_WORLD); 
      cout<<"\n Filosofo --> "<<id<< " solicita tenedor izq -->"<< izq <<endl<<flush;
      MPI_Ssend(&izq,1, MPI_INT,izq,0,MPI_COMM_WORLD); // MPI_Ssend( &i, 1, MPI_INT, Buffer, 0, MPI_COMM_WORLD );
      cout << "\n Filosofo --> " << id << " coge el izq tenedor --> " << izq << endl << flush;
      //Solicita tenedor derecho
      cout<<"\n Filosofo --> "<<id<< " solicita tenedor derecha -->"<<der <<endl<<flush;
      MPI_Ssend(&der,1, MPI_INT, der, 0, MPI_COMM_WORLD); 
      cout<<"\n Filosofo "<<id<< " coge tenedor der --> "<<der <<endl<<flush;
      cout<<"\n Filosofo "<<id<< " COMIENDO"<<endl<<flush;
      sleep((rand() % 3)+1); //comiendo
      //suelta el tenedor izquierdo
      cout<<"\n Filosofo "<<id<< " suelta tenedor izq -->"<<izq <<endl<<flush;
      MPI_Ssend(&izq,1, MPI_INT,izq, 1, MPI_COMM_WORLD);
      //suelta el tenedor derecho
      MPI_Ssend(&der,1, MPI_INT, der, 1, MPI_COMM_WORLD); 
      cout <<"\n Filosofo ("<<id<<") suelta tenedor der --> ("<<der <<")"<<endl<<flush;
      cout <<"\n Filosofo ("<<id<<") se levanta de la mesa";
      MPI_Ssend(&levantarse,1, MPI_INT, 10, 3, MPI_COMM_WORLD); 
    }
}



void Tenedor (int id, int nprocesos){
   int buf; MPI_Status status; int Filo;

   while(1){
      // Espera un peticion desde cualquier ﬁlosofo vecino ...
      MPI_Probe(MPI_ANY_SOURCE,MPI_ANY_TAG, MPI_COMM_WORLD,&status); 
      // Recibe la peticion del ﬁlosofo ...
      MPI_Recv(&buf,1,MPI_INT,MPI_ANY_SOURCE,0,MPI_COMM_WORLD,&status);
      cout<<"Proceso tenedor --> ("<<id<<") recibe peticion del filosofo --> "<<status.MPI_SOURCE <<endl<<flush;
      // Espera a que el ﬁlosofo suelte el tenedor...
      MPI_Recv(&buf,1,MPI_INT,MPI_ANY_SOURCE,1,MPI_COMM_WORLD,&status);
      cout<<"Proceso tenedor --> ("<<id<<") recibe la liberacion  del filosofo --> ("<<status.MPI_SOURCE<<")" << endl<<flush;
   }

}

void Camarero_Central (int id, int nprocesos){
   int permiso,size; MPI_Status status; int Filo,flag;
   int contador_mesa=0;

   while(true){
      //Espera una peticion de sentarse a cualquier filosofo..
       if((contador_mesa<4)){
          MPI_Probe(MPI_ANY_SOURCE,2, MPI_COMM_WORLD,&status);
          cout << "\n El camarero --> ("<<id<<")comprueba que la mesa esta vacia --> tamanio mesa ("<< contador_mesa+1 <<") debe ser menor que 5" << endl << flush;
          MPI_Recv(&permiso,1,MPI_INT,MPI_ANY_SOURCE,2,MPI_COMM_WORLD,&status);
          cout << "\n El camarero deja que se siente el filosofo --> (" << status.MPI_SOURCE << ")" << endl<<flush;
          contador_mesa++;
          MPI_Iprobe(MPI_ANY_SOURCE,3,MPI_COMM_WORLD,&flag,&status);
          if(flag>1){
             MPI_Recv(&permiso,1,MPI_INT,MPI_ANY_SOURCE,3,MPI_COMM_WORLD,&status);
             cout << "\n El camarero despide al filosofo --> (" << status.MPI_SOURCE << ")" << endl << flush;
             contador_mesa--;
          }
       }  
       else {
          cout << "\n la mesa esta llena !!";
          MPI_Probe(MPI_ANY_SOURCE,3, MPI_COMM_WORLD,&status);
          MPI_Recv(&permiso,1,MPI_INT,MPI_ANY_SOURCE,3,MPI_COMM_WORLD,&status);
          cout << "\n El camarero despide al filosofo --> (" << status.MPI_SOURCE << ")" << endl << flush;
          contador_mesa--;
      }
   }
}

int main(int argc,char** argv ){
   int rank,size;
   srand(time(0));
   MPI_Init( &argc, &argv );
   MPI_Comm_rank( MPI_COMM_WORLD, &rank );
   MPI_Comm_size( MPI_COMM_WORLD, &size );
   if( size!=11){
      if (rank == 0) 
         cout<<"El numero de procesos debe ser 10"<<endl<<flush;
      MPI_Finalize( ); 
   return 0;
  }

   if (((rank%2) == 0) && (rank<10)) Filosofo(rank,size-1); // Los pares son Filosofos
   else if(((rank%2) != 0) && (rank<10))Tenedor(rank,size-1); // Los impares son Tenedores
   else { 
      Camarero_Central(rank,size);
   }

   MPI_Finalize();
   return 0;
}


