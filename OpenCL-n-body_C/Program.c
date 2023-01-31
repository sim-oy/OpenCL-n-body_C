#include "Program.h"
#include <stdio.h>
#include <math.h>

void CalculateSingleArray(particle* particles, int n, float G, float smthing) {
	for (int i = 0; i < n; i++) {
		float xi = particles->pos[i].x;
		float yi = particles->pos[i].y;
		float sumX = 0, sumY = 0;

		for (int j = 0; j < n; j++)
		{
			float distanceX = particles->pos[j].x - xi;
			float distanceY = particles->pos[j].y - yi;

			float x2_y2 = distanceX * distanceX + distanceY * distanceY;
			float dist = sqrtf(x2_y2 * x2_y2 * x2_y2 + smthing);

			float b = particles->mss[j] / (dist);

			sumX += distanceX * b;
			sumY += distanceY * b;
		}
		particles->vel[i].x += sumX * G;
		particles->vel[i].y += sumY * G;

	}
	for (int i = 0; i < n; i++) {
		particles->pos[i].x += particles->vel[i].x;
		particles->pos[i].y += particles->vel[i].y;
	}
}

/*
void CalculateSingleArray3(particle particles[], int n, float G, float smthing) {
	for (int i = 0; i < n; i++) {
		float xi = particles[i].x, yi = particles[i].y;
		float sumX = 0, sumY = 0;
		for (int j = 0; j < n; j++)
		{
			float distanceX = particles[j].x - xi;
			float distanceY = particles[j].y - yi;

			float dist = sqrtf(distanceX * distanceX + distanceY * distanceY);

			float c = dist + smthing;

			float bx = particles[j].mss * distanceX / sqrtf(c * c * c);
			float by = particles[j].mss * distanceY / sqrtf(c * c * c);

			sumX += bx;
			sumY += by;
		}
		particles[i].vx += sumX * G;
		particles[i].vy += sumY * G;

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
}*/
