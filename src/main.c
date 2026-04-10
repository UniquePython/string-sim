#include "raylib.h"
#include "str.h"
#include "audio.h"
#include "viz.h"

#define WIDTH 1080
#define HEIGHT 720

float damping = 0.0002f;
float pickup_pos = 0.12f;
float pluck_pos = 0.2f;
float pluck_amp = 0.8f;
int n = 80;

int main(void)
{
    InitWindow(WIDTH, HEIGHT, "String Simulation");
    InitAudioDevice();
    SetTargetFPS(60);

    StringState string;
    string_init(&string, n, damping);

    AudioContext audio;
    audio_init(&audio, &string, pickup_pos);

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_SPACE))
            string_pluck(&string, pluck_pos, pluck_amp);

        string.damping = damping;
        audio.pickup_pos = pickup_pos;

        audio_update(&audio);

        BeginDrawing();
        ClearBackground(BLACK);
        viz_draw_string(&string);

        int old_n = n;

        viz_draw_gui(&damping, &pickup_pos, &pluck_pos, &pluck_amp, &n);

        if (n != old_n)
        {
            string_init(&string, n, damping);
            old_n = n;
        }

        EndDrawing();
    }

    audio_close(&audio);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}