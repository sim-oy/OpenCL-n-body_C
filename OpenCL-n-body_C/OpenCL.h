#ifndef OPENCL_H
#define OPENCL_H

#include "Environment.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CL/cl.h>

void CLInit(particle particles[], int arr_len, float G, float smthing);
void CLRun(particle particles[], int arr_len);
char* RdFstr(char* filename);

void CheckErr(cl_int err, char* msg);
void build_error_callback(cl_program program, void* user_data);
void CheckArgErr(cl_kernel kernel, int arg_indx, cl_int err);
void PrintWorkGroupSizes(cl_device_id device, cl_kernel kernel);


#endif