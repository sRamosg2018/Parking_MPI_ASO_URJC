#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <mpi.h>
int main(int argc, char**argv){
	int plazas = atoi(argv[1]);
	int num_plazas = plazas;
	int parking[plazas];
	MPI_Status estado;
	int bloquear = 0;

	for(int i = 0; i<plazas;i++)parking[i]=0;

	printf("Plazas totales del PARKING: %d\n", plazas);
	printf("Parking:");
	for(int i=0; i<plazas; i++){
		printf(" [%d]", parking[i]);
	}
	printf("\n");
	
	//LO QUE DEBE RECIBIR ES: el id del proceso, si se pira o se queda, si es un coche o un camion

	int datos[3];
	

	MPI_Init(&argc, &argv);

	MPI_Barrier(MPI_COMM_WORLD); // se bloquean todos los procesos
	while(1){
		//int MPI_Recv(void *buf, int count, MPI_Datatype datatype,int source, int tag, MPI_Comm comm, MPI_Status *status)
		MPI_Recv(
			datos, //el vector que recibe los datos y los almacena
			 3, //cuantos elementos se van a recibir
			 MPI_INT, //Tipo de dato que se recibe
			 MPI_ANY_SOURCE, //pid del proceso 
			 0, //una etaiqueta (tag), se puede dejar como MPI_ANY_TAG
			 MPI_COMM_WORLD, //comunicador por el que se recibe, como una pipe
			 &estado); //información sobre el estado

			// id proceso = datos[0];
			// accion = datos[1]; //0 salir, 1 entrar
			// vehiculo = datos[2]; //0 coche, 1 camión


			//printf("----------Soy el ID %d\n", datos[0]);

			if(datos[1]==0){ //salir

				if(datos[2]==0){//coche
					for(int i=0; i<plazas; i++){
						if(parking[i]==datos[0]){
							parking[i]=0;

							

							num_plazas++;
							printf("SALIDA: Coche %d saliendo. Plazas libres: %d\n", datos[0], num_plazas);	

						}
						
					}

				}else{ //camión
					for(int i=0; i<plazas; i++){
						if(parking[i]==datos[0] && parking[i+1]==datos[0]){
							parking[i]=0;
							parking[i+1]=0;

							
							num_plazas=num_plazas+2;
							printf("SALIDA: Camión %d saliendo. Plazas libres: %d\n", datos[0], num_plazas);
						}
					}
				}

			}else{//entrar
				if(num_plazas==0){
					printf("PARKING: No hay plazas disponibles\n");
					int error = -1;
					MPI_Send(&error, 1, MPI_INT, datos[0], 0, MPI_COMM_WORLD);

				}else{

					if(datos[2]==0){//coche
						for(int i=0; i<plazas; i++){
							if(parking[i]==0 && bloquear==0){
								parking[i]=datos[0];

								bloquear = 1;

								num_plazas--;
								printf("ENTRADA: Coche %d aparca en  %d. Plazas libres: %d\n", datos[0], i, num_plazas);
								MPI_Send(&i, //lo que se va a enviar
								1, //cuantos elementos se van a enviar
								MPI_INT, //tipo de dato que se envía
								datos[0], //proceso destino
								0, //etiqueta, sin mas
								MPI_COMM_WORLD); // comunicador por el que se manda, como una pipe
							}
							
						}
					}else{ //camión
						for(int i=0; i<plazas; i++){
							if(parking[i]==0 && parking[i+1]==0 &&bloquear==0){
								parking[i]=datos[0];
								parking[i+1]=datos[0];


								bloquear = 1;

								num_plazas=num_plazas-2;
								printf("ENTRADA: Camión %d aparca en %d y %d. Plazas libres: %d\n", datos[0], i,i+1, num_plazas);
								MPI_Send(&i, 1, MPI_INT, datos[0], 0, MPI_COMM_WORLD);

							}
								
						}
					}
				}
			}
			bloquear = 0;
			sleep(1);
			printf("Parking:");
			for(int i=0; i<plazas; i++){
				printf(" [%d]", parking[i]);
			}
			printf("\n");
	}
	MPI_Finalize();	
}

/*Mencionar ayuda de:


https://lsi2.ugr.es/jmantas/ppr/ayuda/mpi_ayuda.php?ayuda=mpi_const_types
https://www.open-mpi.org/

*/