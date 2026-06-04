#include "shared/Match.h"
#include "shared/entities/Tower.h"
#include "shared/entities/Nexus.h"
#include "shared/entities/Minion.h"
#include "shared/entities/NeutralMonster.h"
#include <memory>

namespace shared {

static const char* Side(Team t) { return t == Team::Blue ? "blue" : "red"; }

void Match::Setup(const MapData& map) {
    map_ = map;

    for (const auto& b : map.bases) {
        auto core = std::make_unique<Nexus>();
        core->team = b.team;
        core->pos = b.corePos;
        core->maxHp = core->hp = 3500.0f;
        core->visualKey = std::string(Side(b.team)) + "_core";
        Entity* p = world.Spawn(std::move(core));
        if (b.team == Team::Blue) blueNexus_ = p; else redNexus_ = p;
    }

    for (const auto& t : map.towers) {
        auto tw = std::make_unique<Tower>();
        tw->team = t.team;
        tw->pos = t.pos;
        tw->maxHp = tw->hp = 1600.0f;
        tw->attackDamage = 90.0f;
        tw->attackRange = 310.0f;
        tw->attackCooldown = 1.0f;
        tw->goldReward = 150;
        tw->visualKey = std::string(Side(t.team)) + "_tower";
        world.Spawn(std::move(tw));
    }

    for (const auto& c : map.camps) {
        auto mon = std::make_unique<NeutralMonster>();
        mon->pos = c.pos;
        if (c.monster == "river_drake") {
            mon->maxHp = mon->hp = 1800.0f; mon->attackDamage = 60.0f; mon->attackRange = 130.0f;
            mon->attackCooldown = 1.6f; mon->goldReward = 150; mon->xpReward = 200.0f; mon->radius = 46.0f;
        } else if (c.monster == "stone_golem") {
            mon->maxHp = mon->hp = 1200.0f; mon->attackDamage = 40.0f; mon->attackRange = 100.0f;
            mon->attackCooldown = 1.5f; mon->goldReward = 90; mon->xpReward = 130.0f; mon->radius = 40.0f;
        } else { // bramble_spirit
            mon->maxHp = mon->hp = 700.0f; mon->attackDamage = 28.0f; mon->attackRange = 120.0f;
            mon->attackCooldown = 1.3f; mon->goldReward = 60; mon->xpReward = 90.0f; mon->radius = 34.0f;
        }
        mon->visualKey = c.monster;
        world.Spawn(std::move(mon));
    }
}

Hero* Match::AddPlayerHero(const std::string& slug, Vec2 pos) {
    auto h = std::make_unique<Hero>(slug);
    h->pos = pos;
    return static_cast<Hero*>(world.Spawn(std::move(h)));
}

static void ConfigMinion(Minion& m, Team team, const std::string& type) {
    const std::string side = Side(team);
    if (type == "ranged") {
        m.maxHp = m.hp = 130.0f; m.attackDamage = 16.0f; m.attackRange = 240.0f;
        m.attackCooldown = 1.3f; m.moveSpeed = 185.0f; m.ranged = true; m.radius = 20.0f;
        m.visualKey = side + "_ranged_minion";
    } else if (type == "siege") {
        m.maxHp = m.hp = 420.0f; m.attackDamage = 42.0f; m.attackRange = 120.0f;
        m.attackCooldown = 2.6f; m.moveSpeed = 150.0f; m.ranged = false; m.radius = 28.0f;
        m.visualKey = side + "_siege_minion";
    } else { // melee
        m.maxHp = m.hp = 220.0f; m.attackDamage = 12.0f; m.attackRange = 72.0f;
        m.attackCooldown = 1.0f; m.moveSpeed = 185.0f; m.ranged = false; m.radius = 22.0f;
        m.visualKey = side + "_melee_minion";
    }
    m.goldReward = (type == "siege") ? 45 : 22;
    m.xpReward = (type == "siege") ? 70.0f : 35.0f;
}

void Match::SpawnWave() {
    ++waveNumber_;
    for (const auto& lane : map_.lanes) {
        if (lane.points.size() < 2) continue;

        const std::vector<Vec2> bluePath = lane.points;
        const std::vector<Vec2> redPath(lane.points.rbegin(), lane.points.rend());

        auto spawnGroup = [&](Team team, const std::vector<Vec2>& path) {
            const Vec2 dir0 = Normalized(path[1] - path[0]);
            const Vec2 perp{ -dir0.y, dir0.x };
            int idx = 0;
            auto one = [&](const std::string& type) {
                auto m = std::make_unique<Minion>();
                ConfigMinion(*m, team, type);
                m->path = path;
                const float lateral = (float)((idx % 3) - 1) * 26.0f;
                const float back = (float)(idx / 3) * 28.0f;
                m->pos = path.front() + perp * lateral - dir0 * back;
                world.Spawn(std::move(m));
                ++idx;
            };
            one("melee"); one("melee"); one("melee");
            one("ranged");
            if (waveNumber_ % 3 == 0) one("siege");
        };

        spawnGroup(Team::Blue, bluePath);
        spawnGroup(Team::Red, redPath);
    }
}

void Match::Update(float dt) {
    if (result == Result::Ongoing) {
        waveTimer_ -= dt;
        if (waveTimer_ <= 0.0f) { SpawnWave(); waveTimer_ = 28.0f; }
    }

    world.Tick(dt);

    if (result == Result::Ongoing) {
        if (blueNexus_ && !blueNexus_->alive) result = Result::Defeat;
        if (redNexus_ && !redNexus_->alive) result = Result::Victory;
    }
}

} // namespace shared
