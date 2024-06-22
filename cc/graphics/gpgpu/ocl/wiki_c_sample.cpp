
#include <CL/cl.h>
#include <CL/cl_platform.h>
#include <CL/opencl.h>
#include <stddef.h>
#include <sys/types.h>

#define NUM_ENTRIES 1024

int main() {
  const char *kernernal_source = R"(
  // This kernel computes FFT of length 1024. The 1024 length FFT is decomposed into
  // calls to a radix 16 function, another radix 16 function and then a radix 4 function

  __kernel void fft1D_1024 (__global float2 *in, __global float2 *out,
                          __local float *sMemx, __local float *sMemy) {
    int tid = get_local_id(0);
    int blockIdx = get_group_id(0) * 1024 + tid;
    float2 data[16];

    // starting index of data to/from global memory
    in = in + blockIdx;  out = out + blockIdx;

    globalLoads(data, in, 64); // coalesced global reads
    fftRadix16Pass(data);      // in-place radix-16 pass
    twiddleFactorMul(data, tid, 1024, 0);

    // local shuffle using local memory
    localShuffle(data, sMemx, sMemy, tid, (((tid & 15) * 65) + (tid >> 4)));
    fftRadix16Pass(data);               // in-place radix-16 pass
    twiddleFactorMul(data, tid, 64, 4); // twiddle factor multiplication

    localShuffle(data, sMemx, sMemy, tid, (((tid >> 4) * 64) + (tid & 15)));

    // four radix-4 function calls
    fftRadix4Pass(data);      // radix-4 function number 1
    fftRadix4Pass(data + 4);  // radix-4 function number 2
    fftRadix4Pass(data + 8);  // radix-4 function number 3
    fftRadix4Pass(data + 12); // radix-4 function number 4

    // coalesced global writes
    globalStores(data, out, 64);
  }
  )";

  const cl_uint num = 1;
  clGetDeviceIDs(NULL, CL_DEVICE_TYPE_GPU, 0, nullptr, (cl_uint *)&num);

  cl_device_id devices[1];
  clGetDeviceIDs(nullptr, CL_DEVICE_TYPE_GPU, num, devices, nullptr);

  cl_context context = clCreateContextFromType(nullptr, CL_DEVICE_TYPE_GPU,
                                               nullptr, nullptr, nullptr);

  // Create a command quque
  clGetDeviceIDs(nullptr, CL_DEVICE_TYPE_DEFAULT, 1, devices, nullptr);
  cl_int ret;
  cl_command_queue queue =
      clCreateCommandQueueWithProperties(context, devices[0], nullptr, &ret);

  // Memories
  auto buf1 = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                             sizeof(float) * 2 * NUM_ENTRIES, nullptr, nullptr);
  auto buf2 = clCreateBuffer(context, CL_MEM_READ_WRITE,
                             sizeof(float) * 2 * NUM_ENTRIES, nullptr, nullptr);
  cl_mem memory_objs[] = {buf1, buf2};

  // Compute Program
  cl_program program = clCreateProgramWithSource(
      context, 1, (const char **)&kernernal_source, nullptr, nullptr);
  clBuildProgram(program, 0, nullptr, nullptr, nullptr, nullptr);

  // Compute Kernel
  cl_kernel kernel = clCreateKernel(program, "fft1D_1024", nullptr);

  // args values
  size_t local_work_size[1] = {256};

  clSetKernelArg(kernel, 0, sizeof(cl_mem), memory_objs[0]);
  clSetKernelArg(kernel, 1, sizeof(cl_mem), memory_objs[1]);
  clSetKernelArg(kernel, 2, sizeof(float) * (local_work_size[0] + 1) * 16,
                 nullptr);
  clSetKernelArg(kernel, 3, sizeof(float) * (local_work_size[0] + 1) * 16,
                 nullptr);

  // ND ranged object with work-item and execute kernel
  size_t global_work_size[1] = {256};

  global_work_size[0] = NUM_ENTRIES;
  local_work_size[0] = 64;
  clEnqueueNDRangeKernel(queue, kernel, 1, NULL, global_work_size,
                         local_work_size, 0, nullptr, nullptr);
}
