#include <cstdio>
#include <cuda.h>
#include <cuda_device_runtime_api.h>
#include <cuda_runtime_api.h>
#include <curand_globals.h>

#include "./vec_add.cuh"

const int N = 5;

__host__ void vec_add(float *out, float *a, float *b, int N)
{
    vec_add<<<1, N>>>(out, a, b, N);
}


int main()
{
    float *out, *a, *b;
    out = (float *)malloc(sizeof(float) * N);
    a   = (float *)malloc(sizeof(float) * N);
    b   = (float *)malloc(sizeof(float) * N);

    float *d_out, *d_a, *d_b;
    cudaMalloc((void **)&d_out, sizeof(float) * N);
    cudaMalloc((void **)&d_a, sizeof(float) * N);
    cudaMalloc((void **)&d_b, sizeof(float) * N);

    for (int i = 0; i < N; ++i) {
        a[i] = i;
        b[i] = i + 1;
    }

    cudaMemcpy(d_a, a, sizeof(float) * N, cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, b, sizeof(float) * N, cudaMemcpyHostToDevice);



    cudaMemcpy(out, d_out, sizeof(float) * N, cudaMemcpyDeviceToHost);



    for (int i = 0; i < N; ++i) {
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
