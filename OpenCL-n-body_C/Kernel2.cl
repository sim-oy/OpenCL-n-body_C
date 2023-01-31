
inline void AtomicAdd(volatile __global float* source, const float operand) {
	union {
		unsigned int intVal;
		float floatVal;
	} newVal;
	union {
		unsigned int intVal;
		float floatVal;
	} prevVal;
	do {
		prevVal.floatVal = *source;
		newVal.floatVal = prevVal.floatVal + operand;
	} while (atomic_cmpxchg((volatile __global unsigned int*)source, prevVal.intVal, newVal.intVal) != prevVal.intVal);
}

void Zorder(int ind, int* x, int* y) {
	int len = 0;
	*x = 0, *y = 0;
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

__kernel void Calc(__global particle particles[], float G, float smoothing, int N, int block_size) {
	int i = get_global_id(0);
	int k = get_global_id(1);
	barrier(CLK_LOCAL_MEM_FENCE);
	float sumX = 0, sumY = 0;
	for (int j = 0; j < N / (N / block_size); j++)
	{

		float distanceX = particles[j + block_size * k].x - particles[i].x;
		float distanceY = particles[j + block_size * k].y - particles[i].y;

		float x2_y2 = distanceX * distanceX + distanceY * distanceY;

		//float dist = sqrt(x2_y2 * x2_y2 * x2_y2);
		//float b = particles[j].mss / (dist + smoothing);

		float dist = rsqrt(x2_y2 * x2_y2 * x2_y2 + smoothing);
		float b = particles[j + block_size * k].mss * dist;

		sumX += distanceX * b;
		sumY += distanceY * b;
	}
	barrier(CLK_LOCAL_MEM_FENCE);
	particles[i].vx += sumX * G;
	particles[i].vy += sumY * G;
	//particles[i].vx += sumX;
	//particles[i].vy += sumY;
}

__kernel void Move(__global particle particles[], int N) {
	int i = get_global_id(0);
	particles[i].x += particles[i].vx;
	particles[i].y += particles[i].vy;
}