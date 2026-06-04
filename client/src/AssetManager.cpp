#include "AssetManager.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <vector>

// Path next to the running exe (assets are copied there at build time).
static std::string AssetPath(const std::string& rel) {
    std::string dir = GetApplicationDirectory();
    if (!dir.empty() && dir.back() != '/' && dir.back() != '\\') dir += '/';
    return dir + rel;
}

// Parse a pack *_actions.json into an AnimationSet (states -> frames + anchor).
static AnimationSet LoadAnimationSet(const std::string& jsonPath) {
    AnimationSet set;
    std::ifstream f(jsonPath);
    if (!f.is_open()) return set;

    nlohmann::json j;
    try { f >> j; } catch (...) { return set; }
    if (!j.contains("states")) return set;

    bool gotAnchor = false;
    for (auto& item : j["states"].items()) {
        const std::string& name = item.key();
        std::vector<AnimFrame> frames;
        for (auto& fr : item.value()) {
            const auto& r = fr["rect"];
            const float dur = fr.contains("duration_ms") ? fr["duration_ms"].get<float>() / 1000.0f : 0.12f;
            frames.push_back({ Rectangle{ r["x"].get<float>(), r["y"].get<float>(),
                                          r["w"].get<float>(), r["h"].get<float>() }, dur });
            if (!gotAnchor && fr.contains("anchor_px")) {
                set.anchor = { fr["anchor_px"][0].get<float>(), fr["anchor_px"][1].get<float>() };
                gotAnchor = true;
            }
        }
        set.states[name] = std::move(frames);
    }
    return set;
}

namespace {
struct SheetDef { const char* key; const char* cat; const char* slug; };
const SheetDef kDefs[] = {
    { "ember_vanguard", "heroes", "ember_vanguard" },
    { "frost_ranger",   "heroes", "frost_ranger" },
    { "storm_oracle",   "heroes", "storm_oracle" },
    { "iron_sentinel",  "heroes", "iron_sentinel" },
    { "night_veil",     "heroes", "night_veil" },
    { "blue_melee_minion",  "minions", "blue_melee_minion" },
    { "blue_ranged_minion", "minions", "blue_ranged_minion" },
    { "blue_siege_minion",  "minions", "blue_siege_minion" },
    { "red_melee_minion",   "minions", "red_melee_minion" },
    { "red_ranged_minion",  "minions", "red_ranged_minion" },
    { "red_siege_minion",   "minions", "red_siege_minion" },
    { "stone_golem",    "monsters", "stone_golem" },
    { "bramble_spirit", "monsters", "bramble_spirit" },
    { "river_drake",    "monsters", "river_drake" },
    { "blue_tower", "tower", "blue_tower" },
    { "red_tower",  "tower", "red_tower" },
    { "blue_core",  "core",  "blue_core" },
    { "red_core",   "core",  "red_core" },
};
} // namespace

void AssetManager::Load() {
    const std::string mapPath = AssetPath("assets/moba_asset_pack/map/moba_three_lane_map.png");
    if (FileExists(mapPath.c_str())) mapTex_ = LoadTexture(mapPath.c_str());
    if (mapTex_.id == 0) {
        Image im = GenImageChecked(128, 128, 2, 2, Color{ 54, 84, 54, 255 }, Color{ 48, 76, 48, 255 });
        mapTex_ = LoadTextureFromImage(im);
        UnloadImage(im);
    }
    SetTextureFilter(mapTex_, TEXTURE_FILTER_BILINEAR);

    for (const auto& d : kDefs) {
        const std::string base =
            std::string("assets/moba_interaction_pack/sprites/") + d.cat + "/" + d.slug + "/" + d.slug;
        const std::string sheetPath = AssetPath(base + "_sprite_sheet.png");
        const std::string actionsPath = AssetPath(base + "_actions.json");

        if (FileExists(sheetPath.c_str())) {
            Texture2D t = LoadTexture(sheetPath.c_str());
            if (t.id != 0) {
                SetTextureFilter(t, TEXTURE_FILTER_BILINEAR);
                sheets_[d.key] = t;
            }
        }
        AnimationSet set = LoadAnimationSet(actionsPath);
        if (!set.states.empty()) anims_[d.key] = std::move(set);
    }

    font_ = GetFontDefault();
    loaded_ = true;
}

Texture2D AssetManager::Sheet(const std::string& key) const {
    auto it = sheets_.find(key);
    return it != sheets_.end() ? it->second : Texture2D{};
}

const AnimationSet* AssetManager::Anim(const std::string& key) const {
    auto it = anims_.find(key);
    return it != anims_.end() ? &it->second : nullptr;
}

void AssetManager::Unload() {
    if (!loaded_) return;
    UnloadTexture(mapTex_);
    for (auto& kv : sheets_) UnloadTexture(kv.second);
    sheets_.clear();
    anims_.clear();
    loaded_ = false;
}
