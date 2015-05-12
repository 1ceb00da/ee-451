#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <limits.h>

#define TRUE 0
#define FALSE 1


typedef struct {
	
	int src;
	int dst;
	int cost;
} Edge;


__global__ void bellman_ford_kernel(int *dis_arr, Edge *edges, int *change) {
	int my_id;
	my_id = blockIdx.x*blockDim.x + threadIdx.x;

	Edge my_edge = edges[my_id];
	int curr_dis, cand_dis;


	int i;
	// test code
	for (i =0; i < 5; i++) {
		printf("%d->%d: %d\n", edges[i].src, edges[i].dst, edges[i].cost);
	}

	curr_dis = dis_arr[my_edge.dst];
	cand_dis = dis_arr[my_edge.src] + my_edge.cost;

	if (cand_dis < curr_dis) {
		*change = TRUE;
		dis_arr[my_edge.dst] = cand_dis;
	}
}

int main() {
	
	int *dis_arr;
	Edge *edges;

	int n = 5; // num vertices
	int m = 5; // num edges

	dis_arr = (int*)malloc(sizeof(int)*n);
	edges = (Edge*)malloc(sizeof(Edge)*m);

	int i;

	// init edges
	Edge e1 = {0,1, 5};
	Edge e2 = {0,3, 1};
	Edge e3 = {1,2, 3};
	Edge e4 = {3,4, 4};
	Edge e5 = {2,4, 8};
	edges[0] = e1;
	edges[1] = e2;
	edges[2] = e3;
	edges[3] = e4;
	edges[4] = e5;

	// init dis_arr
	// set each val to INT_MAX
	for (i=0; i < n; i++)
		dis_arr[i] = INT_MAX;

	// set source dis
	dis_arr[0] = 0;

	int *gpu_dis_arr;
	Edge *gpu_edges;

	cudaMalloc((void**)&gpu_dis_arr, sizeof(int)*n);
	cudaMalloc((void**)&gpu_edges, sizeof(Edge)*m);

	cudaMemcpy(gpu_dis_arr, dis_arr, sizeof(int)*n, cudaMemcpyHostToDevice);
	cudaMemcpy(gpu_edges, edges, sizeof(Edge)*m, cudaMemcpyHostToDevice);

	int *change;
	int *gpu_change;

	change = (int*)malloc(sizeof(int));
	cudaMalloc((void**)&gpu_change, sizeof(int));

	*change = TRUE;

	while(change == TRUE) {
		//*change = FALSE;
		cudaMemcpy(gpu_change, change, sizeof(int)*1, cudaMemcpyHostToDevice);
		bellman_ford_kernel<<< 1 , 5 >>>(gpu_dis_arr, gpu_edges, gpu_change);
		cudaMemcpy(change, gpu_change, sizeof(int)*1, cudaMemcpyDeviceToHost);
	}

	cudaMemcpy(dis_arr, gpu_dis_arr, sizeof(int)*n, cudaMemcpyDeviceToHost);

	// print dis-arr
	printf("Distance array\n");
	for (i=0; i < n; i++)
		printf("%d ", dis_arr[i]);

}



