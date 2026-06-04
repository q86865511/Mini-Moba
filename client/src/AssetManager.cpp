#include "AssetManager.h"

// A round "champion token": opaque circle (outline + body + highlight) on a
// transparent background.
static Texture2D MakeHeroToken(int size, Color body, Color outline, Color highlight) {
    Image img = GenImageColor(size, size, BLANK);
    const int c = size / 2;
    const int r = size / 2 - 2;
    ImageDrawCircle(&img, c, c, r, outline);
    ImageDrawCircle(&img, c, c, r - 3, body);
    ImageDrawCircle(&img, c - r / 3, c - r / 3, r / 5, highlight);
    Texture2D tex = LoadTextureFromImage(img);
    UnloadImage(img);
    return tex;
}

// A subtle two-tone "grass" checker that tiles seamlessly.
static Texture2D MakeGroundTile() {
    Image img = GenImageChecked(128, 128, 2, 2,
                                Color{ 54, 84, 54, 255 }, Color{ 48, 76, 48, 255 });
    Texture2D tex = LoadTextureFromImage(img);
    UnloadImage(img);
    return tex;
}

void AssetManager::Load() {
    heroTex_ = MakeHeroToken(40,
                             Color{ 70, 140, 255, 255 },   // body (blue team)
                             Color{ 20, 40, 90, 255 },     // outline
                             Color{ 150, 190, 255, 255 }); // highlight
    groundTex_ = MakeGroundTile();
    font_ = GetFontDefault();
    loaded_ = true;
}

void AssetManager::Unload() {
    if (!loaded_) return;
    UnloadTexture(heroTex_);
    UnloadTexture(groundTex_);
    // font_ is raylib's built-in default font; do NOT unload it.
    loaded_ = false;
}
