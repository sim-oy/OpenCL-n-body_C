#ifndef OPENCL_H
#define OPENCL_H


void CLInit(float particles[], int arr_len, float G, float smthing);
void CLRun(float particles[], int arr_len);
char* RdFstr(char* filename);


#endif