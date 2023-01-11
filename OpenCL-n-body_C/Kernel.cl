
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

typedef struct {
	float x;
	float y;
	float vx;
	float vy;
	float mss;
} particle;


__kernel void Calc(__global particle particles[], float G, float smoothing, int N) {
	int i = get_global_id(0);

	float xi = particles[i].x, yi = particles[i].y;
	float sumX = 0, sumY = 0;
	for (int j = 0; j < N; j++)
	{
		float distanceX = particles[j].x - xi;
		float distanceY = particles[j].y - yi;

		float x2_y2 = distanceX * distanceX + distanceY * distanceY;
		float dist = (float)sqrt(x2_y2 * x2_y2 * x2_y2);

		float b = G * particles[j].mss / (dist + smoothing);

		sumX += distanceX * b;
		sumY += distanceY * b;
	}
	particles[i].vx += sumX;
	particles[i].vy += sumY;
}

__kernel void Move(__global particle particles[], int N) {
	int i = get_global_id(0);
	particles[i].x += particles[i].vx;
	particles[i].y += particles[i].vy;
}