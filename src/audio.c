#include "audio.h"

void audio_init(AudioContext *ctx, StringState *string, float pickup_pos)
{
    ctx->string = string;
    ctx->pickup_pos = pickup_pos;

    SetAudioStreamBufferSizeDefault(BUFFER_FRAMES);

    ctx->stream = LoadAudioStream(SAMPLE_RATE, 32, 1);
    PlayAudioStream(ctx->stream);
}

void audio_update(AudioContext *ctx)
{
    // Keep feeding the stream as long as it needs data
    while (IsAudioStreamProcessed(ctx->stream))
    {
        float buffer[BUFFER_FRAMES];

        for (int i = 0; i < BUFFER_FRAMES; i++)
        {
            string_step(ctx->string);
            buffer[i] = string_sample(ctx->string, ctx->pickup_pos) * 0.2f;
        }

        UpdateAudioStream(ctx->stream, buffer, BUFFER_FRAMES);
    }
}

void audio_close(AudioContext *ctx)
{
    StopAudioStream(ctx->stream);
    UnloadAudioStream(ctx->stream);
}
