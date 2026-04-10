#include "viz.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

static const Color STRING_COLORS[8] = {
    RED, ORANGE, YELLOW, GREEN, SKYBLUE, BLUE, PURPLE, PINK};

void viz_draw_strings(const StringState *strings, int count)
{
    float width = (float)GetScreenWidth();
    float height = (float)GetScreenHeight();
    float y_center = height * 0.5f;
    float scale = height * 0.025f;

    for (int s = 0; s < count; s++)
    {
        const StringState *str = &strings[s];
        int N = str->N;
        Color col = STRING_COLORS[s % 8];

        for (int i = 0; i < N - 1; i++)
        {
            float x0 = (float)i / (N - 1) * width;
            float x1 = (float)(i + 1) / (N - 1) * width;
            float y0 = y_center - str->u_curr[i] * scale;
            float y1 = y_center - str->u_curr[i + 1] * scale;
            DrawLineV((Vector2){x0, y0}, (Vector2){x1, y1}, col);
        }
    }
}

void viz_draw_gui(float *damping, float *pickup_pos, float *pluck_pos, float *pluck_amp)
{
    // Panel background
    DrawRectangle(10, 10, 260, 180, (Color){0, 0, 0, 180});

    int x = 60;
    int y = 20;
    int w = 190;
    int h = 20;
    int row = 30;

    // Damping
    GuiSliderBar((Rectangle){x, y, w, h}, "Damping", TextFormat("%.5f", *damping), damping, 0.00005f, 0.0006f);
    y += row;

    // Pickup position
    GuiSliderBar((Rectangle){x, y, w, h}, "Pickup", TextFormat("%.5f", *pickup_pos), pickup_pos, 0.01f, 0.99f);
    y += row;

    // Pluck position
    GuiSliderBar((Rectangle){x, y, w, h}, "Pluck Pos", TextFormat("%.5f", *pluck_pos), pluck_pos, 0.01f, 0.99f);
    y += row;

    // Pluck amplitude
    GuiSliderBar((Rectangle){x, y, w, h}, "Pluck Amp", TextFormat("%.5f", *pluck_amp), pluck_amp, 0.1f, 1.0f);
    y += row;
}
