#ifndef MAIN_H
#define MAIN_H

#define randf() ((float)rand()/(float)(RAND_MAX))

#define FRAMES_PER_PRINT 100
#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 500
#define WRITE_TO_FILE 0

void DrawParticlesSerial(float particles[], char windowBuffer[]);
void DrawParticles(float particles[], char windowBuffer[]);
double DoubleArraySum(double array[], int len);
void GenerateParticlesSerial(float particles[]);
void GenerateParticles(float particles[]);
void ParticlesPreset8(float particles[]);

#endif