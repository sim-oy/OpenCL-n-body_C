#ifndef MAIN_H
#define MAIN_H

#define randf() ((float)rand()/(float)(RAND_MAX))

#define FRAMES_PER_PRINT 10
#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 500
#define WRITE_TO_FILE 0

void DrawParticles(float particles[], char windowBuffer[]);
double DoubleArraySum(double array[], int len);
void GenerateParticles(float particles[]);

#endif