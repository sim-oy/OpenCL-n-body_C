__kernel void Calc(__global float particles[], __global float* G_p, __global float* smoothing_p, __global int* N_p){
	int i = get_global_id(0);

	float G = *G_p;
	float smoothing = *smoothing_p;
	float N = *N_p;

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

	particles[i * 5] += particles[i * 5 + 2];
	particles[i * 5 + 1] += particles[i * 5 + 3];
}