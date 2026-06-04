#pragma once
#include "shared/Vec2.h"
#include "shared/Team.h"
#include <vector>
#include <string>

namespace shared {

// Plain map layout data (parsed by the client from interactive_map_zones.json,
// then handed to Match). No file IO / JSON here -> shared stays dependency-free.
struct LanePath  { std::vector<Vec2> points; };          // ordered blue-base -> red-base
struct TowerSite { Vec2 pos; Team team; };
struct BaseSite  { Vec2 corePos; Vec2 spawnPos; Team team; };
struct CampSite  { Vec2 pos; std::string monster; };     // jungle camp

struct MapData {
    std::vector<LanePath>  lanes;
    std::vector<TowerSite> towers;
    std::vector<BaseSite>  bases;
    std::vector<CampSite>  camps;
};

} // namespace shared
