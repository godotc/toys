#include<stdio.h>
#include<stdlib.h>


const int N =5;

__global__ void vec_add(float* out, float* a, float* b, int N)
{
	int i = threadIdx.x;
	if(i< N){
		out[i] = a[i] + b[i];
	}
}

int main(){
	float* out, *a ,*b;
	out =  (float*) malloc(sizeof(float)*N);
	a =  (float*) malloc(sizeof(float)*N);
	b =  (float*) malloc(sizeof(float)*N);

	float* d_out, *d_a ,*d_b;
	cudaMalloc((void**)&d_out, sizeof(float) * N);
    cudaMalloc((void**)&d_a, sizeof(float) * N);
    cudaMalloc((void**)&d_b, sizeof(float) * N);

	for(int i =0 ;i< N; ++i){
		a[i] = i;
		b[i]= i+1;
	}

	cudaMemcpy(d_a, a, sizeof(float) * N, cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, b, sizeof(float) * N, cudaMemcpyHostToDevice);


	vec_add<<<1,N>>>(d_out, d_a, d_b, N);

    cudaMemcpy(out, d_out, sizeof(float) * N, cudaMemcpyDeviceToHost);



	for(int i =0 ;i< N; ++i){
		printf("%f ", out[i]);
	}

	cudaFree(d_out);
    cudaFree(d_a);
    cudaFree(d_b);
    free(out);
    free(a);
    free(b);

	return 0;
}
