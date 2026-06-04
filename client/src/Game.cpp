#include "Game.h"
#include "MapLoader.h"
#include "raylib.h"
#include <string>

static std::string AppFile(const char* rel) {
    std::string d = GetApplicationDirectory();
    if (!d.empty() && d.back() != '/' && d.back() != '\\') d += '/';
    return d + rel;
}

Game::Game() {
    InitWindow(kScreenW, kScreenH, "Mini MOBA");
    SetTargetFPS(60);

    assets_.Load();
    audio_.Init();

    shared::MapData map =
        LoadMapZones(AppFile("assets/moba_interaction_pack/map/interactive_map_zones.json"));
    match_.Setup(map);

    shared::Vec2 spawn{ 315.0f, 1695.0f };
    for (const auto& b : map.bases) if (b.team == shared::Team::Blue) spawn = b.spawnPos;
    hero_ = match_.AddPlayerHero("ember_vanguard", spawn);
}

Game::~Game() {
    audio_.Shutdown();
    assets_.Unload();
    CloseWindow();
}

void Game::Run() {
    while (!WindowShouldClose()) {
        // 1) Input
        const InputCommand cmd = input_.Process(hero_, camera_);
        if (cmd.moved) audio_.PlayMove();
        if (cmd.castQ && hero_ && hero_->CastQ(match_.world, cmd.worldMouse)) audio_.PlayCast();
        if (cmd.toggleShop) shop_.Toggle();
        if (shop_.open && cmd.buyIndex >= 0 && hero_ && shop_.Buy(cmd.buyIndex, *hero_)) audio_.PlayCast();

        // 2) Fixed-timestep match update (waves + world tick + win check)
        accumulator_ += GetFrameTime();
        if (accumulator_ > 0.25f) accumulator_ = 0.25f;
        while (accumulator_ >= shared::kFixedDt) {
            match_.Update(shared::kFixedDt);
            accumulator_ -= shared::kFixedDt;
        }

        // 3) Combat feedback
        if (!match_.world.hitEvents.empty()) {
            for (const auto& ev : match_.world.hitEvents) effects_.SpawnDamage(ev.pos, ev.amount);
            audio_.PlayHit();
            match_.world.hitEvents.clear();
        }

        // 4) Views + effects + camera + audio + render
        const float frameDt = GetFrameTime();
        effects_.Update(frameDt);
        views_.Sync(match_.world, assets_, frameDt);
        if (hero_) camera_.Follow(hero_->pos, kWorldW, kWorldH);
        audio_.Update();

        const int res = (match_.result == shared::Match::Result::Victory) ? 1
                      : (match_.result == shared::Match::Result::Defeat)  ? 2 : 0;
        renderer_.Draw(match_.world, assets_, views_, camera_, hero_, effects_, res, shop_);
    }
}
