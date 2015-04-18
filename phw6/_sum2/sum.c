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
	int a[64], b[64], c[64], d[64];
	int size, rank;
	int i;
	int sum;
	int finalsum;
	
	finalsum = 0;
	sum = 0;
	
	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank == 0) {
		read_array_file(a);
	}

	MPI_BCAST(a, 64, MPI_INT, 0, MPI_COMM_WORLD);

	//TODO for loop to sum up
	if (rank == 0) {
		for (i =0; i<16;i++)
			sum += a[i];
	}
	else if (rank == 1) {
		for (i=16;i<32;i++)
			sum += a[i];
	}
	else if (rank == 2) {
		for (i=32;i<48;i++)
			sum += a[i];
	}
	else if (rank == 3) {
		for (i=48;i<64;i++)
			sum += a[i];
	}
	//TODO  MPI_SUM
	MPI_Reduce(sum, finalsum, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	MPI_Finalize();
	
	return 0;	
}
