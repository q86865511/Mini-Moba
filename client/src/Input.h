#pragma once
#include "shared/entities/Hero.h"
#include "shared/Vec2.h"

class GameCamera;

// What the player asked for this frame.
struct InputCommand {
    bool         moved = false;      // fresh right-click (move cue)
    bool         castQ = false;      // pressed Q
    bool         toggleShop = false; // pressed B
    int          buyIndex = -1;      // pressed 1..4 to buy (else -1)
    shared::Vec2 worldMouse{};
};

class Input {
public:
    InputCommand Process(shared::Hero* hero, const GameCamera& camera);
};
