#pragma once
#include "shared/Vec2.h"
#include "shared/Team.h"

namespace shared {

class World; // forward declaration (entities act on the world during Update)

enum class EntityType {
    Hero,
    Minion,
    Tower,
    Nexus,
    NeutralMonster,
    Projectile,
};

// Base class for everything that lives in the world. Pure logic: no raylib.
class Entity {
public:
    int   id = 0;
    Team  team = Team::Neutral;
    Vec2  pos{};
    float radius = 16.0f;
    float hp = 100.0f;
    float maxHp = 100.0f;
    bool  alive = true;

    virtual ~Entity() = default;

    virtual EntityType Type() const = 0;

    // Advance this entity by one fixed step. Default: do nothing.
    virtual void Update(World& world, float dt) {}

    bool IsEnemyOf(const Entity& other) const {
        return team != other.team && team != Team::Neutral && other.team != Team::Neutral;
    }
};

} // namespace shared
