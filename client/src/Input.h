#pragma once
#include "shared/entities/Hero.h"
#include "shared/Vec2.h"

class GameCamera;

// What the player asked for this frame.
struct InputCommand {
    bool        moved = false;   // a fresh right-click (for the move cue)
    bool        castQ = false;   // pressed Q this frame
    shared::Vec2 worldMouse{};   // cursor position in world space
};

// Translates raw input into commands on the player's hero.
class Input {
public:
    InputCommand Process(shared::Hero* hero, const GameCamera& camera);
};
