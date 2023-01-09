#ifndef GLOBALSETTINGS_H
#define GLOBALSETTINGS_H

#define N_PAR 1
#define N2 3
#define rounding 1
#define N (N2 % rounding == 0 ? N2 : (N2 - N2 % rounding) + rounding)

extern const float G;
extern const float smoothing;

#endif