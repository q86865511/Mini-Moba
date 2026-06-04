#pragma once
#include "raylib.h"
#include <vector>
#include <string>
#include <unordered_map>

// One frame: a source rectangle in a sprite sheet + how long to show it (seconds).
struct AnimFrame {
    Rectangle src;
    float     duration;
};

// Immutable animation data for one sprite sheet (state name -> frames).
// Owned once by the AssetManager and shared by every instance of that sheet.
struct AnimationSet {
    std::unordered_map<std::string, std::vector<AnimFrame>> states;
    Vector2 anchor{ 128.0f, 205.0f }; // anchor within a frame, in sheet pixels (feet)

    bool Has(const std::string& s) const { return states.find(s) != states.end(); }
};

// Per-instance playback over an AnimationSet.
class AnimationPlayer {
public:
    void SetSet(const AnimationSet* set);
    void Play(const std::string& state, bool loop = true);
    void Update(float dt);
    Rectangle Frame() const;

    const AnimationSet* Set() const { return set_; }
    const std::string& State() const { return current_; }
    bool Finished() const { return finished_; }

private:
    const AnimationSet* set_ = nullptr;
    std::string current_;
    size_t index_ = 0;
    float  timer_ = 0.0f;
    bool   loop_ = true;
    bool   finished_ = false;
};
