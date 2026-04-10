#include "raylib.h"
#include "str.h"
#include "audio.h"
#include "viz.h"

#include <math.h>

#define WIDTH 1080
#define HEIGHT 720

#define NUM_NOTES 8

float damping = 0.0002f;
float pickup_pos = 0.12f;
float pluck_pos = 0.2f;
float pluck_amp = 0.8f;

int octave_shift = 0;

static const int KEYS[NUM_NOTES] = {
    KEY_A, KEY_S, KEY_D, KEY_F, KEY_G, KEY_H, KEY_J, KEY_K};

// A3 major: A B C# D E F# G# A4
static const float FREQS[NUM_NOTES] = {
    220.0f, 246.94f, 277.18f, 293.66f,
    329.63f, 369.99f, 415.30f, 440.0f};

int main(void)
{
    InitWindow(WIDTH, HEIGHT, "String Simulation");
    InitAudioDevice();
    SetTargetFPS(60);

    StringState strings[NUM_NOTES];
    for (int i = 0; i < NUM_NOTES; i++)
    {
        float freq = FREQS[i] * powf(2.0f, (float)octave_shift);
        int note_n = (int)roundf(44100.0f / (2.0f * freq));
        string_init(&strings[i], note_n, damping);
    }

    StringState *string_ptrs[NUM_NOTES];
    for (int i = 0; i < NUM_NOTES; i++)
        string_ptrs[i] = &strings[i];

    AudioContext audio;
    audio_init(&audio, string_ptrs, NUM_NOTES, pickup_pos);

    int old_octave = octave_shift;

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_UP))
            octave_shift++;
        if (IsKeyPressed(KEY_DOWN))
            octave_shift--;
        if (octave_shift > 2)
            octave_shift = 2;
        if (octave_shift < 0)
            octave_shift = 0;

        if (octave_shift != old_octave)
        {
            for (int i = 0; i < NUM_NOTES; i++)
            {
                float freq = FREQS[i] * powf(2.0f, (float)octave_shift);
                int note_n = (int)roundf(44100.0f / (2.0f * freq)) + 1;
                string_init(&strings[i], note_n, damping);
            }
            old_octave = octave_shift;
        }

        for (int i = 0; i < NUM_NOTES; i++)
            if (IsKeyPressed(KEYS[i]))
                string_pluck(&strings[i], pluck_pos, pluck_amp);

        for (int i = 0; i < NUM_NOTES; i++)
            strings[i].damping = damping;

        audio.pickup_pos = pickup_pos;
        audio_update(&audio);

        for (int i = 0; i < NUM_NOTES; i++)
            string_sync_display(&strings[i]);

        BeginDrawing();
        ClearBackground(BLACK);
        viz_draw_strings(strings, NUM_NOTES);
        viz_draw_gui(&damping, &pickup_pos, &pluck_pos, &pluck_amp);
        DrawText(TextFormat("Octave: %+d  (UP/DOWN to change)", octave_shift), 10, HEIGHT - 30, 20, GRAY);
        EndDrawing();
    }

    audio_close(&audio);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}