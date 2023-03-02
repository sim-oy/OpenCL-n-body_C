#ifndef PARTICLES_COUNT
#define PARTICLES_COUNT -1
#endif

typedef struct {
	float2 pos[PARTICLES_COUNT];
	float2 vel[PARTICLES_COUNT];
	float mss[PARTICLES_COUNT];
} particle;

__kernel void Calc(__global particle* particles, float G, float smoothing) {
	unsigned int i = get_global_id(0);

	float2 xi = particles->pos[i];
	float2 sumXY = 0;

	#pragma unroll 256
	for (int j = 0; j < PARTICLES_COUNT; j++)
	{
		float2 distanceXY = particles->pos[j] - xi;

		float x2_y2 = distanceXY.x * distanceXY.x + distanceXY.y * distanceXY.y;

		//float dist = sqrt(x2_y2 * x2_y2 * x2_y2);
		//float b = particles[j].mss / (dist + smoothing);

		float dist = rsqrt(x2_y2 * x2_y2 * x2_y2 + smoothing);
		float b = particles->mss[j] * dist;

		sumXY += distanceXY * b;
	}

	particles->vel[i] += sumXY * G;
}

__kernel void Move(__global particle* particles) {
	unsigned int i = get_global_id(0);
	particles->pos[i] += particles->vel[i];
}
