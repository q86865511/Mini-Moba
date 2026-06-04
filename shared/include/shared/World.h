#pragma once
#include <vector>
#include <memory>
#include "shared/Entity.h"

namespace shared {

// Fixed simulation timestep.
constexpr float kFixedDt = 1.0f / 60.0f;

// A combat hit, surfaced to the client for floating numbers / sound.
struct HitEvent { Vec2 pos; float amount; };

// Owns every entity and advances the whole simulation. Pure logic: no raylib.
class World {
public:
    std::vector<std::unique_ptr<Entity>> entities;

    // Combat feedback produced this tick; the client reads and clears it.
    std::vector<HitEvent> hitEvents;

    Entity* Spawn(std::unique_ptr<Entity> e);
    void    Tick(float dt);

    Entity* FindNearestEnemy(const Entity& self, float maxRange) const;

    // Apply damage (handles death, rewards to `byTeam`, and records a HitEvent).
    void DealDamage(Entity& target, float dmg, Team byTeam = Team::Neutral);

    // Spawn a straight-line projectile owned by `ownerId` on `team`.
    void SpawnProjectile(Vec2 pos, Vec2 dir, float speed, float dmg, Team team,
                         int ownerId, float maxDist);

private:
    int nextId_ = 1;
};

} // namespace shared
