#pragma once
#include "shared/entities/Hero.h"

class GameCamera;

// Translates raw input (mouse/keyboard) into commands on the player's hero.
class Input {
public:
    // Updates the hero's move target while RIGHT mouse is held.
    // Returns true on a fresh press (so the caller can play a move cue).
    bool Process(shared::Hero* hero, const GameCamera& camera);
};
