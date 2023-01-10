
void Zorder(int ind, int* x, int* y) {
	int len = 0;
	*x = 0, * y = 0;
	while (ind != 0) {
		*x += ((ind % 2) << len);
		ind >>= 1;
		*y += ((ind % 2) << len);
		ind >>= 1;
		len++;
	}
}
/*
__kernel void Calc(__global float particles[], float G, float smoothing, int N) {
	int i = get_global_id(0);
	int x, y;
	Zorder(i, &x, &y);

	float distanceX = particles[j * 5] - xi;
	float distanceY = particles[j * 5 + 1] - yi;

	float x2_y2 = distanceX * distanceX + distanceY * distanceY;
	float dist = (float)sqrt(x2_y2 * x2_y2 * x2_y2);

	float b = G * particles[j * 5 + 4] / (dist + smoothing);

	particles[i * 5 + 2] += distanceX * b;
	particles[i * 5 + 3] += distanceY * b;
}*/


__kernel void Calc(__global float particles[], float G, float smoothing, int N) {
	int i = get_global	_id(0);

	float xi = particles[i * 5], yi = particles[i * 5 + 1];
	float sumX = 0, sumY = 0;
	for (int j = 0; j < N; j++)
	{
		float distanceX = particles[j * 5] - xi;
		float distanceY = particles[j * 5 + 1] - yi;

		float x2_y2 = distanceX * distanceX + distanceY * distanceY;
		float dist = (float)sqrt(x2_y2 * x2_y2 * x2_y2);

		float b = G * particles[j * 5 + 4] / (dist + smoothing);

		sumX += distanceX * b;
		sumY += distanceY * b;
	}
	particles[i * 5 + 2] += sumX;
	particles[i * 5 + 3] += sumY;
}

__kernel void Move(__global float particles[], int N) {
	int i = get_global_id(0);
	particles[i * 5] += particles[i * 5 + 2];
	particles[i * 5 + 1] += particles[i * 5 + 3];
}