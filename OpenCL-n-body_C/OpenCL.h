#ifndef OPENCL_H
#define OPENCL_H

#include "Environment.h"
#include <CL/cl.h>

#define KERNEL_BUILD_ARGS "-cl-std=CL3.0 -D PARTICLES_COUNT=%d"

void CLInit(particle* particles, int arr_len, float G, float smthing);
void CLRun(particle* particles, int arr_len, int round_size);
char* RdFstr(char* filename);
char* Preprocstr(char* str, int n);

void CheckErr(cl_int err, char* msg);
void build_error_callback(cl_program program, void* user_data);
void CheckArgErr(cl_kernel kernel, int arg_indx, cl_int err);
void PrintWorkGroupSizes(cl_device_id device, cl_kernel kernel);


#endif