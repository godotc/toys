#include <CL/cl.h>
#include <CL/cl_platform.h>

#include <array>
#include <cstdio>
#include <cstdlib>
#include <vector>


int main()
{
    cl_uint ret           = 0;
    cl_uint num_platfroms = 0;

    ret = clGetPlatformIDs(0, nullptr, &num_platfroms);
    if (CL_SUCCESS != ret) {
        exit(-1);
    }
    printf("Num platfroms: %d\n", num_platfroms);


    std::vector<cl_platform_id> platfrom(num_platfroms);
    ret = clGetPlatformIDs(num_platfroms, platfrom.data(), nullptr);
    if (CL_SUCCESS != ret) {
        exit(-2);
    }
    for (int i = 0; i < num_platfroms; ++i) {

        cl_platform_info info;
        printf("The platfrom %d: %p\n", i, platfrom[i]);
    }



    cl_device_id *devices     = nullptr;
    cl_uint       num_devices = 0;
}
