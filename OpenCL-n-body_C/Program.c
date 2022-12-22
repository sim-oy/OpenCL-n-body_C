#include "Program.h"
#include "GlobalSettings.h"
#include <stdio.h>
#include <math.h>
#include <immintrin.h>
#include <omp.h>

const float G = 0.000000001f;
const float smoothing = 0.000001f;


void CalculateSingleArray(float particles[]) {
	for (int i = 0; i < N; i++) {
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
	for (int i = 0; i < N; i++) {
		particles[i * 5] += particles[i * 5 + 2];
		particles[i * 5 + 1] += particles[i * 5 + 3];
	}
}


void CalculateSIMD(float px[N_PAR][N / N_PAR], float py[N_PAR][N / N_PAR], float pvx[N_PAR][N / N_PAR], float pvy[N_PAR][N / N_PAR], float pm[N_PAR][N / N_PAR]) {
	__m128 vG = _mm_set_ps1(G);
	__m128 vsmoothing = _mm_set_ps1(smoothing);
	for (int i = 0; i < N_PAR; i++) {
		__m128 sumX = _mm_setzero_ps(), sumY = _mm_setzero_ps();
		for (int j = 0; j < N / N_PAR; j++)
		{
			__m128 distanceX = _mm_sub_ps(_mm_load_ps(px[i]), _mm_load_ps(py[i]));
			__m128 distanceY = _mm_sub_ps(_mm_load_ps(py[i]), _mm_load_ps(py[i]));

			__m128 distanceXY = _mm_mul_ps(distanceX, distanceY);
			__m128 x2_y2 = _mm_add_ps(distanceXY, distanceXY);

			__m128 dist = _mm_sqrt_ps(_mm_mul_ps(_mm_mul_ps(x2_y2, x2_y2), x2_y2));
			
			__m128 b = _mm_div_ps(_mm_mul_ps(vG, _mm_load_ps(pm[i])), _mm_add_ps(dist, vsmoothing));

			sumX = _mm_add_ps(sumX, _mm_mul_ps(distanceX, b));
			sumY = _mm_add_ps(sumX, _mm_mul_ps(distanceY, b));

			ShiftArray(px[i], N);
			ShiftArray(py[i], N);
			ShiftArray(pvx[i], N);
			ShiftArray(pvy[i], N);
			ShiftArray(pm[i], N);
		}
		__m128 new_pvx = _mm_add_ps(_mm_load_ps(pvx[i]), sumX);
		_mm_store_ps(pvx[i], new_pvx);
		__m128 new_pvy = _mm_add_ps(_mm_load_ps(pvy[i]), sumY);
		_mm_store_ps(pvy[i], new_pvy);

		__m128 new_px = _mm_add_ps(_mm_load_ps(px[i]), new_pvx);
		_mm_store_ps(px[i], new_px);
		__m128 new_py = _mm_add_ps(_mm_load_ps(py[i]), new_pvy);
		_mm_store_ps(py[i], new_py);
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