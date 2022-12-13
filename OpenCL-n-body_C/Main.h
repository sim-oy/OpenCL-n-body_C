#ifndef MAIN_H
#define MAIN_H

const static int WINDOW_WIDTH = 500;
const static int WINDOW_HEIGHT = 500;
const static int PARTICLEAMOUNT = 500;

#define randf() static_cast <float> (rand()) / static_cast <float> (RAND_MAX)

void DrawParticles(float* particles);

#endif