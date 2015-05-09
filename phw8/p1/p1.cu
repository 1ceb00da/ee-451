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

	dim3 dimGrid(64,64);
	dim3 dimBlock(16,16);

	for(i=0;i<n*n;i++) {
		a[i] = 1;
		b[i] = 2;
		c[i] = 0;
	}

	int *gpua, *gpub, *gpuc;

	cudaMalloc((void**)&gpua, sizeof(int)*n*n);
	cudaMalloc((void**)&gpub, sizeof(int)*n*n);
	cudaMalloc((void**)&gpuc, sizeof(int)*n*n);

	cudaStream_t stream[2];
	cudaStreamCreate(&stream[0]);
	cudaStreamCreate(&stream[1]);

	cudaMemcpyAsync(gpua, a, sizeof(int)*n*n, cudaMemcpyHostToDevice, stream[0]);
	cudaMemcpyAsync(gpub, b, sizeof(int)*n*n, cudaMemcpyHostToDevice, stream[0]);
	//cudaMemcpy(gpua, a, sizeof(int)*n*n, cudaMemcpyHostToDevice);
	//cudaMemcpy(gpub, b, sizeof(int)*n*n, cudaMemcpyHostToDevice);
	
	cudaEventCreate(&start);
	cudaEventCreate(&stop);

	cudaEventRecord(start,0);
	mul_mat<<<dimGrid, dimBlock, 0, stream[0]>>> (gpua, gpub, gpuc);
	cudaEventRecord(stop,0);

	cudaEventSynchronize(stop);
	cudaEventElapsedTime(&time, start, stop);
	
	cudaMemcpyAsync(c, gpuc, sizeof(int)*n*n, cudaMemcpyDeviceToHost, stream[0]);
	//cudaMemcpy(c, gpuc, sizeof(int)*n*n, cudaMemcpyDeviceToHost);

	cudaStreamSynchronize(stream[0]);
	cudaStreamDestroy(stream[0]);

	printf("C[451][451] = %d\n",c[451*1024 + 451]);
	printf("Time - %f\n", time);

	cudaEventDestroy(start);
	cudaEventDestroy(stop);
	free(a);
	free(b);
	free(c);

	cudaFree(gpua);
	cudaFree(gpub);
	cudaFree(gpuc);

	return 0;

}
