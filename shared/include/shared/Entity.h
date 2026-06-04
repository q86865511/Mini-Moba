#pragma once
#include "shared/Vec2.h"
#include "shared/Team.h"
#include <string>

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
    Vec2  facing{ 1.0f, 0.0f };
    float radius = 16.0f;
    float hp = 100.0f;
    float maxHp = 100.0f;
    bool  alive = true;

    // Combat (used from phase 3 on).
    float attackDamage   = 0.0f;
    float attackRange    = 0.0f;
    float attackCooldown = 1.0f;
    float attackTimer    = 0.0f; // counts down to next allowed attack

    // Bounty granted to the killing team when this entity dies.
    int   goldReward = 0;
    float xpReward   = 0.0f;

    // Presentation hints, read by the client. These are plain data (no raylib),
    // so `shared` stays free of any rendering dependency.
    std::string visualKey;       // which sprite sheet the client should use
    bool        moving = false;  // drives move/idle animation
    float       attackAnimTime = 0.0f; // >0 => client plays the attack animation
    float       hurtAnimTime = 0.0f;   // >0 => client plays the hurt animation
    float       deadTimer = 0.0f;      // time spent dead (for corpse cleanup)

    virtual ~Entity() = default;

    virtual EntityType Type() const = 0;
    virtual void Update(World& world, float dt) {}

    bool IsEnemyOf(const Entity& other) const {
        if (team == other.team) return false;
        if (team == Team::Neutral && other.team == Team::Neutral) return false;
        return true; // opposite teams, or neutral-vs-team, are enemies
    }
};

} // namespace shared
