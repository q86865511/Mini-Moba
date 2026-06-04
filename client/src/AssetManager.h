#pragma once
#include "raylib.h"
#include "AnimatedSprite.h"
#include <string>
#include <unordered_map>

// Owns visual assets: the map texture, every entity sprite sheet, and the
// matching animation data (parsed from each pack's *_actions.json).
class AssetManager {
public:
    void Load();
    void Unload();

    Texture2D           Map() const { return mapTex_; }
    Texture2D           Sheet(const std::string& key) const;
    const AnimationSet* Anim(const std::string& key) const;
    Font                UiFont() const { return font_; }

private:
    Texture2D mapTex_{};
    std::unordered_map<std::string, Texture2D>    sheets_;
    std::unordered_map<std::string, AnimationSet> anims_;
    Font font_{};
    bool loaded_ = false;
};
