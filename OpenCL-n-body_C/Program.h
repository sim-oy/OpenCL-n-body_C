#ifndef PROGRAM_H
#define PROGRAM_H

#include "Environment.h"
#include <stdio.h>
#include <math.h>
#include <immintrin.h>
#include <omp.h>

void CalculateSingleArray2(float particles[], int n, float G, float smthing);
void CalculateSingleArray(particle particles[], int n, float G, float smthing);
void CalculateSingleArray3(particle particles[], int n, float G, float smthing);

void ShiftArray(float arr[], int len);

#endif