#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <emmintrin.h>

#define N 40
#define M 30

void getWallTime(double *wct){
	struct timeval tp;//domh tou system
	gettimeofday(&tp,NULL);
	*wct = (double)(tp.tv_sec+(tp.tv_usec/1000000.0));
}

int main(){
	
	float *a, *b;
	int i, j, k;
	double timeStart, timeEnd;


	a = (float*) malloc((N*M)*sizeof(float));
	if(a == NULL){
		printf("Something went wrong when allocating array a!");
		exit(1);
	}
	b = (float*) malloc((N*M)*sizeof(float));
	if(a == NULL){
		printf("Something went wrong when allocating array b!");
		free(a);
		exit(1);
	}

	int counter = 0;
	for(i=0;i<N*M;i++){
		a[i] = i+1;
		b[i] = 0;
	}
	getWallTime(&timeStart);
	for(i=0;i<M;i++){
		if(i > 0 && i < M-1){
			for(j=1;j<N-1;j++){
				b[i*N+j] += (a[(i-1)*N+(j-1)] * 0.5) + (a[(i-1)*N+j] * 0.5) + (a[(i-1)*N+(j+1)] * 0.5);
				b[i*N+j] += (a[i*N+(j-1)] * 0.5) + (a[i*N+j] * 5) + (a[i*N+(j+1)] * 0.5);
				b[i*N+j] += (a[(i+1)*N+(j-1)] * 0.5) + (a[(i+1)*N+j] * 0.5) + (a[(i+1)*N+j+1] * 0.5);
			}	
		}
	}
	getWallTime(&timeEnd);
	/*printf("\nDONE!\n");
	for(i=0;i<M;i++){
		for(j=0;j<N;j++){
			printf("%f ",b[i*N+j]);
		}
		printf("\n");

	}*/


	double mflops;
	mflops = ((timeEnd-timeStart)*1e6);
	printf("Done in %lf \n",mflops);
	return 0;
}
