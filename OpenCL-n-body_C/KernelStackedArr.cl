
__kernel void Calc(__global float particles[], float G, float smoothing, int N) {
	int i = get_global_id(0);

	float xi = particles[i], yi = particles[i + N];
	float sumX = 0, sumY = 0;
	for (int j = 0; j < N; j++)
	{
		float distanceX = particles[j] - xi;
		float distanceY = particles[j + N] - yi;

		float x2_y2 = distanceX * distanceX + distanceY * distanceY;
		float dist = (float)sqrt(x2_y2 * x2_y2 * x2_y2);

		float b = G * particles[j + N * 4] / (dist + smoothing);

		sumX += distanceX * b;
		sumY += distanceY * b;
	}
	particles[i + N * 2] += sumX;
	particles[i + N * 3] += sumY;
}

__kernel void Move(__global float particles[], int N) {
	int i = get_global_id(0);
	particles[i] += particles[i + N * 2];
	particles[i + N] += particles[i + N * 3];
}