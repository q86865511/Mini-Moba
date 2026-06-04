#include "AnimatedSprite.h"

void AnimatedSprite::AddState(const std::string& name, const std::vector<AnimFrame>& frames) {
    states_[name] = frames;
    if (current_.empty()) { current_ = name; index_ = 0; timer_ = 0.0f; }
}

bool AnimatedSprite::HasState(const std::string& name) const {
    return states_.find(name) != states_.end();
}

void AnimatedSprite::Play(const std::string& state) {
    if (state == current_ || !HasState(state)) return;
    current_ = state;
    index_ = 0;
    timer_ = 0.0f;
}

void AnimatedSprite::Update(float dt) {
    auto it = states_.find(current_);
    if (it == states_.end() || it->second.empty()) return;
    const auto& frames = it->second;
    timer_ += dt;
    while (timer_ >= frames[index_].duration && frames[index_].duration > 0.0f) {
        timer_ -= frames[index_].duration;
        index_ = (index_ + 1) % frames.size();
    }
}

Rectangle AnimatedSprite::Frame() const {
    auto it = states_.find(current_);
    if (it == states_.end() || it->second.empty()) return Rectangle{ 0, 0, 0, 0 };
    return it->second[index_].src;
}
