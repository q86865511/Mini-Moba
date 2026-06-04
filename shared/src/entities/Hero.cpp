#include "shared/entities/Hero.h"
#include "shared/World.h"

namespace shared {

Hero::Hero() {
    team = Team::Blue;
    radius = 18.0f;
    maxHp = hp = 600.0f;
}

void Hero::SetMoveTarget(Vec2 target) {
    moveTarget = target;
    hasTarget = true;
}

void Hero::Update(World& /*world*/, float dt) {
    if (!hasTarget) return;

    const Vec2 toTarget = moveTarget - pos;
    const float dist = Length(toTarget);
    if (dist > 0.0001f) facing = Normalized(toTarget); // face the way we walk

    const float step = moveSpeed * dt;
    if (dist <= step) {
        pos = moveTarget; // arrive exactly, then stop
        hasTarget = false;
    } else {
        pos = pos + facing * step;
    }
}

} // namespace shared
