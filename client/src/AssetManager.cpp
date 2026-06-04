#include "AssetManager.h"
#include <string>

// Build a path next to the running exe (assets are copied there at build time),
// so loading works regardless of the current working directory.
static std::string AssetPath(const char* rel) {
    std::string dir = GetApplicationDirectory();
    if (!dir.empty() && dir.back() != '/' && dir.back() != '\\') dir += '/';
    return dir + rel;
}

// Seamless grass fallback, only used if the real map fails to load.
static Texture2D MakeGrassFallback() {
    Image img = GenImageChecked(128, 128, 2, 2,
                                Color{ 54, 84, 54, 255 }, Color{ 48, 76, 48, 255 });
    Texture2D t = LoadTextureFromImage(img);
    UnloadImage(img);
    return t;
}

void AssetManager::Load() {
    const std::string mapPath =
        AssetPath("assets/moba_asset_pack/map/moba_three_lane_map.png");
    if (FileExists(mapPath.c_str())) mapTex_ = LoadTexture(mapPath.c_str());
    if (mapTex_.id == 0) mapTex_ = MakeGrassFallback();

    const std::string heroPath = AssetPath(
        "assets/moba_interaction_pack/sprites/heroes/ember_vanguard/ember_vanguard_sprite_sheet.png");
    heroSheet_ = LoadTexture(heroPath.c_str()); // id==0 if missing; renderer falls back to a shape

    SetTextureFilter(mapTex_, TEXTURE_FILTER_BILINEAR);
    if (heroSheet_.id != 0) SetTextureFilter(heroSheet_, TEXTURE_FILTER_BILINEAR);

    font_ = GetFontDefault();
    loaded_ = true;
}

void AssetManager::Unload() {
    if (!loaded_) return;
    UnloadTexture(mapTex_);
    if (heroSheet_.id != 0) UnloadTexture(heroSheet_);
    // font_ is raylib's built-in default font; do NOT unload it.
    loaded_ = false;
}
