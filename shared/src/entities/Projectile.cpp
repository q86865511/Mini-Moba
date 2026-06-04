#include "shared/entities/Projectile.h"
#include "shared/World.h"

namespace shared {

void Projectile::Update(World& world, float dt) {
    pos = pos + velocity * dt;
    traveled += speed * dt;

    for (auto& e : world.entities) {
        if (!e || !e->alive) continue;
        if (e->Type() == EntityType::Projectile) continue;
        if (!e->IsEnemyOf(*this)) continue;
        if (Distance(pos, e->pos) <= e->radius + radius) {
            world.DealDamage(*e, damage);
            alive = false;
            return;
        }
    }

    if (traveled >= maxDist) alive = false;
}

} // namespace shared
