#pragma once
#include "raylib.h"

// Owns audio. Everything is generated procedurally (no external audio files).
class Audio {
public:
    void Init();
    void Shutdown();
    void Update();      // loops the ambient track
    void PlayMove();
    void PlayHit();
    void PlayCast();

private:
    bool  ready_ = false;
    Sound moveSfx_{};
    Sound hitSfx_{};
    Sound castSfx_{};
    Sound ambient_{};
    bool  ambientReady_ = false;
};
