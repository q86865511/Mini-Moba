#include "shared/World.h"
#include "shared/entities/Projectile.h"
#include <algorithm>

namespace shared {

Entity* World::Spawn(std::unique_ptr<Entity> e) {
    e->id = nextId_++;
    Entity* ptr = e.get();
    entities.push_back(std::move(e));
    return ptr;
}

static void Tickdown(float& t, float dt) { if (t > 0.0f) { t -= dt; if (t < 0.0f) t = 0.0f; } }

void World::Tick(float dt) {
    for (auto& e : entities) {
        if (!e) continue;
        Tickdown(e->attackAnimTime, dt);
        Tickdown(e->hurtAnimTime, dt);
        Tickdown(e->attackTimer, dt);
        if (e->alive) e->Update(*this, dt);
    }
    // Remove dead projectiles immediately (they have no corpse/death animation).
    entities.erase(std::remove_if(entities.begin(), entities.end(),
        [](const std::unique_ptr<Entity>& e) {
            return e && !e->alive && e->Type() == EntityType::Projectile;
        }), entities.end());
}

Entity* World::FindNearestEnemy(const Entity& self, float maxRange) const {
    Entity* best = nullptr;
    float bestDist = maxRange;
    for (const auto& e : entities) {
        if (!e || !e->alive || e.get() == &self) continue;
        if (!e->IsEnemyOf(self)) continue;
        const float d = Distance(self.pos, e->pos);
        if (d < bestDist) { bestDist = d; best = e.get(); }
    }
    return best;
}

void World::DealDamage(Entity& target, float dmg) {
    if (!target.alive) return;
    target.hp -= dmg;
    target.hurtAnimTime = 0.25f;
    hitEvents.push_back({ target.pos, dmg });
    if (target.hp <= 0.0f) {
        target.hp = 0.0f;
        target.alive = false;
    }
}

void World::SpawnProjectile(Vec2 pos, Vec2 dir, float speed, float dmg, Team team,
                            int ownerId, float maxDist) {
    auto pr = std::make_unique<Projectile>();
    pr->pos = pos;
    pr->team = team;
    pr->ownerId = ownerId;
    pr->velocity = Normalized(dir) * speed;
    pr->speed = speed;
    pr->damage = dmg;
    pr->maxDist = maxDist;
    Spawn(std::move(pr));
}

} // namespace shared
