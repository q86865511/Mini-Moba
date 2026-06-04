#pragma once
#include "shared/Entity.h"

namespace shared {

// A controllable champion. Phase 2: just moves toward a clicked target.
class Hero : public Entity {
public:
    Vec2  moveTarget{};
    bool  hasTarget = false;
    float moveSpeed = 300.0f;       // sim units per second
    Vec2  facing{ 1.0f, 0.0f };     // unit vector the hero is facing (for rendering/abilities)

    Hero();

    EntityType Type() const override { return EntityType::Hero; }
    void Update(World& world, float dt) override;

    // The only player input in phase 2: "walk to this point".
    void SetMoveTarget(Vec2 target);
};

} // namespace shared
