#ifndef AUDIO_H
#define AUDIO_H

#include "raylib.h"
#include "str.h"

#define SAMPLE_RATE 44100
#define BUFFER_FRAMES 512

#define MAX_STRINGS 8

typedef struct
{
    AudioStream stream;
    StringState *strings[MAX_STRINGS];
    int num_strings;
    float pickup_pos;
} AudioContext;

void audio_init(AudioContext *ctx, StringState **strings, int num_strings, float pickup_pos);
void audio_update(AudioContext *ctx); // call each frame, feeds the stream
void audio_close(AudioContext *ctx);

#endif