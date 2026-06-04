#include "shared/entities/Minion.h"
#include "shared/World.h"

namespace shared {

void Minion::Update(World& world, float dt) {
    // Stop and attack any enemy in range.
    Entity* target = world.FindNearestEnemy(*this, attackRange);
    if (target) {
        const Vec2 d = target->pos - pos;
        if (Length(d) > 0.001f) facing = Normalized(d);
        moving = false;
        if (attackTimer <= 0.0f) {
            if (ranged) world.SpawnProjectile(pos, d, 600.0f, attackDamage, team, id, attackRange + 80.0f);
            else        world.DealDamage(*target, attackDamage);
            attackTimer = attackCooldown;
            attackAnimTime = 0.3f;
        }
        return;
    }

    // Otherwise march along the lane.
    if (waypoint < path.size()) {
        const Vec2 to = path[waypoint] - pos;
        const float dist = Length(to);
        if (dist < 16.0f) {
            ++waypoint;
        } else {
            facing = Normalized(to);
            pos = pos + facing * (moveSpeed * dt);
            moving = true;
        }
    } else {
        moving = false;
    }
}

} // namespace shared
