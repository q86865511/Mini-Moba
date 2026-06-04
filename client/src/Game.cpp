#include "Game.h"
#include "raylib.h"
#include <memory>

Game::Game() {
    InitWindow(kScreenW, kScreenH, "Mini MOBA - Phase 2");
    SetTargetFPS(60);

    assets_.Load(); // needs the GL context from InitWindow
    audio_.Init();

    // Spawn the player's hero at the centre of the world.
    auto hero = std::make_unique<shared::Hero>();
    hero->pos = { kWorldW / 2.0f, kWorldH / 2.0f };
    hero_ = static_cast<shared::Hero*>(world_.Spawn(std::move(hero)));
}

Game::~Game() {
    audio_.Shutdown();
    assets_.Unload();
    CloseWindow();
}

void Game::Run() {
    while (!WindowShouldClose()) {
        // 1) Input -> commands (and a move cue on a fresh click)
        if (input_.Process(hero_, camera_)) audio_.PlayMove();

        // 2) Fixed-timestep simulation
        accumulator_ += GetFrameTime();
        if (accumulator_ > 0.25f) accumulator_ = 0.25f; // avoid spiral of death
        while (accumulator_ >= shared::kFixedDt) {
            world_.Tick(shared::kFixedDt);
            accumulator_ -= shared::kFixedDt;
        }

        // 3) Camera + audio + render
        if (hero_) camera_.Follow(hero_->pos, kWorldW, kWorldH);
        audio_.Update();
        renderer_.Draw(world_, assets_, camera_, hero_);
    }
}
