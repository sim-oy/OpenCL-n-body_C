#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    float x;
    float y;
    float vx;
    float vy;
    float mss;
} particle;

#define randf() ((float)rand()/(float)(RAND_MAX))

void GenerateParticles2(float particles[]);
void GenerateParticles(particle particles[]);
void ParticlesPreset8(float particles[]);

#endif