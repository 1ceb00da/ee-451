#include <stdlib.h>
#include <stdio.h>
#include <cublas.h>
#include <time.h>

#define size 65536


__global__ void vector_add(int *a, int *b, int *c){
	int my_id;
	my_id = blockIdx.x*blockDim.x + threadIdx.x;	
	c[my_id] = a[my_id] + b[my_id];  
}

int main(){		
    int i;
    int *a ;
	int *b ;
    int *c ;
	
	cudaMallocHost((void**)&a, sizeof(int)*size);	
	cudaMallocHost((void**)&b, sizeof(int)*size);	
	cudaMallocHost((void**)&c, sizeof(int)*size);	
	
	  for(i=0; i<size; i++){
			a[i]=100;
			b[i]=2;
  	}
		int *gpu_a, *gpu_b, *gpu_c;
		cudaMalloc((void**)&gpu_a, sizeof(int)*size); 
		cudaMalloc((void**)&gpu_b, sizeof(int)*size);
		cudaMalloc((void**)&gpu_c, sizeof(int)*size);
			  
		
		dim3 dimGrid(32);
		dim3 dimBlock(1024);
		
		cudaStream_t stream[2];
		cudaStreamCreate (&stream[0]);
		cudaStreamCreate (&stream[1]);
		
		cudaMemcpyAsync(gpu_a, a, sizeof(int)*size/2, cudaMemcpyHostToDevice, stream[0]);
		cudaMemcpyAsync(gpu_b, b, sizeof(int)*size/2, cudaMemcpyHostToDevice, stream[0]);
		vector_add<<<dimGrid, dimBlock, 0, stream[0]>>>(gpu_a, gpu_b, gpu_c);						
		cudaMemcpyAsync(c, gpu_c, sizeof(int)*size/2, cudaMemcpyDeviceToHost, stream[0]);
		
		cudaMemcpyAsync(&gpu_a[size/2], &a[size/2], sizeof(int)*size/2, cudaMemcpyHostToDevice, stream[1]);
		cudaMemcpyAsync(&gpu_b[size/2], &b[size/2], sizeof(int)*size/2, cudaMemcpyHostToDevice, stream[1]);
		vector_add<<<dimGrid, dimBlock, 0, stream[1]>>>(&gpu_a[size/2], &gpu_b[size/2], &gpu_c[size/2]);						
		cudaMemcpyAsync(&c[size/2], &gpu_c[size/2], sizeof(int)*size/2, cudaMemcpyDeviceToHost, stream[1]);
		
		cudaStreamSynchronize(stream[0]);
		cudaStreamSynchronize(stream[1]);
		cudaStreamDestroy(stream[0]);
		cudaStreamDestroy(stream[1]);
		
		for(i=size/2; i<size/2+12; i++)
			printf("c[%d]=%d ", i, c[i]);
  	
		cudaFree(a);
		cudaFree(b);
		cudaFree(c);
		cudaFree(gpu_a);  
		cudaFree(gpu_b);  
		cudaFree(gpu_c);  
		return 0;
}	
