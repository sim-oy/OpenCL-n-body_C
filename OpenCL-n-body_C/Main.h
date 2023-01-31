#ifndef MAIN_H
#define MAIN_H

#include "Environment.h"
#include <windows.h>
#include <SFML/Graphics.h>
#include <stdint.h>

#define FRAMES_PER_PRINT 100
#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 500
#define WRITE_TO_FILE 0

#define N2 120000
#define rounding 256
#define N (N2 % rounding == 0 ? N2 : (N2 - N2 % rounding) + rounding)

const float G = 0.000000001f;
const float smthing = 0.00000001f;

void DrawParticles(particle particles, uint8_t windowBuffer[]);
void DrawTrackingCircle(sfRenderWindow* window, particle particles);

double DoubleArraySum(double array[], int len);

#endif