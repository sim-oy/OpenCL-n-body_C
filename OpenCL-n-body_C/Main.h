#ifndef MAIN_H
#define MAIN_H

#define randf() ((float)rand()/(float)(RAND_MAX))

#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 500
#define PARTICLEAMOUNT 3

extern const double G = 0.0000000001;

void DrawParticles(float particles[], char windowBuffer[]);

#endif