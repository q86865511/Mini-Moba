#include "shared/entities/Tower.h"
#include "shared/World.h"

namespace shared {

void Tower::Update(World& world, float dt) {
    Entity* target = world.FindNearestEnemy(*this, attackRange);
    if (target && attackTimer <= 0.0f) {
        const Vec2 d = target->pos - pos;
        world.SpawnProjectile(pos, d, 900.0f, attackDamage, team, id, attackRange + 80.0f);
        attackTimer = attackCooldown;
        attackAnimTime = 0.3f;
    }
}

} // namespace shared
