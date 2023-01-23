
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

		//float dist = sqrt(x2_y2 * x2_y2 * x2_y2);
		//float b = particles[j].mss / (dist + smoothing);

		float dist = rsqrt(x2_y2 * x2_y2 * x2_y2 + smoothing);
		float b = particles[j].mss * dist;

		sumX += distanceX * b;
		sumY += distanceY * b;
	}
	particles[i].vx += sumX * G;
	particles[i].vy += sumY * G;
}

__kernel void Move(__global particle particles[], int N) {
	int i = get_global_id(0);
	particles[i].x += particles[i].vx;
	particles[i].y += particles[i].vy;
}