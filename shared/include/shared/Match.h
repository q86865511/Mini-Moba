#pragma once
#include "shared/World.h"
#include "shared/MapData.h"
#include "shared/entities/Hero.h"
#include <string>

namespace shared {

// Owns the single-player match: builds towers/cores from the map, spawns minion
// waves, ticks the world, and tracks the win/lose condition.
class Match {
public:
    enum class Result { Ongoing, Victory, Defeat };

    World  world;
    Result result = Result::Ongoing;

    void  Setup(const MapData& map);
    Hero* AddPlayerHero(const std::string& slug, Vec2 pos);
    void  Update(float dt);

    int   WaveNumber() const { return waveNumber_; }
    float NextWaveIn() const { return waveTimer_; }

private:
    void SpawnWave();

    MapData map_;
    float   waveTimer_ = 5.0f;
    int     waveNumber_ = 0;
    Entity* blueNexus_ = nullptr;
    Entity* redNexus_ = nullptr;
};

} // namespace shared
