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
	int sum[4];
	int rsum1,rsum2,rsum3;
	int ssum1,ssum2,ssum3;

	for (i=0;i<4;i++)
		sum[i] = 0;
	
	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank == 0) {
		read_array_file(a);
		for (i=0; i<16; i++) {
			sum[0] += a[i];
		}
		MPI_Recv(&rsum1, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(&rsum2, 1, MPI_INT, 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(&rsum3, 1, MPI_INT, 3, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		rsum1 += rsum2 + rsum3 + sum[0];
		printf("FINAL_SUM = %d\n", rsum1);
	}

	else if (rank == 1) {
		read_array_file(b);
		for(i=16;i<32;i++) {
			sum[1] += b[i];
		}
		ssum1 = sum[1];	
		MPI_Send(&ssum1, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}
	
	else if (rank == 2) {
		read_array_file(c);
		for (i=32;i<48;i++) {
			sum[2] += c[i];
		}
		ssum2 = sum[2];
		MPI_Send(&ssum2, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}

	else if (rank == 3) {
		read_array_file(d);
		for(i=48;i<64;i++) {
			sum[3] += d[i];
		}
		ssum3 = sum[3];
		MPI_Send(&ssum3, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}

	MPI_Finalize();
	return 0;
	//printf("process %d sum %d\n", rank, sum[rank]);
}
