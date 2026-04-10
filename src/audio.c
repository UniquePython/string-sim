#include "audio.h"

void audio_init(AudioContext *ctx, StringState **strings, int num_strings, float pickup_pos)
{
    // Clamp number of strings
    if (num_strings > MAX_STRINGS)
        num_strings = MAX_STRINGS;

    ctx->num_strings = num_strings;
    ctx->pickup_pos = pickup_pos;

    // Store string pointers
    for (int i = 0; i < num_strings; i++)
        ctx->strings[i] = strings[i];

    SetAudioStreamBufferSizeDefault(BUFFER_FRAMES);

    ctx->stream = LoadAudioStream(SAMPLE_RATE, 32, 1);
    PlayAudioStream(ctx->stream);
}

void audio_update(AudioContext *ctx)
{
    while (IsAudioStreamProcessed(ctx->stream))
    {
        float buffer[BUFFER_FRAMES];

        for (int i = 0; i < BUFFER_FRAMES; i++)
        {
            float sample = 0.0f;

            // Step and sample each string
            for (int s = 0; s < ctx->num_strings; s++)
            {
                StringState *str = ctx->strings[s];

                string_step(str);
                sample += string_sample(str, ctx->pickup_pos);
            }

            // Simple normalization to avoid clipping
            if (ctx->num_strings > 0)
                sample /= (float)ctx->num_strings;

            // output gain
            buffer[i] = sample * 0.2f;
        }

        UpdateAudioStream(ctx->stream, buffer, BUFFER_FRAMES);
    }
}

void audio_close(AudioContext *ctx)
{
    StopAudioStream(ctx->stream);
    UnloadAudioStream(ctx->stream);
}
