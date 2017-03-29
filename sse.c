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
	
	float *a, *b, *sumA, *sumB;
	float c[] __attribute__ ((aligned (16))) = {0.5,0.5,0.5,0.5};
	 __m128 *vc = (__m128*)c;  

	double timeStart, timeEnd;
	float constantOne = 0.5;
	float constantTwo = 5;

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


	int counter = 0;
	for(i=0;i<N*M;i++){
		a[i] = i+1;
		b[i] = 0;
	}
	for(i=0;i<4;i++){
		sumA[i] = 0;
		sumB[i] = 0;
	}
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

				vSumA=(__m128 *)sumA;
				vSumB=(__m128 *)sumB;
				*vSumA = _mm_mul_ps(*vSumA,constantOne);
				*vSumB = _mm_mul_ps(*vSumB,constantTwo);

				float souma = 0;
				for(k=0;k<4;k++){
					souma += sumA[k];
				}
				for(k=0;k<4;k++){
					souma += sumB[k];
				}
				souma += a[i*N+(j)] * 5;
				printf("\n");
				b[i*N+j] += souma;
			}	
		}
	}
	getWallTime(&timeEnd);
	printf("\nDONE!\n");
	double mflops;
	mflops = ((timeEnd-timeStart)*1e6);
	printf("Done in %lf \n",mflops);
	return 0;
}
