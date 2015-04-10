#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<mpi.h>




/*typedef struct MPI_Status_ {
	int MPI_SOURCE;
	int MPI_TAG;
	int MPI_ERROR;
}
*/
int main(int argc, char **argv) {
	int size, rank;
	int d;
	int r;
	int r_tag;
	int firstStep = 1;
	MPI_Status *status;

	d = 451;
	
	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank != 0) {
		MPI_Recv(&r, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		r = r+1;
		printf("Process %d: Msg = %d\n", rank, r);
		d = r;

		//MPI_Send(&d, 1, MPI_INT, 1, d, MPI_COMM_WORLD);
		//printf("Process 0: Initially Msg = %d\n", d);
	}
	else {
		d = 451;
		printf("Process 0 Initially: Msg %d\n", d); 
	}

	r_tag = rank + 10;
	MPI_Send(&d, 1, MPI_INT, (rank+1) % size, 0, MPI_COMM_WORLD);

	if (rank == 0) {
		MPI_Recv(&r, 1, MPI_INT, size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("Process 0: Recieved Msg = %d Done!\n",r);
		//MPI_Recv(&r, 1, MPI_INT, rank - 1, r_tag, MPI_COMM_WORLD, status);
		//r = r + 1;
		//r_tag = r_tag + 10; 
		//printf("Process %d: Msg = %d\n", rank, r);
		//MPI_Send(&r, 1, MPI_INT, rank + 1, r_tag, MPI_COMM_WORLD);
	}
	MPI_Finalize();
}
