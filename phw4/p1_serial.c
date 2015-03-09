#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>

#define		num_of_points	   40000000
typedef struct{
	double x;  
	double y;
}Point; 

int main(void){
	int i,tid;
	int num_of_points_in_circle;
	int chunk;
	
	double pi;
	struct timespec start, stop; 
	double time;
	Point * data_point = (Point *) malloc (sizeof(Point)*num_of_points);
	for(i=0; i<num_of_points; i++){
		data_point[i].x=(double)rand()/(double)RAND_MAX;
		data_point[i].y=(double)rand()/(double)RAND_MAX;
	}
	num_of_points_in_circle=0;
	
	if( clock_gettime(CLOCK_REALTIME, &start) == -1) { perror("clock gettime");}
	
	
	////////**********Use OpenMP to parallize this loop***************//
	omp_set_num_threads(4);
	chunk = num_of_points/4;
	printf("%d\n", chunk);
	#pragma omp parallel shared(data_point, num_of_points_in_circle) private(i,tid)
	{
		#pragma omp for schedule(static, chunk) reduction(+:num_of_points_in_circle)
		for(i=0; i<num_of_points; i++){
		tid = omp_get_thread_num();
		//printf("in thread %d, i=%d, num_of_points=%d, num_of_points_circle=%d\n", tid, num_of_points, num_of_points_in_circle);
			if((data_point[i].x-0.5)*(data_point[i].x-0.5)
			+(data_point[i].y-0.5)*(data_point[i].y-0.5)
			<=0.25) {
				num_of_points_in_circle++;
			}	
		}
	
	}
	///////******************************////
	
	if( clock_gettime( CLOCK_REALTIME, &stop) == -1 ) { perror("clock gettime");}		
	time = (stop.tv_sec - start.tv_sec)+ (double)(stop.tv_nsec - start.tv_nsec)/1e9;
		
	pi =4*(double)num_of_points_in_circle/(double)num_of_points;
	printf("Estimated pi is %f, execution time = %f sec\n",  pi, time);
}	
