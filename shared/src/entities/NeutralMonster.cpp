#include "shared/entities/NeutralMonster.h"
#include "shared/World.h"

namespace shared {

void NeutralMonster::Update(World& world, float dt) {
    // Stand at the camp and strike anything hostile in range.
    Entity* target = world.FindNearestEnemy(*this, attackRange);
    if (target && attackTimer <= 0.0f) {
        facing = Normalized(target->pos - pos);
        world.DealDamage(*target, attackDamage, team);
        attackTimer = attackCooldown;
        attackAnimTime = 0.3f;
    }
    moving = false;
}

} // namespace shared
