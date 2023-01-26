
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
	
	float xi = particles[i].x;
	float yi = particles[i].y;
	float sumX = 0;
	float sumY = 0;
	
	#pragma unroll 256
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


/*
__device__ float3 bodyBodyInteraction(float4 bi, float4 bj, float3 ai) {
	float3 r;   // r_ij  [3 FLOPS]   
	r.x = bj.x - bi.x;
	r.y = bj.y - bi.y;
	r.z = bj.z - bi.z;   // distSqr = dot(r_ij, r_ij) + EPS^2  [6 FLOPS]    
	float distSqr = r.x * r.x + r.y * r.y + r.z * r.z + EPS2;   // invDistCube =1/distSqr^(3/2)  [4 FLOPS (2 mul, 1 sqrt, 1 inv)]    
	float distSixth = distSqr * distSqr * distSqr;
	float invDistCube = 1.0f / sqrtf(distSixth);   // s = m_j * invDistCube [1 FLOP]    
	float s = bj.w * invDistCube;   // a_i =  a_i + s * r_ij [6 FLOPS]   
	ai.x += r.x * s;
	ai.y += r.y * s;
	ai.z += r.z * s;
	return ai;
}

__device__ float3 tile_calculation(float4 myPosition, float3 accel) {
	int i;
	extern __shared__ float4[] shPosition;
	for (i = 0; i < blockDim.x; i++) {
		accel = bodyBodyInteraction(myPosition, shPosition[i], accel);
	}
	return accel;
}
*/