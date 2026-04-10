#include "viz.h"

void viz_draw_string(const StringState *s)
{
    int N = s->N;

    float width = (float)GetScreenWidth();
    float height = (float)GetScreenHeight();

    float y_center = height * 0.5f;
    float scale = height * 0.025f;

    for (int i = 0; i < N - 1; i++)
    {
        float x0 = (float)i / (N - 1) * width;
        float x1 = (float)(i + 1) / (N - 1) * width;

        float y0 = y_center - s->u_curr[i] * scale;
        float y1 = y_center - s->u_curr[i + 1] * scale;

        Vector2 p0 = {x0, y0};
        Vector2 p1 = {x1, y1};

        DrawLineV(p0, p1, WHITE);
    }
}
