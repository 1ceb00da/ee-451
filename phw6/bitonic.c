#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<mpi.h>


int cmp_fn(const void *a, const void *b) 
{ 
	const int *ia = (const int *)a; // casting pointer types 
	const int *ib = (const int *)b;
	return *ia  - *ib; 
	/* integer comparison: returns negative if b > a 
	 * and positive if a > b */ 
}

int main(int argc, char **argv) {
	
	int a[512];
	int i;
	int size, rank;

	int chunk[128];

	int final[512];

	MPI_Init(NULL,NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank == 0) {
		for (i=0;i<512;i++)
			a[i] = rand() % 2048;
	}

	MPI_Scatter(a,128,MPI_INT, chunk, 128,MPI_INT,0,MPI_COMM_WORLD);
	
	qsort(chunk, 128, sizeof(int), cmp_fn);
	
	MPI_Gather(chunk,128,MPI_INT, final, 128, MPI_INT, 0, MPI_COMM_WORLD);

	if (rank == 0) {
		printf("unsorted - \n");
		for (i=0;i<512;i++)
			printf("%d ", a[i]);
		
		printf("sorted - \n");
                for (i=0;i<512;i++)
			printf("%d ", final[i]);

	}
	MPI_Finalize();
	return 0;

}



