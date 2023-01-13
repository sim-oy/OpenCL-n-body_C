
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

void ShiftArray(float arr[], int len) {
	float save = arr[len - 1];
	for (int i = len - 1; i > 0; i--)
	{
		arr[i] = arr[i - 1];
	}
	arr[0] = save;
}

float8 NullVector() {
	float8 v;
	for (int i = 0; i < 8; i++) {
		v[i] = 0;
	}
	return v;
}

typedef struct {
	float8 x;
	float8 y;
	float8 vx;
	float8 vy;
	float8 mss;
} particle;

/*
__kernel void Calc(__global particle particles[], float G, float smoothing, int N) {
	int i = get_global_id(0);

	float8 sumX = 0, sumY = 0;
	for (int j = 0; j < N; j++)
	{
		for (int k = 0; j < 8; k++) {
			float8 distanceX = particles[j].x - particles[i].x;
			float8 distanceY = particles[j].y - particles[i].y;

			float8 x2_y2 = distanceX * distanceX + distanceY * distanceY;
			float8 dist = sqrt(x2_y2 * x2_y2 * x2_y2);

			float8 b = G * particles[j].mss / (dist + smoothing);

			sumX += distanceX * b;
			sumY += distanceY * b;

			ShiftArray(particles[j].x, N);
			ShiftArray(particles[j].y, N);
			ShiftArray(particles[j].mss, N);
		}
	}
	particles[i].vx += sumX;
	particles[i].vy += sumY;
}*/

__kernel void Calc(__global particle particles[], float G, float smoothing, int N) {
	int i = get_global_id(0);

	for (int k = 0; k < 8; k++) {
		float8 sumX = NullVector(), sumY = NullVector();
		for (int j = 0; j < N; j++)
		{
			float8 distanceX = particles[j].x - particles[i].x[k];
			float8 distanceY = particles[j].y - particles[i].y[k];

			float8 x2_y2 = distanceX * distanceX + distanceY * distanceY;
			float8 dist = sqrt(x2_y2 * x2_y2 * x2_y2);

			float8 b = G * particles[j].mss / (dist + smoothing);

			sumX += distanceX * b;
			sumY += distanceY * b;
		}
		float sumsX = 0, sumsY = 0;
		for (int l = 0; l < 8; l++) {
			sumsX += sumX[l];
			sumsY += sumY[l];
		}

		particles[i].vx[k] += sumsX;
		particles[i].vy[k] += sumsY;
	}
}

__kernel void Move(__global particle particles[], int N) {
	int i = get_global_id(0);

	particles[i].x += particles[i].vx;
	particles[i].y += particles[i].vy;
}