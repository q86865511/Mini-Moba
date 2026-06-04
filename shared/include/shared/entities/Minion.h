#pragma once
#include "shared/Entity.h"
#include <vector>

namespace shared {

// A lane minion: walks its waypoint path, stops to attack enemies in range.
class Minion : public Entity {
public:
    std::vector<Vec2> path;     // ordered waypoints toward the enemy base
    size_t waypoint = 0;
    float  moveSpeed = 185.0f;
    bool   ranged = false;

    Minion() { radius = 22.0f; }

    EntityType Type() const override { return EntityType::Minion; }
    void Update(World& world, float dt) override;
};

} // namespace shared
