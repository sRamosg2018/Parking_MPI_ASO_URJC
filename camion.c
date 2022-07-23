#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <mpi.h>
int main(int argc, char**argv){

int id_proceso; 
int datos[3];
int plaza=-1;
MPI_Status estado;


MPI_Init(&argc, &argv);

MPI_Comm_rank(MPI_COMM_WORLD,&id_proceso);


//Los siguientes datos deben estar correctamente cohesionados con los definidos en parking.c 

datos[0] = id_proceso; //id del proceso
datos[1] = 1; //se pone en 0' en un estado inicial porque en parking se ha definido que 0 es para salir y 1 para entrar
datos[2] = 1; // 0 porque es un coche, en el fichero del camion esto será un 1


MPI_Barrier(MPI_COMM_WORLD);

while(1){

	sleep(2); 
	while(plaza == -1){
	MPI_Send(datos, 3, MPI_INT, 0, 0, MPI_COMM_WORLD);
	MPI_Recv(&plaza, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &estado);
	
	}
	sleep(2); 

	datos[1]=0; //ya que quiere salir

	MPI_Send(datos, 3, MPI_INT, 0, 0, MPI_COMM_WORLD);

	datos[1]=1; //para entrar
	plaza = -1; //entrar al bucle de nuevo
}

MPI_Barrier(MPI_COMM_WORLD);

MPI_Finalize();

}
