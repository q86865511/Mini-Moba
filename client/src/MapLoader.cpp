#include "MapLoader.h"
#include <nlohmann/json.hpp>
#include <fstream>

static shared::Team TeamFromStr(const std::string& s) {
    return (s == "blue") ? shared::Team::Blue : shared::Team::Red;
}

shared::MapData LoadMapZones(const std::string& path) {
    shared::MapData md;
    std::ifstream f(path);
    if (!f.is_open()) return md;

    nlohmann::json j;
    try { f >> j; } catch (...) { return md; }

    if (j.contains("lanes")) {
        for (auto& lane : j["lanes"]) {
            shared::LanePath lp;
            for (auto& pt : lane["path"])
                lp.points.push_back({ pt[0].get<float>(), pt[1].get<float>() });
            if (lp.points.size() >= 2) md.lanes.push_back(std::move(lp));
        }
    }
    if (j.contains("towers")) {
        for (auto& t : j["towers"]) {
            shared::TowerSite ts;
            ts.pos = { t["pos_px"][0].get<float>(), t["pos_px"][1].get<float>() };
            ts.team = TeamFromStr(t["team"].get<std::string>());
            md.towers.push_back(ts);
        }
    }
    if (j.contains("bases")) {
        for (auto& b : j["bases"]) {
            shared::BaseSite bs;
            bs.corePos = { b["core_px"][0].get<float>(), b["core_px"][1].get<float>() };
            bs.spawnPos = { b["spawn_px"][0].get<float>(), b["spawn_px"][1].get<float>() };
            bs.team = TeamFromStr(b["team"].get<std::string>());
            md.bases.push_back(bs);
        }
    }
    return md;
}
