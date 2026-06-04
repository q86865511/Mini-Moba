#pragma once
#include "shared/Entity.h"

namespace shared {

// A stationary defensive tower: fires projectiles at enemies in range.
class Tower : public Entity {
public:
    Tower() { radius = 48.0f; }

    EntityType Type() const override { return EntityType::Tower; }
    void Update(World& world, float dt) override;
};

} // namespace shared
