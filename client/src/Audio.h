#pragma once
#include "raylib.h"

// Owns audio. Phase 2 generates everything procedurally (a move "blip" and a
// gentle ambient pad), so there are no external audio files. Later phases can
// swap in downloaded CC0 music/SFX behind this same interface.
class Audio {
public:
    void Init();      // InitAudioDevice + generate sfx & ambient track
    void Shutdown();
    void Update();    // call every frame (loops the ambient track)
    void PlayMove();  // a short cue when the player issues a move order

private:
    bool  ready_ = false;
    Sound moveSfx_{};
    Sound ambient_{};
    bool  ambientReady_ = false;
};
