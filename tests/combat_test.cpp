// Headless unit tests for the shared combat simulation (no raylib needed).
#include "shared/World.h"
#include "shared/Match.h"
#include "shared/entities/Hero.h"
#include "shared/entities/Projectile.h"
#include <cstdio>
#include <memory>

using namespace shared;

static int g_fails = 0;
#define CHECK(cond, msg) do { if (!(cond)) { std::printf("FAIL: %s\n", msg); ++g_fails; } \
                              else std::printf("ok  : %s\n", msg); } while (0)

int main() {
    // 1) Hero auto-attacks a nearby enemy until it dies.
    {
        World w;
        auto hero = std::make_unique<Hero>();
        hero->team = Team::Blue; hero->pos = { 0, 0 };
        w.Spawn(std::move(hero));

        auto enemy = std::make_unique<Hero>();
        enemy->team = Team::Red; enemy->pos = { 80, 0 };
        enemy->maxHp = enemy->hp = 100.0f;
        enemy->attackRange = 0.0f; // does not fight back
        Entity* e = w.Spawn(std::move(enemy));

        const float startHp = e->hp;
        bool sawHitEvent = false;
        for (int i = 0; i < 300; ++i) { w.Tick(kFixedDt); if (!w.hitEvents.empty()) sawHitEvent = true; w.hitEvents.clear(); }

        CHECK(e->hp < startHp, "auto-attack reduced enemy HP");
        CHECK(!e->alive, "enemy died from auto-attacks");
        CHECK(sawHitEvent, "hit events were produced");
    }

    // 2) A projectile damages the first enemy it reaches.
    {
        World w;
        auto enemy = std::make_unique<Hero>();
        enemy->team = Team::Red; enemy->pos = { 300, 0 };
        enemy->maxHp = enemy->hp = 200.0f;
        enemy->attackRange = 0.0f;
        Entity* e = w.Spawn(std::move(enemy));

        const float startHp = e->hp;
        w.SpawnProjectile({ 0, 0 }, { 1, 0 }, 800.0f, 40.0f, Team::Blue, 999, 900.0f);
        for (int i = 0; i < 120; ++i) w.Tick(kFixedDt);

        CHECK(e->hp < startHp, "projectile damaged the enemy");
    }

    // 3) Projectiles are cleaned up after they expire (no enemy in path).
    {
        World w;
        w.SpawnProjectile({ 0, 0 }, { 1, 0 }, 800.0f, 40.0f, Team::Blue, 1, 200.0f);
        for (int i = 0; i < 120; ++i) w.Tick(kFixedDt);
        CHECK(w.entities.empty(), "expired projectile was removed");
    }

    // 4) Match: setup builds towers + cores; waves spawn minions; killing red core wins.
    {
        Match m;
        MapData md;
        LanePath lane; lane.points = { { 0, 0 }, { 500, 500 }, { 1000, 1000 } };
        md.lanes.push_back(lane);
        md.bases.push_back({ { 0, 0 }, { 40, 40 }, Team::Blue });
        md.bases.push_back({ { 1000, 1000 }, { 960, 960 }, Team::Red });
        md.towers.push_back({ { 350, 350 }, Team::Blue });
        m.Setup(md);
        CHECK(m.world.entities.size() == 3, "match setup spawned 2 cores + 1 tower");

        for (int i = 0; i < 400; ++i) m.Update(kFixedDt); // ~6.6s; first wave at 5s
        bool anyMinion = false;
        for (auto& e : m.world.entities) if (e && e->Type() == EntityType::Minion) anyMinion = true;
        CHECK(anyMinion, "a minion wave spawned");

        for (auto& e : m.world.entities)
            if (e && e->Type() == EntityType::Nexus && e->team == Team::Red) e->alive = false;
        m.Update(kFixedDt);
        CHECK(m.result == Match::Result::Victory, "destroying red core => Victory");
    }

    std::printf(g_fails == 0 ? "\nALL TESTS PASSED\n" : "\n%d TEST(S) FAILED\n", g_fails);
    return g_fails;
}
