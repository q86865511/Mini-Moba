#pragma once
#include "shared/Entity.h"

namespace shared {

// A controllable champion: moves, auto-attacks nearby enemies, and casts Q.
class Hero : public Entity {
public:
    Vec2  moveTarget{};
    bool  hasTarget = false;
    float moveSpeed = 300.0f;
    Vec2  facing{ 1.0f, 0.0f };

    float qCooldown = 3.0f;
    float qTimer = 0.0f;

    explicit Hero(const std::string& slug = "ember_vanguard");

    EntityType Type() const override { return EntityType::Hero; }
    void Update(World& world, float dt) override;

    void SetMoveTarget(Vec2 target);
    bool CastQ(World& world, Vec2 targetPos); // returns true if it fired
};

} // namespace shared
