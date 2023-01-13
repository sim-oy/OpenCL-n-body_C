#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>

#pragma pack(4)
typedef struct {
    float x;
    float y;
    float vx;
    float vy;
    float mss;
} particle;
#pragma pack()

typedef struct {
    float x[8];
    float y[8];
    float vx[8];
    float vy[8];
    float mss[8];
} particle8;

#define randf() ((float)rand()/(float)(RAND_MAX))

void GenerateParticles2(float particles[]);
void GenerateParticles(particle particles[]);
void GenerateParticles8(particle8 particles[], int N, int N_PAR);
void ParticlesPreset8(float particles[]);

#endif