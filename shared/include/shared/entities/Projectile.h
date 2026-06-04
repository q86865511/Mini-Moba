#pragma once
#include "shared/Entity.h"

namespace shared {

// A straight-line projectile that damages the first enemy it touches.
class Projectile : public Entity {
public:
    Vec2  velocity{};
    float speed = 700.0f;
    float damage = 0.0f;
    float maxDist = 900.0f;
    float traveled = 0.0f;
    int   ownerId = 0;

    Projectile() { radius = 12.0f; maxHp = hp = 1.0f; }

    EntityType Type() const override { return EntityType::Projectile; }
    void Update(World& world, float dt) override;
};

} // namespace shared
