#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<mpi.h>

int my_random(int length) {
    return (rand() % length);
}

void swap(int *a, int i, int j) {
    int t;
    
    //printf("i,j %d %d; ai,aj %d %d  \n",i,j,a[i], a[j]);
    t = a[i];
    a[i] = a[j];
    a[j] = t;
    
    
}

int partition(int *array, int left, int right) {
    int pivot;
    int wall;
    int i;
    int randIdx;
    
    randIdx = left + my_random(right-left);
    randIdx = right-1;
    pivot = array[randIdx];
    ///printf("piv=%d;arr= ", pivot);
    
    wall = left;
    
    for (i = left; i < right; i++) {
        if (array[i] < pivot) {
            // swap a[i] & a[wall]
            swap(array, i, wall);
            wall += 1;
        }
    }
    swap(array, wall, randIdx);
    //printArray(array);
    
    
    return wall;
}


void quickSort(int *array, int start, int end){
    // you quick sort function goes here
    int pivotIdx; int x;
    if (start < end) {
        pivotIdx = partition(array, start, end);
        quickSort(array, start, pivotIdx);
        quickSort(array, pivotIdx+1, end);
    }
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
	int os = 512;
	int cs = 128;
	
	int a[os];
	int i;
	int size, rank;

	int chunk[cs];
	
	int from0to1[cs], from0to2[cs];
	int from1to0[cs], from1to3[cs];
	int from2to3[cs], from2to0[cs];
	int from3to2[cs], from3to1[cs];

	int final[os];

	MPI_Init(NULL,NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank == 0) {
		for (i=0;i<os;i++)
			a[i] = rand() % 2048;
	}

	MPI_Scatter(a,cs,MPI_INT, chunk, cs,MPI_INT,0,MPI_COMM_WORLD);
	
//	qsort(chunk, cs, sizeof(int), cmp_fn);
	quickSort(chunk, 0, cs);
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
		
		printf("\nsorted array - \n");
                for (i=0;i<os;i++)
			printf("%d ", final[i]);
		printf("\n");

	}
	MPI_Finalize();
	return 0;

}



