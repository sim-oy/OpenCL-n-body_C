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

void CLInit(float particles[]) {
	cl_platform_id platform;
	cl_device_id device;
	cl_int err;

	char* sourceName = "Kernel.cl";
	
	char* shader = RdFstr(sourceName);

	//printf("%s\n", shader);

	err = clGetPlatformIDs(1, &platform, NULL);
	CheckErr(err, "Error getting platform IDs");

	err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
	CheckErr(err, "Error getting device IDs");

	cl_context context;
	cl_command_queue queue;

	context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
	CheckErr(err, "Error creating context");

	queue = clCreateCommandQueue(context, device, 0, &err);
	CheckErr(err, "Error creating command queue");

	cl_mem pos_buf = clCreateBuffer(context, CL_MEM_READ_WRITE, N * sizeof(float), NULL, &err);
	CheckErr(err, "Error creating buffer");

	cl_program program = clCreateProgramWithSource(context, 1, &shader, NULL, &err);
	CheckErr(err, "Error creating buffer");

	err = clBuildProgram(program, 1, &device, "-cl-std=CL3.0", NULL, NULL);
	if (err != CL_SUCCESS) {
		size_t log_size;
		clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
		char* log = malloc(log_size);
		clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);
		printf("Build errors:\n%s\n", log);
		free(log);
	}
	CheckErr(err, "Error building program");

	cl_kernel kernel;
	kernel = clCreateKernel(program, "Calc", &err);
	CheckErr(err, "Error creating kernel");

	err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &pos_buf);
	CheckArgErr(kernel, 0, err);
	err = clSetKernelArg(kernel, 1, sizeof(float), &G);
	CheckArgErr(kernel, 1, err);
	err = clSetKernelArg(kernel, 2, sizeof(float), &smoothing);
	CheckArgErr(kernel, 2, err);
	err = clSetKernelArg(kernel, 3, sizeof(int), (int*)N);
	CheckArgErr(kernel, 3, err);

	size_t global_size = N;
	err = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &global_size, NULL, 0, NULL, NULL);
	CheckErr(err, "Error executing kernel");
	err = clFinish(queue);
	CheckErr(err, "Error finishing queue");
	void* ptr = 0;
	err = clEnqueueReadBuffer(queue, pos_buf, CL_TRUE, 0, sizeof(float) * N, ptr, 0, NULL, NULL);
	CheckErr(err, "Error reading buffer");

	clReleaseMemObject(pos_buf);
	clReleaseKernel(kernel);
	clReleaseProgram(program);
	clReleaseCommandQueue(queue);
	clReleaseContext(context);

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