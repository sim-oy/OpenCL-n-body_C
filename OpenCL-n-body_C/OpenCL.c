#include "OpenCL.h"
#include "Program.h"
#include "GlobalSettings.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CL/cl.h>

void CheckErr(cl_int err, char* msg);
void build_error_callback(cl_program program, void* user_data);
void CheckArgErr(cl_kernel kernel, int arg_indx, cl_int err);

cl_command_queue queue;
cl_kernel kernelCalc;
cl_kernel kernelMove;
cl_mem pos_buf;
cl_int err;


void CLInit(float particles[], int arr_len) {

	char* sourceName = "Kernel.cl";
	char* shader = RdFstr(sourceName);

	//printf("%s\n", shader);

	cl_uint num_platforms;
	clGetPlatformIDs(0, NULL, &num_platforms);
	cl_platform_id* platforms = (cl_platform_id*)malloc(num_platforms * sizeof(cl_platform_id));
	clGetPlatformIDs(num_platforms, platforms, NULL);
	CheckErr(err, "Error getting platform IDs");

	cl_uint num_devices;
	clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_ALL, 0, NULL, &num_devices);
	cl_device_id* devices = (cl_device_id*)malloc(num_devices * sizeof(cl_device_id));
	clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_ALL, num_devices, devices, NULL);
	CheckErr(err, "Error getting device IDs");

	cl_context context = clCreateContext(NULL, num_devices, devices, NULL, NULL, &err);
	CheckErr(err, "Error creating context");

	queue = clCreateCommandQueue(context, devices[0], 0, &err);
	CheckErr(err, "Error creating command queue");

	pos_buf = clCreateBuffer(context, CL_MEM_READ_WRITE, arr_len * sizeof(cl_float), NULL, &err);
	CheckErr(err, "Error creating buffer");

	cl_program program = clCreateProgramWithSource(context, 1, &shader, NULL, &err);
	CheckErr(err, "Error creating program with source");

	err = clBuildProgram(program, 1, &devices[0], "-cl-std=CL3.0", NULL, NULL);
	if (err != CL_SUCCESS) {
		size_t log_size;
		clGetProgramBuildInfo(program, devices[0], CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
		char* log = malloc(log_size);
		clGetProgramBuildInfo(program, devices[0], CL_PROGRAM_BUILD_LOG, log_size, log, NULL);
		printf("Build errors:\n%s\n", log);
		free(log);
		printf("%Error building program: %d\n", err);
		exit(-1);
	}

	kernelCalc = clCreateKernel(program, "Calc", &err);
	CheckErr(err, "Error creating kernel");
	kernelMove = clCreateKernel(program, "Move", &err);
	CheckErr(err, "Error creating kernel");

	size_t preferred_work_group_size;
	err = clGetKernelWorkGroupInfo(kernelCalc, devices[0], CL_KERNEL_WORK_GROUP_SIZE,
		sizeof(preferred_work_group_size), &preferred_work_group_size, NULL);
	CheckErr(err, "Error getting kernel CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE");
	printf("Work group size: %zu\n", preferred_work_group_size);
	err = clGetKernelWorkGroupInfo(kernelCalc, devices[0], CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE,
		sizeof(preferred_work_group_size), &preferred_work_group_size, NULL);
	CheckErr(err, "Error getting kernel CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE");
	printf("Preferred work group size: %zu\n", preferred_work_group_size);
	


	err = clSetKernelArg(kernelCalc, 0, sizeof(cl_mem), &pos_buf);
	CheckArgErr(kernelCalc, 0, err);
	err = clSetKernelArg(kernelCalc, 1, sizeof(cl_float), &G);
	CheckArgErr(kernelCalc, 1, err);
	err = clSetKernelArg(kernelCalc, 2, sizeof(cl_float), &smoothing);
	CheckArgErr(kernelCalc, 2, err);
	int n = N;
	err = clSetKernelArg(kernelCalc, 3, sizeof(cl_int), &n);
	CheckArgErr(kernelCalc, 3, err);

	err = clSetKernelArg(kernelMove, 0, sizeof(cl_mem), &pos_buf);
	CheckArgErr(kernelMove, 0, err);

	clEnqueueWriteBuffer(queue, pos_buf, CL_FALSE, 0, arr_len * sizeof(cl_float), particles, 0, NULL, NULL);

	clReleaseProgram(program);
	clReleaseContext(context);

	printf("CL init done!\n");
}

