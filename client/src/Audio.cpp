#include "Audio.h"
#include <math.h>
#include <stdlib.h>

// A short downward "blip" for move orders.
static Sound MakeMoveSfx() {
    const int   sampleRate = 44100;
    const float dur = 0.09f;
    const int   frames = (int)(sampleRate * dur);
    short* data = (short*)malloc(sizeof(short) * (size_t)frames);
    for (int i = 0; i < frames; i++) {
        const float t = (float)i / sampleRate;
        const float k = (float)i / frames;
        const float env = 1.0f - k;                 // linear decay
        const float freq = 720.0f - 260.0f * k;     // slight downward chirp
        const float s = sinf(2.0f * PI * freq * t) * env * 0.35f;
        data[i] = (short)(s * 32767.0f);
    }
    Wave w{ (unsigned int)frames, (unsigned int)sampleRate, 16, 1, data };
    Sound snd = LoadSoundFromWave(w);
    UnloadWave(w); // frees data
    return snd;
}

// A gentle 8-second ambient pad (slow 4-chord progression of soft sines).
// Starts and ends near silence so it can be looped by simply replaying it.
static Sound MakeAmbientPad() {
    const int   sampleRate = 44100;
    const float dur = 8.0f;
    const int   frames = (int)(sampleRate * dur);
    short* data = (short*)malloc(sizeof(short) * (size_t)frames);

    const float roots[4] = { 220.0f, 174.61f, 196.0f, 146.83f }; // A3, F3, G3, D3
    for (int i = 0; i < frames; i++) {
        const float t = (float)i / sampleRate;
        const int   chord = ((int)(t / 2.0f)) % 4;
        const float local = fmodf(t, 2.0f);
        const float env = 0.5f - 0.5f * cosf(2.0f * PI * (local / 2.0f)); // swell 0..1..0
        const float r = roots[chord];
        float s = sinf(2 * PI * r * t)
                + 0.6f * sinf(2 * PI * r * 1.5f * t)
                + 0.4f * sinf(2 * PI * r * 2.0f * t);
        s *= env * 0.10f; // quiet
        if (s >  1.0f) s =  1.0f;
        if (s < -1.0f) s = -1.0f;
        data[i] = (short)(s * 32767.0f);
    }
    Wave w{ (unsigned int)frames, (unsigned int)sampleRate, 16, 1, data };
    Sound snd = LoadSoundFromWave(w);
    UnloadWave(w);
    return snd;
}

void Audio::Init() {
    InitAudioDevice();
    if (!IsAudioDeviceReady()) { ready_ = false; return; }
    ready_ = true;

    moveSfx_ = MakeMoveSfx();
    SetSoundVolume(moveSfx_, 0.5f);

    ambient_ = MakeAmbientPad();
    SetSoundVolume(ambient_, 0.35f);
    PlaySound(ambient_);
    ambientReady_ = true;
}

void Audio::Update() {
    // Loop the ambient track (it begins/ends near silence, so the seam is inaudible).
    if (ambientReady_ && !IsSoundPlaying(ambient_)) PlaySound(ambient_);
}

void Audio::PlayMove() {
    if (ready_) PlaySound(moveSfx_);
}

void Audio::Shutdown() {
    if (ambientReady_) UnloadSound(ambient_);
    if (ready_) {
        UnloadSound(moveSfx_);
        CloseAudioDevice();
    }
    ready_ = false;
    ambientReady_ = false;
}
