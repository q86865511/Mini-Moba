#include "AnimatedSprite.h"

void AnimationPlayer::SetSet(const AnimationSet* set) {
    set_ = set;
    current_.clear();
    index_ = 0;
    timer_ = 0.0f;
    finished_ = false;
}

void AnimationPlayer::Play(const std::string& state, bool loop) {
    if (!set_ || !set_->Has(state)) return;
    if (state == current_) { loop_ = loop; return; }
    current_ = state;
    index_ = 0;
    timer_ = 0.0f;
    loop_ = loop;
    finished_ = false;
}

void AnimationPlayer::Update(float dt) {
    if (!set_ || finished_) return;
    auto it = set_->states.find(current_);
    if (it == set_->states.end() || it->second.empty()) return;
    const auto& frames = it->second;
    timer_ += dt;
    while (frames[index_].duration > 0.0f && timer_ >= frames[index_].duration) {
        timer_ -= frames[index_].duration;
        if (index_ + 1 < frames.size()) {
            ++index_;
        } else if (loop_) {
            index_ = 0;
        } else {
            finished_ = true;
            break;
        }
    }
}

Rectangle AnimationPlayer::Frame() const {
    if (!set_) return Rectangle{ 0, 0, 0, 0 };
    auto it = set_->states.find(current_);
    if (it == set_->states.end() || it->second.empty()) return Rectangle{ 0, 0, 0, 0 };
    return it->second[index_].src;
}
