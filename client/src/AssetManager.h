#pragma once
#include "raylib.h"

// Owns the game's visual assets. In phase 2 every texture is generated in code
// (procedural / "Claude-drawn"); later phases can swap in downloaded sprites.
// Must be Load()ed after InitWindow() (needs a GL context) and Unload()ed before close.
class AssetManager {
public:
    void Load();
    void Unload();

    Texture2D Hero()   const { return heroTex_; }
    Texture2D Ground() const { return groundTex_; }
    Font      UiFont() const { return font_; }

private:
    Texture2D heroTex_{};
    Texture2D groundTex_{};
    Font      font_{};
    bool      loaded_ = false;
};
