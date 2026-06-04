#include "shared/World.h"

namespace shared {

Entity* World::Spawn(std::unique_ptr<Entity> e) {
    e->id = nextId_++;
    Entity* ptr = e.get();
    entities.push_back(std::move(e));
    return ptr;
}

void World::Tick(float dt) {
    for (auto& e : entities) {
        if (e && e->alive) e->Update(*this, dt);
    }
    // (Dead-entity removal is added in phase 3, once things can actually die.)
}

Entity* World::FindNearestEnemy(const Entity& self, float maxRange) const {
    Entity* best = nullptr;
    float bestDist = maxRange;
    for (const auto& e : entities) {
        if (!e || !e->alive || e.get() == &self) continue;
        if (!e->IsEnemyOf(self)) continue;
        const float d = Distance(self.pos, e->pos);
        if (d < bestDist) {
            bestDist = d;
            best = e.get();
        }
    }
    return best;
}

} // namespace shared
