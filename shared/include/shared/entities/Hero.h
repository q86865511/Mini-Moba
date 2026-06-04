#pragma once
#include "shared/Entity.h"
#include <vector>
#include <string>

namespace shared {

// A controllable champion: moves, auto-attacks, casts Q, gains gold/xp/levels.
class Hero : public Entity {
public:
    Vec2  moveTarget{};
    bool  hasTarget = false;
    float moveSpeed = 300.0f;

    float qCooldown = 3.0f;
    float qTimer = 0.0f;

    // Economy / progression.
    int   gold = 0;
    int   level = 1;
    float xp = 0.0f;
    std::vector<std::string> items;

    explicit Hero(const std::string& slug = "ember_vanguard");

    EntityType Type() const override { return EntityType::Hero; }
    void Update(World& world, float dt) override;

    void SetMoveTarget(Vec2 target);
    bool CastQ(World& world, Vec2 targetPos);

    void AddGold(int amount) { gold += amount; }
    bool SpendGold(int amount) { if (gold < amount) return false; gold -= amount; return true; }
    void AddXp(float amount);
    float XpToNext() const;   // xp needed for the current level

private:
    void LevelUp();
};

} // namespace shared
