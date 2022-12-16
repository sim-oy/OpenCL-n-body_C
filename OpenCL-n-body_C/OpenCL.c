#include "OpenCL.h"
#include "GlobalSettings.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CL/cl.h>


void CLInit() {
	cl_platform_id platform;
	cl_device_id device;
	cl_int err;

	char* sourceName = "Kernel.cl";
	
	char* string = RdFstr(sourceName);

	printf("%s", string);

	err = clGetPlatformIDs(1, &platform, NULL);
	err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);

	cl_context context;
	cl_command_queue queue;

	context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
	queue = clCreateCommandQueue(context, device, 0, &err);


	cl_mem input_buffer, output_buffer;

	input_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(float) * N, NULL, &err);
	output_buffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(float) * N, NULL, &err);

	cl_program program = clCreateProgramWithSource(context, 1, &sourceName, NULL, &err);
	err = clBuildProgram(program, 1, &device, NULL, NULL, NULL);

	cl_kernel kernel;
	kernel = clCreateKernel(program, "calc", &err);
	err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &input_buffer);
	err = clSetKernelArg(kernel, 1, sizeof(cl_mem), &output_buffer);

	size_t global_size = N;
	err = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &global_size, NULL, 0, NULL, NULL);
	err = clFinish(queue);
	void* ptr = 0;
	err = clEnqueueReadBuffer(queue, output_buffer, CL_TRUE, 0, sizeof(float) * N, ptr, 0, NULL, NULL);

	clReleaseMemObject(input_buffer);
	clReleaseMemObject(output_buffer);
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