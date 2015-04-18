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


void keep_min(int local[], int remote[], int size) {
	int i,j,k,temp[size];

	j = 0; 
	k = 0;
	for(i=0;i<size;i++) {
		if (local[j] <= remote[k]) {
			temp[i] = local[j];
			j++;
		}
		else {
			temp[i] = remote[k];
			k++;
		}
	}
	for(i=0;i<size;i++) {
		local[i] = temp[i];
	}

}

void keep_max(int local[], int remote[], int size) {

	int i,j,k,temp[size];
        j = size-1; 
	k = size-1;
	for(i=size-1;i>=0;i--) {
		if (local[j] > remote[k]) {
			temp[i] = local[j];
			j--;
		}
		
		else {
			temp[i] = remote[k];
			k--;
		}
	}
	
	for(i=0;i<size;i++) {
		local[i] = temp[i];
	}

}

int main(int argc, char **argv) {
	
	int a[512];
	int i;
	int size, rank;

	int chunk[128];
	
	int from0to1[128], from0to2[128];
	int from1to0[128], from1to3[128];
	int from2to3[128], from2to0[128];
	int from3to2[128], from3to1[128];

	int final[512];
	int os = 512;
	int cs = 128;

	MPI_Init(NULL,NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank == 0) {
		for (i=0;i<os;i++)
			a[i] = rand() % 2048;
	}

	MPI_Scatter(a,cs,MPI_INT, chunk, cs,MPI_INT,0,MPI_COMM_WORLD);
	
	qsort(chunk, cs, sizeof(int), cmp_fn);

	if (rank == 0) {
		MPI_Send(chunk, cs, MPI_INT, 1, 0, MPI_COMM_WORLD);
		MPI_Recv(from1to0,cs,MPI_INT,1,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		keep_min(chunk,from1to0,cs);

		MPI_Send(chunk, cs, MPI_INT, 2, 0, MPI_COMM_WORLD);
		MPI_Recv(from2to0,cs,MPI_INT,2,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		keep_min(chunk,from2to0,cs);
		
		MPI_Send(chunk, cs, MPI_INT, 1, 0, MPI_COMM_WORLD);
                MPI_Recv(from1to0,cs,MPI_INT,1,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		keep_min(chunk,from1to0,cs);

	}

	else if (rank == 1) {
		MPI_Recv(from0to1, cs, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Send(chunk,cs,MPI_INT,0,0,MPI_COMM_WORLD);
		keep_max(chunk, from0to1, cs);

		// p1p3 comm
		MPI_Send(chunk,cs,MPI_INT,3,0,MPI_COMM_WORLD);
		MPI_Recv(from3to1,cs,MPI_INT,3,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		keep_min(chunk,from3to1,cs);

		MPI_Recv(from0to1,cs,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		MPI_Send(chunk,cs,MPI_INT,0,0,MPI_COMM_WORLD);
		keep_max(chunk,from0to1,cs);
	}

	else if (rank == 2)
	{
		// p2 p3
		MPI_Send(chunk,cs,MPI_INT,3,0,MPI_COMM_WORLD);
		MPI_Recv(from3to2,cs,MPI_INT,3,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		keep_max(chunk,from3to2,3);

		// p0p2
		MPI_Recv(from0to2, cs, MPI_INT, 0,0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Send(chunk,cs,MPI_INT,0,0,MPI_COMM_WORLD);
		keep_max(chunk,from0to2,cs);

		MPI_Send(chunk,cs,MPI_INT,3,0,MPI_COMM_WORLD);
		MPI_Recv(from3to2,cs,MPI_INT,3,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		keep_min(chunk,from3to2,cs);

	}

	else if (rank == 3)
	{
		MPI_Recv(from2to3,cs,MPI_INT,2,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		MPI_Send(chunk,cs,MPI_INT,2,0,MPI_COMM_WORLD);
		keep_min(chunk,from2to3,cs);

		MPI_Recv(from1to3,cs,MPI_INT,1,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		MPI_Send(chunk,cs,MPI_INT,1,0,MPI_COMM_WORLD);
		keep_max(chunk,from1to3,cs); //keepmax(local,fromsomehwerelese,size);
		
		MPI_Recv(from2to3,cs,MPI_INT,2,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		MPI_Send(chunk,cs,MPI_INT,2,0,MPI_COMM_WORLD);
		keep_max(chunk,from2to3,cs);

	}

	MPI_Gather(chunk,cs,MPI_INT, final, cs, MPI_INT, 0, MPI_COMM_WORLD);

	if (rank == 0) {
		printf("unsorted - \n");
		for (i=0;i<os;i++)
			printf("%d ", a[i]);
		
		printf("\nsorted - \n");
                for (i=0;i<os;i++)
			printf("%d ", final[i]);
		printf("\n");

	}
	MPI_Finalize();
	return 0;

}



