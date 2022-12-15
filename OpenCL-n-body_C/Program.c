#include "Program.h"
#include <stdio.h>
#include <math.h>

const float G = 0.00000001f;

void Calculate(float particles[], int amount) {
	for (int i = 0; i < amount; i++) {
		float xi = particles[i * 5], yi = particles[i * 5 + 1];
		float sumX = 0, sumY = 0;
		for (int j = 0; j < amount; j++)
		{
			float distanceX = particles[j * 5] - xi;
			float distanceY = particles[j * 5 + 1] - yi;

			float x2_y2 = distanceX * distanceX + distanceY * distanceY;
			float dist = sqrt(x2_y2 * x2_y2 * x2_y2);

			float b = G * particles[j * 5 + 4] / (dist + 0.000001f);

			sumX += distanceX * b;
			sumY += distanceY * b;
		}
		particles[i * 5 + 2] += sumX;
		particles[i * 5 + 3] += sumY;
	}
	
	for (int i = 0; i < amount; i++) {
		particles[i * 5] += particles[i * 5 + 2];
		particles[i * 5 + 1] += particles[i * 5 + 3];
	}
}