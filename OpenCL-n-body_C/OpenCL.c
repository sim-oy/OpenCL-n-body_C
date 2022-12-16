#include "OpenCL.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CL/cl.h>

void CLInit() {
	char* sourceName = "Kernel.cl";

	FILE* file = NULL;
	fopen_s(&file, sourceName, "r");
	char c;
	char str[100] = "\0";
	while ((c = fgetc(file)) != EOF) {
		strcat_s(str, 100 + 1, &c);
	}
	printf("%s", str);

	fclose(file);

	cl_platform_id platform;
	clGetPlatformIDs(1, &platform, NULL);

	// Get the first available device
	cl_device_id device;
	clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, 1, &device, NULL);

	// Create an OpenCL context
	cl_context context = clCreateContext(NULL, 1, &device, NULL, NULL, NULL);

	// Create a command queue
	cl_command_queue queue = clCreateCommandQueue(context, device, 0, NULL);

	// Clean up
	clReleaseCommandQueue(queue);
	clReleaseContext(context);

}