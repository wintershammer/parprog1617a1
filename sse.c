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
	
	float *a, *b, *sumA, *sumB;
	double timeStart, timeEnd;

	__m128 *va,*vb,*vSumA,*vSumB;

	int i, j, k;

	i=posix_memalign((void**)&a,16,(N*M)*sizeof(float));
	if(i!=0){
		printf("Wrong");
		exit(1);}
	i=posix_memalign((void**)&b,16,(N*M)*sizeof(float));
	if(i!=0){
		printf("Wrong2");
		free(a);
		exit(1);
	}

	i=posix_memalign((void**)&sumA,16,4*sizeof(float));
	if(i!=0){printf("Wrong3");
		free(a);
		free(b);
		exit(1);
	}

	i=posix_memalign((void**)&sumB,16,4*sizeof(float));
	if(i!=0){printf("Wrong3");
		free(a);
		free(b);
		exit(1);
	}

    srand(time(NULL));

	int counter = 0;
	for(i=0;i<N*M;i++){
		a[i] = rand();
		b[i] = 0;
	}
	for(i=0;i<4;i++){
		sumA[i] = 0;
		sumB[i] = 0;
	}

	const __m128 scalar = _mm_set1_ps(0.5);

	vSumA=(__m128 *)sumA;
	vSumB=(__m128 *)sumB;
	getWallTime(&timeStart);
	for(i=0;i<M;i++){
		va=(__m128 *)a;
		vb=(__m128 *)b;

		va += i * N;
		if(i > 0 && i < M-1){
			for(j=1;j<N-1;j++){
				sumA[0] = a[(i-1)*N+(j-1)];
				sumA[1] = a[(i-1)*N+(j)];
				sumA[2] = a[(i-1)*N+(j+1)];
				sumA[3] = a[(i)*N+(j-1)];
				sumB[0] = a[(i)*N+(j+1)];
				sumB[1] = a[(i+1)*N+(j-1)];
				sumB[2] = a[(i+1)*N+(j)];
				sumB[3] = a[(i+1)*N+(j+1)];
				*vSumA = _mm_add_ps(_mm_mul_ps(*vSumA,scalar),  _mm_mul_ps(*vSumB,scalar)) ;
				const __m128 t = _mm_add_ps(*vSumA, _mm_movehl_ps(*vSumA, *vSumA));
				const __m128 sum = _mm_add_ss(t, _mm_shuffle_ps(t, t, 1));
				b[i*N+j] = sum[0] + a[(i)*N+(j)] * 5;

			}	
		}
	}
	getWallTime(&timeEnd);
	printf("\nDONE!\n");


	double mflops;
	mflops = ((timeEnd-timeStart)*1e6);
	printf("Done for dims %d * %d in %lf \n",N,M,mflops);
	free(a);
	free(b);
	free(sumA);
	free(sumB);
	return 0;
}
