#ifndef STR_H
#define STR_H

#define MAX_NODES 512

typedef struct
{
    float u_curr[MAX_NODES]; // displacement at time n
    float u_prev[MAX_NODES]; // displacement at time n-1
    int N;                   // node count (sets pitch: f0 = fs / (2*N))
    float r;                 // Courant number (keep ≤ 1, ideally = 1)
    float damping;           // γ·dt coefficient
} StringState;

void string_init(StringState *s, int N, float damping);
void string_pluck(StringState *s, float position, float amplitude);
void string_step(StringState *s);
float string_sample(const StringState *s, float pickup_pos);

#endif