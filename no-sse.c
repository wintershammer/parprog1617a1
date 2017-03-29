#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <emmintrin.h>
#include <time.h>

#define N 4
#define M 3

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
    srand(time(NULL));

	int counter = 0;
	for(i=0;i<N*M;i++){
		a[i] = rand();
		b[i] = 0;
	}
	getWallTime(&timeStart);
	for(i=0;i<M;i++){
		if(i > 0 && i < M-1){
			for(j=1;j<N-1;j++){
				int k = 0;
				int l = 0;
				for(k=0;k<3;k++){
					for(l=0;l<3;l++){
						int firstIndex = (k % 3) ;
						int secondIndex = (l % 3) ;
						
						if(firstIndex == 0 && secondIndex == 0){
							
							b[i*N+j] += a[(firstIndex * N) + (secondIndex)] * 5;
						}
						else{
							b[i*N+j] += a[(firstIndex * N) + (secondIndex)] * 0.5;
						}
					}

				}
			}	
		}
	}
	getWallTime(&timeEnd);
	printf("\nDONE!\n");

	double mflops;
	mflops = ((timeEnd-timeStart)*1e6);
	printf("Done for dims %d * %d in %lf \n",N,M,mflops);
	return 0;
}
