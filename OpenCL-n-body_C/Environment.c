#include "Environment.h"


struct particle {
    float x;
    float y;
    float vx;
    float vy;
    float mss;
};



void GenerateParticles2(float particles[], int N) {
    for (int i = 0; i < N; i++) {
        particles[i] = randf();
        particles[i + N] = randf();
        particles[i + N * 2] = 0;
        particles[i + N * 3] = 0;
        particles[i + N * 4] = randf();
    }
}

void GenerateParticles(particle particles[], int N) {
    for (int i = 0; i < N; i++) {
        particles[i].x = randf();
        particles[i].y = randf();
        particles[i].vx = 0;
        particles[i].vy = 0;
        particles[i].mss = randf();
    }
}

void ParticlesPreset8(float particles[], int N) {
    particles[0] = 0.97000436f + 0.5f;
    particles[0 + N * 1] = -0.24308753f + 0.5f;
    particles[0 + N * 2] = 0.93240737f * 2.0f;
    particles[0 + N * 3] = -0.86473146f * 2.0f;
    particles[0 + N * 4] = 1.0f;

    particles[1] = -0.97000436f + 0.5f;
    particles[1 + N * 1] = 0.24308753f + 0.5f;
    particles[1 + N * 2] = 0.93240737f * 2.0f;
    particles[1 + N * 3] = -0.86473146f * 2.0f;
    particles[1 + N * 4] = 1.0f;

    particles[2] = 0.0f + 0.5f;
    particles[2 + N * 1] = 0.0f + 0.5f;
    particles[2 + N * 2] = -0.93240737f;
    particles[2 + N * 3] = 0.86473146f;
    particles[2 + N * 4] = 1.0f;
}