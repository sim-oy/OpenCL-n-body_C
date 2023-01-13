#ifndef MAIN_H
#define MAIN_H

#include "Program.h"
#include "OpenCL.h"
#include "Environment.h"
#include <SFML/Graphics.h>
#include <SFML/Window.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <time.h>
#include <windows.h>
#include <Cl/cl.h>

#define FRAMES_PER_PRINT 100
#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 500
#define WRITE_TO_FILE 0


void DrawParticlesSerial(float particles[], char windowBuffer[]);
void DrawParticles2(float particles[], char windowBuffer[]);
void DrawParticles(particle particles[], char windowBuffer[]);
void DrawParticles8(particle8 particles[], char windowBuffer[]);
void DrawTrackingCircle(sfRenderWindow* window, float particles[]);

double DoubleArraySum(double array[], int len);

#endif