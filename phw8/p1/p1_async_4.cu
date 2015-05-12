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

	int *a;
	int *b;
	int *c;
	cudaEvent_t start, stop;
	float time;

	dim3 dimGrid(64,64);
	dim3 dimBlock(16,16);

    cudaMallocHost((void**)&a, sizeof(int)*n*n);	
	cudaMallocHost((void**)&b, sizeof(int)*n*n);	
	cudaMallocHost((void**)&c, sizeof(int)*n*n);	

	for(i=0;i<n*n;i++) {
		a[i] = 1;
		b[i] = 2;
		c[i] = 0;
	}

	int *gpua, *gpub, *gpuc;

	cudaMalloc((void**)&gpua, sizeof(int)*n*n);
	cudaMalloc((void**)&gpub, sizeof(int)*n*n);
	cudaMalloc((void**)&gpuc, sizeof(int)*n*n);
	
    cudaStream_t stream[4];
	cudaStreamCreate (&stream[0]);
	cudaStreamCreate (&stream[1]);
    cudaStreamCreate (&stream[2]);
	cudaStreamCreate (&stream[3]);

        
    cudaEventCreate(&start);
	cudaEventCreate(&stop);
	
    // start
    cudaEventRecord(start,0);
    
    cudaMemcpyAsync(gpua, a, sizeof(int)*n*n/4, cudaMemcpyHostToDevice, stream[0]);
	cudaMemcpyAsync(gpub, b, sizeof(int)*n*n/4, cudaMemcpyHostToDevice, stream[0]);
	mul_mat<<<dimGrid, dimBlock, 0, stream[0]>>> (gpua, gpub, gpuc);
	cudaMemcpyAsync(c, gpuc, sizeof(int)*n*n/4, cudaMemcpyDeviceToHost, stream[0]);
    
    cudaMemcpyAsync(&gpua[n*n/4], &a[n*n/4], sizeof(int)*n*n/4, cudaMemcpyHostToDevice, stream[1]);
	cudaMemcpyAsync(&gpub[n*n/4], &b[n*n/4], sizeof(int)*n*n/4, cudaMemcpyHostToDevice, stream[1]);
	mul_mat<<<dimGrid, dimBlock, 0, stream[1]>>> (gpua, gpub, gpuc);
	cudaMemcpyAsync(&c[n*n/4], &gpuc[n*n/4], sizeof(int)*n*n/4, cudaMemcpyDeviceToHost, stream[1]);

    cudaMemcpyAsync(&gpua[n*n/2], &a[n*n/2], sizeof(int)*n*n/4, cudaMemcpyHostToDevice, stream[2]);
	cudaMemcpyAsync(&gpub[n*n/2], &b[n*n/2], sizeof(int)*n*n/4, cudaMemcpyHostToDevice, stream[2]);
	mul_mat<<<dimGrid, dimBlock, 0, stream[2]>>> (gpua, gpub, gpuc);
	cudaMemcpyAsync(&c[n*n/2], &gpuc[n*n/2], sizeof(int)*n*n/4, cudaMemcpyDeviceToHost, stream[2]);

    cudaMemcpyAsync(&gpua[3*n*n/4], &a[3*n*n/4], sizeof(int)*n*n/4, cudaMemcpyHostToDevice, stream[2]);
	cudaMemcpyAsync(&gpub[3*n*n/4], &b[3*n*n/4], sizeof(int)*n*n/4, cudaMemcpyHostToDevice, stream[2]);
	mul_mat<<<dimGrid, dimBlock, 0, stream[3]>>> (gpua, gpub, gpuc);
	cudaMemcpyAsync(&c[3*n*n/4], &gpuc[3*n*n/4], sizeof(int)*n*n/4, cudaMemcpyDeviceToHost, stream[2]);

	// stop 
    cudaEventRecord(stop,0);

	cudaStreamSynchronize(stream[0]);
	cudaStreamSynchronize(stream[1]);
	cudaStreamSynchronize(stream[2]);
	cudaStreamSynchronize(stream[3]);

	cudaStreamDestroy(stream[0]);
	cudaStreamDestroy(stream[1]);
	cudaStreamDestroy(stream[2]);
	cudaStreamDestroy(stream[3]);

	cudaEventSynchronize(stop);
	cudaEventElapsedTime(&time, start, stop);


	printf("C[451][451] = %d\n",c[451*1024 + 451]);
	printf("Time - %f\n", time);

	cudaEventDestroy(start);
	cudaEventDestroy(stop);
	
    
    cudaFree(a);
	cudaFree(b);
	cudaFree(c);
    cudaFree(gpua);
	cudaFree(gpub);
	cudaFree(gpuc);

	return 0;

}
