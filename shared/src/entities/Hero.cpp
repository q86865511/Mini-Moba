#include "shared/entities/Hero.h"
#include "shared/World.h"

namespace shared {

Hero::Hero(const std::string& slug) {
    team = Team::Blue;
    radius = 26.0f;
    maxHp = hp = 600.0f;
    attackDamage = 55.0f;
    attackRange = 120.0f;
    attackCooldown = 0.7f;
    goldReward = 200;
    xpReward = 130.0f;
    visualKey = slug;
}

float Hero::XpToNext() const { return 100.0f + (float)(level - 1) * 60.0f; }

void Hero::LevelUp() {
    ++level;
    maxHp += 90.0f;
    hp += 90.0f;
    attackDamage += 9.0f;
    if (qCooldown > 1.5f) qCooldown -= 0.1f;
}

void Hero::AddXp(float amount) {
    xp += amount;
    while (level < 18 && xp >= XpToNext()) {
        xp -= XpToNext();
        LevelUp();
    }
}

void Hero::SetMoveTarget(Vec2 target) {
    moveTarget = target;
    hasTarget = true;
}

bool Hero::CastQ(World& world, Vec2 targetPos) {
    if (qTimer > 0.0f) return false;
    Vec2 dir = targetPos - pos;
    if (Length(dir) < 0.001f) dir = facing;
    facing = Normalized(dir);
    world.SpawnProjectile(pos + facing * radius, dir, 850.0f, 120.0f, team, id, 900.0f);
    qTimer = qCooldown;
    attackAnimTime = 0.3f;
    return true;
}

void Hero::Update(World& world, float dt) {
    if (qTimer > 0.0f) { qTimer -= dt; if (qTimer < 0.0f) qTimer = 0.0f; }

    Entity* target = world.FindNearestEnemy(*this, attackRange);
    if (target && attackTimer <= 0.0f) {
        facing = Normalized(target->pos - pos);
        world.DealDamage(*target, attackDamage, team);
        attackTimer = attackCooldown;
        attackAnimTime = 0.3f;
    }

    if (!hasTarget) { moving = false; return; }
    const Vec2 toTarget = moveTarget - pos;
    const float dist = Length(toTarget);
    if (dist > 0.0001f) facing = Normalized(toTarget);
    const float step = moveSpeed * dt;
    if (dist <= step) {
        pos = moveTarget;
        hasTarget = false;
        moving = false;
    } else {
        pos = pos + facing * step;
        moving = true;
    }
}

} // namespace shared