void CLRun(float particles[], int arr_len) {

	size_t global_size = N;
	size_t local_size = 256;
	err = clEnqueueNDRangeKernel(queue, kernelCalc, 1, NULL, &global_size, &local_size, 0, NULL, NULL);
	CheckErr(err, "Error executing kernel");

	err = clEnqueueNDRangeKernel(queue, kernelMove, 1, NULL, &global_size, NULL, 0, NULL, NULL);
	CheckErr(err, "Error executing kernel");

	err = clEnqueueReadBuffer(queue, pos_buf, CL_FALSE, 0, arr_len * sizeof(cl_float), particles, 0, NULL, NULL);
	CheckErr(err, "Error reading buffer");

	err = clFinish(queue);
	CheckErr(err, "Error finishing queue");
}




char* RdFstr(char* filename) {
	FILE* fp;
	errno_t error = fopen_s(&fp, filename, "r");
	if (error != 0) {
		perror("Error opening file");
		return -1;
	}

	fseek(fp, 0, SEEK_END);
	size_t file_size = ftell(fp);
	rewind(fp);

	char* string = malloc(file_size + 1);
	if (string == NULL) {
		perror("Error allocating memory for string");
		return -1;
	}

	size_t offset = 0;
	while (fgets(string + offset, file_size - offset, fp) != NULL) {
		offset = strlen(string);
	}

	fclose(fp);
	string[file_size] = '\0';

	return string;
}

void CheckErr(cl_int err, char* msg) {
	if (err != CL_SUCCESS) {
		printf("%s: %d\n", msg, err);
		exit(-1);
	}
}

void CheckArgErr(cl_kernel kernel, int arg_indx, cl_int err) {
	if (err != CL_SUCCESS) {
		printf("Error setting argument %d: %d\n", arg_indx, err);

		size_t log_size;
		clGetKernelArgInfo(kernel, arg_indx, CL_KERNEL_ARG_TYPE_NAME, 0, NULL, &log_size);
		char* log_t_name = malloc(log_size);
		clGetKernelArgInfo(kernel, arg_indx, CL_KERNEL_ARG_TYPE_NAME, log_size, log_t_name, NULL);

		clGetKernelArgInfo(kernel, arg_indx, CL_KERNEL_ARG_NAME, 0, NULL, &log_size);
		char* log_name = malloc(log_size);
		clGetKernelArgInfo(kernel, arg_indx, CL_KERNEL_ARG_NAME, log_size, log_name, NULL);

		printf("Argument info:\n%s %s\n", log_t_name, log_name);
		free(log_t_name);
		free(log_name);

		exit(-1);
	}
}

/*
#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>

// OpenCL kernel to perform the calculation
const char* kernel_source =
"__kernel void calculation(__global int* a, __global int* b, __global int* c) {"
"  int i = get_global_id(0);"
"  c[i] = a[i] + b[i] * 2 - 3;"
"}";

int main() {
	// Declare variables
	int a = 5;
	int b = 3;
	int c;

	// Step 1: Initialize the OpenCL environment
	cl_uint num_platforms;
	clGetPlatformIDs(0, NULL, &num_platforms);
	cl_platform_id* platforms = (cl_platform_id*)malloc(num_platforms * sizeof(cl_platform_id));
	clGetPlatformIDs(num_platforms, platforms, NULL);

	cl_uint num_devices;
	clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_ALL, 0, NULL, &num_devices);
	cl_device_id* devices = (cl_device_id*)malloc(num_devices * sizeof(cl_device_id));
	clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_ALL, num_devices, devices, NULL);

	// Step 2: Create a context and command queue
	cl_context context = clCreateContext(NULL, num_devices, devices, NULL, NULL, NULL);
	cl_command_queue queue = clCreateCommandQueue(context, devices[0], 0, NULL);

	// Step 3: Create memory buffers on the device
	cl_int* input1 = (cl_int*)malloc(sizeof(cl_int));
	cl_int* input2 = (cl_int*)malloc(sizeof(cl_int));
	cl_int* output = (cl_int*)malloc(sizeof(cl_int));

	// Step 4: Create and build the OpenCL program
	cl_program program = clCreateProgramWithSource(context, 1, &kernel_source, NULL, NULL);
	clBuildProgram(program, num_devices, devices, NULL, NULL, NULL);

	// Step 5: Create the OpenCL kernel
	cl_kernel kernel = clCreateKernel(program, "calculation", NULL);

	// Step 6: Set the kernel arguments
	clSetKernelArg(kernel, 0, sizeof(cl_mem), &input1);
	clSetKernelArg(kernel, 1, sizeof(cl_mem), &input2);
	clSetKernelArg(kernel, 2, sizeof(cl_mem), &output);

	// Step 7: Write the input data to the device
	clEnqueueWriteBuffer(queue, input1, CL_TRUE, 0, sizeof(cl_int), &a, 0, NULL, NULL);
	clEnqueueWriteBuffer(queue, input2, CL_TRUE, 0, sizeof(cl_int), &b, 0, NULL, NULL);*/