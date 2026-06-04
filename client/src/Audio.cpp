#include "Audio.h"
#include <math.h>
#include <stdlib.h>

static float Rnd() { return (float)rand() / (float)RAND_MAX * 2.0f - 1.0f; }

static Sound SoundFromSamples(short* data, int frames, int sampleRate) {
    Wave w{ (unsigned int)frames, (unsigned int)sampleRate, 16, 1, data };
    Sound s = LoadSoundFromWave(w);
    UnloadWave(w);
    return s;
}

// Short downward "blip" for move orders.
static Sound MakeMoveSfx() {
    const int sr = 44100; const float dur = 0.09f; const int n = (int)(sr * dur);
    short* d = (short*)malloc(sizeof(short) * (size_t)n);
    for (int i = 0; i < n; i++) {
        float k = (float)i / n, t = (float)i / sr;
        float s = sinf(2 * PI * (720.0f - 260.0f * k) * t) * (1.0f - k) * 0.35f;
        d[i] = (short)(s * 32767);
    }
    return SoundFromSamples(d, n, sr);
}

// Punchy "thwack" for hits (noise + low sine, fast decay).
static Sound MakeHitSfx() {
    const int sr = 44100; const float dur = 0.10f; const int n = (int)(sr * dur);
    short* d = (short*)malloc(sizeof(short) * (size_t)n);
    for (int i = 0; i < n; i++) {
        float k = (float)i / n, t = (float)i / sr;
        float env = (1.0f - k) * (1.0f - k);
        float s = (0.6f * sinf(2 * PI * 180.0f * t) + 0.4f * Rnd()) * env * 0.5f;
        d[i] = (short)(s * 32767);
    }
    return SoundFromSamples(d, n, sr);
}

// Rising "zap" for casting an ability.
static Sound MakeCastSfx() {
    const int sr = 44100; const float dur = 0.20f; const int n = (int)(sr * dur);
    short* d = (short*)malloc(sizeof(short) * (size_t)n);
    for (int i = 0; i < n; i++) {
        float k = (float)i / n, t = (float)i / sr;
        float freq = 320.0f + 700.0f * k;
        float env = sinf(PI * k); // fade in/out
        float s = sinf(2 * PI * freq * t) * env * 0.35f;
        d[i] = (short)(s * 32767);
    }
    return SoundFromSamples(d, n, sr);
}

// Gentle 8-second ambient pad that begins/ends near silence (loops by replay).
static Sound MakeAmbientPad() {
    const int sr = 44100; const float dur = 8.0f; const int n = (int)(sr * dur);
    short* d = (short*)malloc(sizeof(short) * (size_t)n);
    const float roots[4] = { 220.0f, 174.61f, 196.0f, 146.83f };
    for (int i = 0; i < n; i++) {
        float t = (float)i / sr;
        int chord = ((int)(t / 2.0f)) % 4;
        float local = fmodf(t, 2.0f);
        float env = 0.5f - 0.5f * cosf(2 * PI * (local / 2.0f));
        float r = roots[chord];
        float s = sinf(2 * PI * r * t) + 0.6f * sinf(2 * PI * r * 1.5f * t) + 0.4f * sinf(2 * PI * r * 2.0f * t);
        s *= env * 0.09f;
        if (s > 1) s = 1; if (s < -1) s = -1;
        d[i] = (short)(s * 32767);
    }
    return SoundFromSamples(d, n, sr);
}

void Audio::Init() {
    InitAudioDevice();
    if (!IsAudioDeviceReady()) { ready_ = false; return; }
    ready_ = true;

    moveSfx_ = MakeMoveSfx();  SetSoundVolume(moveSfx_, 0.45f);
    hitSfx_  = MakeHitSfx();   SetSoundVolume(hitSfx_, 0.5f);
    castSfx_ = MakeCastSfx();  SetSoundVolume(castSfx_, 0.5f);

    ambient_ = MakeAmbientPad(); SetSoundVolume(ambient_, 0.30f);
    PlaySound(ambient_);
    ambientReady_ = true;
}

void Audio::Update() {
    if (ambientReady_ && !IsSoundPlaying(ambient_)) PlaySound(ambient_);
}

void Audio::PlayMove() { if (ready_) PlaySound(moveSfx_); }
void Audio::PlayHit()  { if (ready_) PlaySound(hitSfx_); }
void Audio::PlayCast() { if (ready_) PlaySound(castSfx_); }

void Audio::Shutdown() {
    if (ambientReady_) UnloadSound(ambient_);
    if (ready_) {
        UnloadSound(moveSfx_);
        UnloadSound(hitSfx_);
        UnloadSound(castSfx_);
        CloseAudioDevice();
    }
    ready_ = false;
    ambientReady_ = false;
}
