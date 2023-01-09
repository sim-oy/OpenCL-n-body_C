#include "Program.h"
#include <stdio.h>
#include <math.h>
#include <immintrin.h>
#include <omp.h>


void CalculateSingleArray(float particles[], int n, float G, float smthing) {
	for (int i = 0; i < n; i++) {
		float xi = particles[i * 5], yi = particles[i * 5 + 1];
		float sumX = 0, sumY = 0;
		for (int j = 0; j < n; j++)
		{
			float distanceX = particles[j * 5] - xi;
			float distanceY = particles[j * 5 + 1] - yi;

			float x2_y2 = distanceX * distanceX + distanceY * distanceY;
			float dist = (float)sqrt(x2_y2 * x2_y2 * x2_y2);

			float b = G * particles[j * 5 + 4] / (dist + smthing);

			sumX += distanceX * b;
			sumY += distanceY * b;
		}
		particles[i * 5 + 2] += sumX;
		particles[i * 5 + 3] += sumY;
		
	}
	for (int i = 0; i < n; i++) {
		particles[i * 5] += particles[i * 5 + 2];
		particles[i * 5 + 1] += particles[i * 5 + 3];
	}
}

void ShiftArray(float arr[], int len) {
	float save = arr[len - 1];
	for (int i = len - 1; i > 0; i--)
	{
		arr[i] = arr[i - 1];
	}
	arr[0] = save;
}