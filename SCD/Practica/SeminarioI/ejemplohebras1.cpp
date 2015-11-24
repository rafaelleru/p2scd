#include <iostream>
#include <pthread.h>
using namespace std;

void* proc1(void* arg){
	for(unsigned long i=0 ; i < 5000 ; i++)
		cout << "Hebra 1, i == " << i << endl;
	return NULL;
}
void* proc2(void* arg){
	for(unsigned long i=0 ; i < 5000 ; i++)
		cout << "Hebra 2, i == " << i << endl;
	return NULL;
}

int main (){
	pthread_t hebra1, hebra2;
	pthread_create(&hebra1,NULL,proc1,NULL);
	pthread_create(&hebra2,NULL,proc2,NULL);
	pthread_exit(NULL);
}
