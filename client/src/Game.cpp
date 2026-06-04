#include "Game.h"
#include "raylib.h"
#include <memory>
#include <vector>

// Build a row of frames from the sprite sheet (256px frames laid out across columns).
static std::vector<AnimFrame> SheetRow(int row, int count, float ms) {
    std::vector<AnimFrame> frames;
    for (int c = 0; c < count; ++c) {
        frames.push_back({ Rectangle{ (float)(c * 256), (float)(row * 256), 256, 256 },
                           ms / 1000.0f });
    }
    return frames;
}

Game::Game() {
    InitWindow(kScreenW, kScreenH, "Mini MOBA - Phase 2");
    SetTargetFPS(60);

    assets_.Load(); // needs the GL context from InitWindow
    audio_.Init();

    // Hero animation (ember_vanguard sheet rows: idle=0 [4 frames], move=1 [6 frames]).
    heroAnim_.AddState("idle", SheetRow(0, 4, 150));
    heroAnim_.AddState("move", SheetRow(1, 6, 115));

    // Spawn the player's hero near the blue base (bottom-left of the 2048 map).
    auto hero = std::make_unique<shared::Hero>();
    hero->pos = { 460.0f, 1600.0f };
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

        // 3) Animation + camera + audio + render
        if (hero_) {
            heroAnim_.Play(hero_->hasTarget ? "move" : "idle");
            heroAnim_.Update(GetFrameTime());
            camera_.Follow(hero_->pos, kWorldW, kWorldH);
        }
        audio_.Update();
        renderer_.Draw(world_, assets_, camera_, hero_, heroAnim_.Frame());
    }
}
