#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <CL/cl.h>

#pragma pack(4)
typedef struct {
    float x;
    float y;
    float vx;
    float vy;
    float mss;
} particle2;
#pragma pack()


typedef struct {
    cl_float2 * pos;
    cl_float2 * vel;
    float * mss;
} particle;


#define randf() ((float)rand()/(float)(RAND_MAX))

void GenerateParticles(int N, particle particles);
void GenerateParticles2(particle particles, int N);
void ParticlesPreset8(float particles[]);

#endif