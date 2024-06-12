#include <stdio.h>
#include <cuda_runtime.h>

__global__ void increment_kernel(int *g_data, int inc_value)
{
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    g_data[idx] = g_data[idx] + inc_value;
}

bool correct_output(int *data, const int n, const int x)
{
    for (int i = 0; i < n; i++)
        if (data[i] != x)
        {
            printf("Error! data[%d] = %d, ref = %d\n", i, data[i], x);
            return false;
        }

    return true;
}

int main()
{
    int n = 16 * 1024 * 1024;
    int nbytes = n * sizeof(int);
    int value = 26;

    // allocate host memory
    int *a = 0;
    cudaMallocHost((void **)&a, nbytes);
    memset(a, 0, nbytes);

    // allocate device memory
    int *d_a=0;
    cudaMalloc((void**)&d_a, nbytes);
    cudaMemset(d_a, 0, nbytes);

    // set kernel launch configuration
    dim3 threads = dim3(512, 1);
    dim3 blocks = dim3(n/threads.x, 1);

    // issue work to the GPU
    cudaMemcpy(d_a,a,nbytes,cudaMemcpyHostToDevice);
    increment_kernel<<<blocks, threads, 0, 0>>>(d_a, value);
    cudaMemcpy(a, d_a, nbytes, cudaMemcpyDeviceToHost);

    // check the output for correctness
    bool bFinalResults = correct_output(a, n, value);

    // release resources
    cudaFreeHost(a);
    cudaFree(d_a);

    exit(0);
}