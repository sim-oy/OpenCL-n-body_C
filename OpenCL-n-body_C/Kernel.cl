
typedef struct {
	float2 pos[%d];
	float2 vel[%d];
	float mss[%d];
} particle;

__kernel void Calc(__global particle* particles, float G, float smoothing, int N) {
	int i = get_global_id(0);

	float xi = particles->pos[i].x;
	float yi = particles->pos[i].y;
	float sumX = 0;
	float sumY = 0;

	#pragma unroll 256
	for (int j = 0; j < N; j++)
	{
		float distanceX = particles->pos[j].x - xi;
		float distanceY = particles->pos[j].y - yi;

		float x2_y2 = distanceX * distanceX + distanceY * distanceY;

		//float dist = sqrt(x2_y2 * x2_y2 * x2_y2);
		//float b = particles[j].mss / (dist + smoothing);

		float dist = half_rsqrt(x2_y2 * x2_y2 * x2_y2 + smoothing);
		float b = particles->mss[j] * dist;

		sumX += distanceX * b;
		sumY += distanceY * b;
	}

	particles->vel[i].x += sumX * G;
	particles->vel[i].y += sumY * G;
}

__kernel void Move(__global particle* particles, int N) {
	int i = get_global_id(0);
	particles->pos[i].x += particles->vel[i].x;
	particles->pos[i].y += particles->vel[i].y;
}
