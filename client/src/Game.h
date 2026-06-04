#pragma once
#include "shared/Match.h"
#include "shared/entities/Hero.h"
#include "shared/MapData.h"
#include "AssetManager.h"
#include "GameCamera.h"
#include "Renderer.h"
#include "EntityView.h"
#include "Effects.h"
#include "Shop.h"
#include "Input.h"
#include "Audio.h"
#include <memory>
#include <vector>
#include <string>

// Top-level application: a small state machine (menu -> hero select -> playing)
// over the shared Match. main() just creates one and calls Run().
class Game {
public:
    Game();
    ~Game();
    void Run();

private:
    enum class State { Menu, HeroSelect, Playing };

    void UpdateMenu();
    void UpdateHeroSelect();
    void UpdatePlaying();
    void StartMatch();
    void ReturnToMenu();

    static constexpr int   kScreenW = 1280;
    static constexpr int   kScreenH = 720;
    static constexpr float kWorldW  = 2048.0f;
    static constexpr float kWorldH  = 2048.0f;

    State state_ = State::Menu;

    shared::MapData                mapData_;
    std::unique_ptr<shared::Match> match_;
    shared::Hero*                  hero_ = nullptr;
    shared::Vec2                   blueSpawn_{ 315.0f, 1695.0f };
    float                          respawnTimer_ = 0.0f;

    std::vector<std::string> heroChoices_{
        "ember_vanguard", "frost_ranger", "storm_oracle", "iron_sentinel", "night_veil"
    };
    int selectedHero_ = 0;

    AssetManager assets_;
    GameCamera   camera_{ kScreenW, kScreenH };
    Renderer     renderer_{ kScreenW, kScreenH, kWorldW, kWorldH };
    ViewRegistry views_;
    Effects      effects_;
    Shop         shop_;
    Input        input_;
    Audio        audio_;

    float accumulator_ = 0.0f;
};
