#pragma once
#include "shared/MapData.h"
#include <string>

// Parse interactive_map_zones.json (lanes / towers / bases) into a MapData.
shared::MapData LoadMapZones(const std::string& jsonPath);
