#pragma once
#include <vector>
#include <memory>
#include "shared/Entity.h"

namespace shared {

// Fixed simulation timestep. The sim advances in discrete steps of this size,
// which keeps movement deterministic and transfers cleanly to a server later.
constexpr float kFixedDt = 1.0f / 60.0f;

// Owns every entity and advances the whole simulation. Pure logic: no raylib.
class World {
public:
    std::vector<std::unique_ptr<Entity>> entities;

    // Takes ownership, assigns a unique id, returns a non-owning pointer.
    Entity* Spawn(std::unique_ptr<Entity> e);

    // Advance the whole simulation by one fixed step.
    void Tick(float dt);

    // Nearest living enemy of `self` within range, or nullptr.
    Entity* FindNearestEnemy(const Entity& self, float maxRange) const;

private:
    int nextId_ = 1;
};

} // namespace shared
