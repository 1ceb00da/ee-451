#include<stdlib.h>
#include<stdio.h>
#include<time.h>

#define n 1024

__global__ void mul_mat(int *a, int *b, int *c) {
	int myx, myy, i;
	myx = blockIdx.x * blockDim.x + threadIdx.x;
	myy = blockIdx.y * blockDim.y + threadIdx.y;

	int local;

	for (i = 0; i < n; i++)
		local += a[myx+n*i] * b[n*i+myy];

	c[myx*n+myy] = local;
}

int main() {
	int i;

	int *a = (int*)malloc(sizeof(int)*n*n);
	int *b = (int*)malloc(sizeof(int)*n*n);
	int *c = (int*)malloc(sizeof(int)*n*n);
	cudaEvent_t start, stop;
	float time;

	dim3 dimGrid(32,32);
	dim3 dimBlock(32,32);

	for(i=0;i<n*n;i++) {
		a[i] = 1;
		b[i] = 2;
		c[i] = 0;
	}

	int *gpua, *gpub, *gpuc;

	cudaMalloc((void**)&gpua, sizeof(int)*n*n);
	cudaMalloc((void**)&gpub, sizeof(int)*n*n);
	cudaMalloc((void**)&gpuc, sizeof(int)*n*n);

	cudaMemcpy(gpua, a, sizeof(int)*n*n, cudaMemcpyHostToDevice);
	cudaMemcpy(gpub, b, sizeof(int)*n*n, cudaMemcpyHostToDevice);
	

	cudaEventCreate(&start);
	cudaEventCreate(&stop);

	cudaEventRecord(start, 0);	
	mul_mat<<<dimGrid, dimBlock>>> (gpua, gpub, gpuc);
	cudaEventRecord(stop, 0);
	
	cudaEventSynchronize(stop);

	cudaMemcpy(c, gpuc, sizeof(int)*n*n, cudaMemcpyDeviceToHost);
	
	cudaEventElapsedTime(&time, start, stop);
	printf("C[451][451] = %d\n",c[451*1024 + 451]);
	printf("Time - %f\n", time);

	free(a);
	free(b);
	free(c);

	cudaEventDestroy(start);
	cudaEventDestroy(stop);

	cudaFree(gpua);
	cudaFree(gpub);
	cudaFree(gpuc);

	return 0;

}
