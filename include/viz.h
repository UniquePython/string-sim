#ifndef VIZ_H
#define VIZ_H

#include "str.h"

#include "raylib.h"

void viz_draw_string(const StringState *s);
void viz_draw_gui(float *damping, float *pickup_pos, float *pluck_pos, float *pluck_amp, int *n);

#endif