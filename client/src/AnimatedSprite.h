#pragma once
#include "raylib.h"
#include <vector>
#include <string>
#include <unordered_map>

// One frame of an animation: a source rectangle in a sprite sheet + how long to show it.
struct AnimFrame {
    Rectangle src;       // region within the sheet texture
    float     duration;  // seconds
};

// A simple state-based sprite animator (idle / move / attack / ...).
// It only tracks which frame to show; the texture and drawing live in the renderer.
class AnimatedSprite {
public:
    void AddState(const std::string& name, const std::vector<AnimFrame>& frames);
    void Play(const std::string& state);  // switch state (no-op if already current/unknown)
    void Update(float dt);                 // advance the current animation
    Rectangle Frame() const;               // current frame's source rectangle
    bool HasState(const std::string& name) const;

private:
    std::unordered_map<std::string, std::vector<AnimFrame>> states_;
    std::string current_;
    size_t      index_ = 0;
    float       timer_ = 0.0f;
};
