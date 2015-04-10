#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<mpi.h>

void read_array_file(int *a) {
	FILE *fp;
	int i;
	fp = fopen("number.txt", "r");
        if(fp == NULL) {
                printf("file open error\n");
        }

        for (i=0; i < 64; i++) {
                fscanf(fp, "%d", &a[i]);
        }

        //for (i =0; i < 64; i++) {
        //        printf("%d\n", a[i]);
        //}
}

int main(int argc, char **argv) {

	FILE *fp;
	int size, rank;
	int i;
	int sum;
	int finalsum;
	int recv[16];
	int rsums[4];
	int a[64];

	finalsum = 0;
	sum = 0;
	
	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank == 0) {
		read_array_file(a);
	}

//	MPI_Bcast(a, 64, MPI_INT, 0, MPI_COMM_WORLD);
	// check if scatter should send 64 or each of 16 to other procs -- this one sends all 64
//	MPI_Scatter(a, 64, MPI_INT, recva, 64, MPI_INT, 0, MPI_COMM_WORLD);
	
	MPI_Scatter(a,16,MPI_INT, recv,16,MPI_INT,0,MPI_COMM_WORLD);
	for (i=0;i<16;i++) {
		sum += recv[i];
	}
	MPI_Gather(&sum, 1, MPI_INT, rsums, 1, MPI_INT, 0, MPI_COMM_WORLD);
	
	if (rank == 0) {
		for (i=0;i<4;i++)
			finalsum += rsums[i];
		printf("FINAL SUM = %d\n", finalsum);
	}
	MPI_Finalize();
	
	return 0;	
}
