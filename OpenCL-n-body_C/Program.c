#include "Program.h"


void CalculateSingleArray2(float particles[], int N, float G, float smthing) {
	for (int i = 0; i < N; i++) {
		float xi = particles[i], yi = particles[i + N];
		float sumX = 0, sumY = 0;
		for (int j = 0; j < N; j++)
		{
			float distanceX = particles[j] - xi;
			float distanceY = particles[j + N] - yi;

			float x2_y2 = distanceX * distanceX + distanceY * distanceY;
			float dist = (float)sqrt(x2_y2 * x2_y2 * x2_y2);

			float b = G * particles[j + N * 4] / (dist + smthing);

			sumX += distanceX * b;
			sumY += distanceY * b;
		}
		particles[i + N * 2] += sumX;
		particles[i + N * 3] += sumY;
		
	}
	for (int i = 0; i < N; i++) {
		particles[i] += particles[i + N * 2];
		particles[i + N] += particles[i + N * 3];
	}
}

void CalculateSingleArray(particle particles[], int n, float G, float smthing) {
	for (int i = 0; i < n; i++) {
		float xi = particles[i].x, yi = particles[i].y;
		float sumX = 0, sumY = 0;
		for (int j = 0; j < n; j++)
		{
			float distanceX = particles[j].x - xi;
			float distanceY = particles[j].y - yi;

			float x2_y2 = distanceX * distanceX + distanceY * distanceY;
			float dist = (float)sqrt(x2_y2 * x2_y2 * x2_y2);

			float b = G * particles[j].mss / (dist + smthing);

			sumX += distanceX * b;
			sumY += distanceY * b;
		}
		particles[i].vx += sumX;
		particles[i].vy += sumY;

	}
	for (int i = 0; i < n; i++) {
		particles[i].x += particles[i].vx;
		particles[i].y += particles[i].vy;
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