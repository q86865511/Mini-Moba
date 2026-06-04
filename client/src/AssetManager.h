#pragma once
#include "raylib.h"

// Owns the game's visual assets (loaded from assets/, which CMake copies next to
// the exe). Must be Load()ed after InitWindow() and Unload()ed before close.
class AssetManager {
public:
    void Load();
    void Unload();

    Texture2D Map()       const { return mapTex_; }      // 2048x2048 three-lane map
    Texture2D HeroSheet() const { return heroSheet_; }   // hero animation sprite sheet
    Font      UiFont()    const { return font_; }

private:
    Texture2D mapTex_{};
    Texture2D heroSheet_{};
    Font      font_{};
    bool      loaded_ = false;
};
