#include "shared/World.h"
#include "shared/entities/Projectile.h"
#include "shared/entities/Hero.h"
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
    // Index-based over a fixed count: entities created during Update (e.g. projectiles
    // from Spawn*) are appended past `n` and handled next tick. This avoids iterator
    // invalidation when the vector reallocates mid-loop. The pointed-to entity objects
    // live on the heap (unique_ptr), so raw pointers stay valid across reallocation.
    const size_t n = entities.size();
    for (size_t i = 0; i < n; ++i) {
        Entity* e = entities[i].get();
        if (!e) continue;
        Tickdown(e->attackAnimTime, dt);
        Tickdown(e->hurtAnimTime, dt);
        Tickdown(e->attackTimer, dt);
        if (e->alive) e->Update(*this, dt);
        else e->deadTimer += dt;
    }
    // Cleanup: projectiles vanish at once; minions/monsters after a short death pause.
    // Heroes / towers / cores stay (corpse or destroyed-state visuals).
    entities.erase(std::remove_if(entities.begin(), entities.end(),
        [](const std::unique_ptr<Entity>& e) {
            if (!e || e->alive) return false;
            if (e->Type() == EntityType::Projectile) return true;
            if (e->Type() == EntityType::Minion || e->Type() == EntityType::NeutralMonster)
                return e->deadTimer > 1.2f;
            return false;
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

void World::DealDamage(Entity& target, float dmg, Team byTeam) {
    if (!target.alive) return;
    target.hp -= dmg;
    target.hurtAnimTime = 0.25f;
    hitEvents.push_back({ target.pos, dmg });
    if (target.hp > 0.0f) return;

    target.hp = 0.0f;
    target.alive = false;

    // Reward the killing team's heroes: gold to the nearest, xp to all in range.
    if (byTeam == Team::Neutral) return;
    if (target.goldReward <= 0 && target.xpReward <= 0.0f) return;
    Hero* goldHero = nullptr;
    float best = 1400.0f;
    for (auto& e : entities) {
        if (!e || !e->alive || e->Type() != EntityType::Hero || e->team != byTeam) continue;
        const float d = Distance(e->pos, target.pos);
        if (d > 1400.0f) continue;
        static_cast<Hero*>(e.get())->AddXp(target.xpReward);
        if (d < best) { best = d; goldHero = static_cast<Hero*>(e.get()); }
    }
    if (goldHero) goldHero->AddGold(target.goldReward);
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
