#include<stdio.h>
#include<stdlib.h>
#include "vec_add.cuh"


__global__ void vec_add(float* out, float* a, float* b, int N)
{
	int i = blockDim.x*blockIdx.x + threadIdx.x;
	if(i< N){
		out[i] = a[i] + b[i];
	}
}

