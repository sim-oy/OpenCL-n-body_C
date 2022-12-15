#ifndef PROGRAM_H
#define PROGRAM_H

void CalculateSingleArray(float particles[]);

void CalculateSIMD(float px[], float py[], float pvx[], float pvy[], float pm[]);
void ShiftArray(float arr[], int len);

#endif