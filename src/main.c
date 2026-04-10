#include "raylib.h"
#include "str.h"
#include "audio.h"
#include "viz.h"

#define WIDTH 1080
#define HEIGHT 720

#define N 80
#define DAMPING 0.0002f

#define PICKUP_POS 0.12f // pickup near the bridge

#define PLUCK_POS 0.2f
#define PLUCK_AMPLITUDE 0.8f

int main(void)
{
    InitWindow(WIDTH, HEIGHT, "String Simulation");
    InitAudioDevice();
    SetTargetFPS(60);

    StringState string;
    string_init(&string, N, DAMPING);

    AudioContext audio;
    audio_init(&audio, &string, PICKUP_POS);

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_SPACE))
            string_pluck(&string, PLUCK_POS, PLUCK_AMPLITUDE);

        audio_update(&audio);

        BeginDrawing();
        ClearBackground(BLACK);
        viz_draw_string(&string);
        EndDrawing();
    }

    audio_close(&audio);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}