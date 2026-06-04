#include "shared/World.h"

namespace shared {

void World::SetMoveTarget(Vec2 target) {
    hero.moveTarget = target;
    hero.hasTarget = true;
}

void World::Tick(float dt) {
    if (!hero.hasTarget) return;

    const Vec2 toTarget = hero.moveTarget - hero.pos;
    const float dist = Length(toTarget);
    const float step = hero.moveSpeed * dt;

    if (dist <= step) {
        hero.pos = hero.moveTarget; // arrive exactly, then stop
        hero.hasTarget = false;
    } else {
        hero.pos = hero.pos + Normalized(toTarget) * step;
    }
}

} // namespace shared
