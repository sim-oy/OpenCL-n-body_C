#ifndef MAIN_H
#define MAIN_H

#define randf() ((float)rand()/(float)(RAND_MAX))

#define FRAMES_PER_PRINT 2
#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 500
#define WRITE_TO_FILE 1

#define PARTICLEAMOUNT 6000

void DrawParticles(float particles[], char windowBuffer[]);
double DoubleArraySum(double array[], int len);

#endif