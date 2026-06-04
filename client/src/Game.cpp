#include "Game.h"
#include "raylib.h"
#include <memory>

Game::Game() {
    InitWindow(kScreenW, kScreenH, "Mini MOBA");
    SetTargetFPS(60);

    assets_.Load();
    audio_.Init();

    // Player hero near the blue base (bottom-left of the 2048 map).
    auto hero = std::make_unique<shared::Hero>("ember_vanguard");
    hero->pos = { 460.0f, 1600.0f };
    hero_ = static_cast<shared::Hero*>(world_.Spawn(std::move(hero)));

    // Phase 3: a couple of red training targets that fight back when approached.
    auto spawnDummy = [&](float x, float y, const std::string& slug) {
        auto d = std::make_unique<shared::Hero>(slug);
        d->team = shared::Team::Red;
        d->pos = { x, y };
        d->maxHp = d->hp = 450.0f;
        d->attackDamage = 22.0f;
        d->attackRange = 110.0f;
        d->attackCooldown = 1.1f;
        world_.Spawn(std::move(d));
    };
    spawnDummy(720.0f, 1430.0f, "iron_sentinel");
    spawnDummy(850.0f, 1330.0f, "night_veil");
}

Game::~Game() {
    audio_.Shutdown();
    assets_.Unload();
    CloseWindow();
}

void Game::Run() {
    while (!WindowShouldClose()) {
        // 1) Input -> commands
        const InputCommand cmd = input_.Process(hero_, camera_);
        if (cmd.moved) audio_.PlayMove();
        if (cmd.castQ && hero_ && hero_->CastQ(world_, cmd.worldMouse)) audio_.PlayCast();

        // 2) Fixed-timestep simulation
        accumulator_ += GetFrameTime();
        if (accumulator_ > 0.25f) accumulator_ = 0.25f;
        while (accumulator_ >= shared::kFixedDt) {
            world_.Tick(shared::kFixedDt);
            accumulator_ -= shared::kFixedDt;
        }

        // 3) Combat feedback (floating numbers + hit sound)
        if (!world_.hitEvents.empty()) {
            for (const auto& ev : world_.hitEvents) effects_.SpawnDamage(ev.pos, ev.amount);
            audio_.PlayHit();
            world_.hitEvents.clear();
        }

        // 4) Views + effects + camera + audio + render
        const float frameDt = GetFrameTime();
        effects_.Update(frameDt);
        views_.Sync(world_, assets_, frameDt);
        if (hero_) camera_.Follow(hero_->pos, kWorldW, kWorldH);
        audio_.Update();
        renderer_.Draw(world_, assets_, views_, camera_, hero_, effects_);
    }
}
