#ifndef GLOBALSETTINGS_H
#define GLOBALSETTINGS_H

#define N_PAR 1
#define N2 10000
#define rounding 256
#define N (N2 % rounding == 0 ? N2 : (N2 - N2 % rounding) + rounding)

extern const float G;
extern const float smoothing;

#endif