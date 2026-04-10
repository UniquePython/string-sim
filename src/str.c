#include "str.h"

#include <string.h>

void string_init(StringState *s, int N, float damping)
{
    if (!s)
        return;

    s->N = N;
    s->damping = damping;

    s->r = 1.0f;

    memset(s->u_curr, 0, sizeof(s->u_curr));
    memset(s->u_prev, 0, sizeof(s->u_prev));
}

void string_pluck(StringState *s, float position, float amplitude)
{
    int N = s->N;

    // Clamp position to [0, 1]
    if (position < 0.0f)
        position = 0.0f;
    if (position > 1.0f)
        position = 1.0f;

    // Peak index
    int peak = (int)(position * (N - 1));

    // Avoid division by zero
    if (peak == 0)
        peak = 1;
    if (peak == N - 1)
        peak = N - 2;

    // Left side: 0 -> peak (linear rise)
    for (int i = 0; i <= peak; i++)
        s->u_curr[i] = amplitude * ((float)i / peak);

    // Right side: peak -> N-1 (linear fall)
    for (int i = peak; i < N; i++)
        s->u_curr[i] = amplitude * (1.0f - ((float)(i - peak) / (N - 1 - peak)));

    // Set previous state to zero (initial velocity = 0)
    memset(s->u_prev, 0, N * sizeof(float));

    s->u_curr[0] = 0.0f;
    s->u_curr[N - 1] = 0.0f;
}

void string_step(StringState *s)
{
    int N = s->N;
    float r2 = s->r * s->r;

    // Temporary buffer for next state
    float u_next[MAX_NODES];

    // Boundary conditions (fixed ends)
    u_next[0] = 0.0f;
    u_next[N - 1] = 0.0f;

    // Interior update
    for (int i = 1; i < N - 1; i++)
    {
        float laplacian = s->u_curr[i + 1] - 2.0f * s->u_curr[i] + s->u_curr[i - 1];

        float velocity = s->u_curr[i] - s->u_prev[i];

        u_next[i] = 2.0f * s->u_curr[i] - s->u_prev[i] + r2 * laplacian - s->damping * velocity;
    }

    // Shift states: u_prev <- u_curr, u_curr <- u_next
    for (int i = 0; i < N; i++)
    {
        s->u_prev[i] = s->u_curr[i];
        s->u_curr[i] = u_next[i];
    }
}

float string_sample(const StringState *s, float pickup_pos)
{
    int N = s->N;

    // Clamp pickup position to [0, 1]
    if (pickup_pos < 0.0f)
        pickup_pos = 0.0f;
    if (pickup_pos > 1.0f)
        pickup_pos = 1.0f;

    // Convert to fractional index
    float idx = pickup_pos * (N - 1);
    int i = (int)idx;
    float frac = idx - i;

    // Safety: ensure i+1 is valid
    if (i >= N - 1)
        return s->u_curr[N - 1];

    // Linear interpolation
    return s->u_curr[i] * (1.0f - frac) + s->u_curr[i + 1] * frac;
}
