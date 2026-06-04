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

    mapData_ = LoadMapZones(AppFile("assets/moba_interaction_pack/map/interactive_map_zones.json"));
    for (const auto& b : mapData_.bases)
        if (b.team == shared::Team::Blue) blueSpawn_ = b.spawnPos;
}

Game::~Game() {
    audio_.Shutdown();
    assets_.Unload();
    CloseWindow();
}

void Game::Run() {
    while (!WindowShouldClose()) {
        switch (state_) {
            case State::Menu:       UpdateMenu();       break;
            case State::HeroSelect: UpdateHeroSelect(); break;
            case State::Playing:    UpdatePlaying();    break;
        }
    }
}

void Game::UpdateMenu() {
    if (IsKeyPressed(KEY_ENTER)) state_ = State::HeroSelect;
    renderer_.DrawMenu(assets_);
}

void Game::UpdateHeroSelect() {
    const int n = (int)heroChoices_.size();
    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) selectedHero_ = (selectedHero_ + 1) % n;
    if (IsKeyPressed(KEY_LEFT)  || IsKeyPressed(KEY_A)) selectedHero_ = (selectedHero_ + n - 1) % n;
    if (IsKeyPressed(KEY_ENTER)) { StartMatch(); state_ = State::Playing; }
    renderer_.DrawHeroSelect(heroChoices_, selectedHero_, assets_);
}

void Game::StartMatch() {
    views_ = ViewRegistry{};
    shop_.open = false;
    match_ = std::make_unique<shared::Match>();
    match_->Setup(mapData_);
    hero_ = match_->AddPlayerHero(heroChoices_[selectedHero_], blueSpawn_);
    accumulator_ = 0.0f;
    respawnTimer_ = 0.0f;
}

void Game::ReturnToMenu() {
    match_.reset();
    hero_ = nullptr;
    views_ = ViewRegistry{};
    shop_.open = false;
    state_ = State::Menu;
}

void Game::UpdatePlaying() {
    if (!match_) { ReturnToMenu(); return; }
    const bool ongoing = match_->result == shared::Match::Result::Ongoing;

    const InputCommand cmd = input_.Process(hero_, camera_);
    if (ongoing && hero_ && hero_->alive) {
        if (cmd.moved) audio_.PlayMove();
        if (cmd.castQ && hero_->CastQ(match_->world, cmd.worldMouse)) audio_.PlayCast();
        if (cmd.toggleShop) shop_.Toggle();
        if (shop_.open && cmd.buyIndex >= 0 && shop_.Buy(cmd.buyIndex, *hero_)) audio_.PlayCast();
    }
    if (!ongoing && IsKeyPressed(KEY_ENTER)) { ReturnToMenu(); return; }

    accumulator_ += GetFrameTime();
    if (accumulator_ > 0.25f) accumulator_ = 0.25f;
    while (accumulator_ >= shared::kFixedDt) {
        match_->Update(shared::kFixedDt);
        accumulator_ -= shared::kFixedDt;
    }

    const float frameDt = GetFrameTime();

    // Hero respawn after a short delay.
    if (ongoing && hero_ && !hero_->alive) {
        respawnTimer_ += frameDt;
        if (respawnTimer_ > 5.0f) {
            hero_->alive = true;
            hero_->hp = hero_->maxHp;
            hero_->pos = blueSpawn_;
            hero_->hasTarget = false;
            hero_->deadTimer = 0.0f;
            respawnTimer_ = 0.0f;
        }
    } else {
        respawnTimer_ = 0.0f;
    }

    if (!match_->world.hitEvents.empty()) {
        for (const auto& ev : match_->world.hitEvents) effects_.SpawnDamage(ev.pos, ev.amount);
        audio_.PlayHit();
        match_->world.hitEvents.clear();
    }

    effects_.Update(frameDt);
    views_.Sync(match_->world, assets_, frameDt);
    if (hero_) camera_.Follow(hero_->pos, kWorldW, kWorldH);
    audio_.Update();

    const int res = (match_->result == shared::Match::Result::Victory) ? 1
                  : (match_->result == shared::Match::Result::Defeat)  ? 2 : 0;
    renderer_.Draw(match_->world, assets_, views_, camera_, hero_, effects_, res, shop_);
}
