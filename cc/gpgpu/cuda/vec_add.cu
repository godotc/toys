#include<stdio.h>
#include<stdlib.h>


const int N =5;

__global__ void vec_add(float* out, float* a, float* b, int N)
{
	int i = blockDim.x*blockIdx.x + threadIdx.x;
	if(i< N){
		out[i] = a[i] + b[i];
	}
}

