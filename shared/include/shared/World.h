#pragma once
#include "shared/Vec2.h"

// The authoritative game simulation. Pure logic: no raylib, no networking.
// In phase 1 the client runs this locally; in phase 2 the server owns it and
// the client just renders the snapshots it broadcasts.
namespace shared {

// Fixed simulation timestep. The sim always advances in discrete steps of this
// size, which keeps movement deterministic and transfers cleanly to the server.
constexpr float kFixedDt = 1.0f / 60.0f;

struct Hero {
    Vec2 pos{};
    Vec2 moveTarget{};
    bool hasTarget = false;
    float moveSpeed = 300.0f; // sim units (pixels in phase 1) per second
};

class World {
public:
    Hero hero;

    // The only player input in phase 1: "walk to this point".
    void SetMoveTarget(Vec2 target);

    // Advance the simulation by one fixed step.
    void Tick(float dt);
};

} // namespace shared
