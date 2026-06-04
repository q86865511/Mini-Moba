#include "Effects.h"
#include <algorithm>
#include <cstdio>

void Effects::SpawnDamage(shared::Vec2 pos, float amount) {
    char buf[16];
    std::snprintf(buf, sizeof(buf), "%d", (int)(amount + 0.5f));
    floaters_.push_back({ pos, std::string(buf), 0.0f, 0.8f });
    rings_.push_back({ pos, 0.0f, 0.30f, 36.0f });
}

void Effects::Update(float dt) {
    for (auto& f : floaters_) { f.age += dt; f.pos.y -= 42.0f * dt; }
    floaters_.erase(std::remove_if(floaters_.begin(), floaters_.end(),
        [](const Floater& f) { return f.age >= f.life; }), floaters_.end());

    for (auto& r : rings_) r.age += dt;
    rings_.erase(std::remove_if(rings_.begin(), rings_.end(),
        [](const Ring& r) { return r.age >= r.life; }), rings_.end());
}

void Effects::DrawWorld() const {
    for (const auto& r : rings_) {
        const float t = r.age / r.life;
        const float rad = r.maxR * t;
        const unsigned char a = (unsigned char)(200.0f * (1.0f - t));
        DrawCircleLines((int)r.pos.x, (int)r.pos.y, rad, Color{ 255, 230, 120, a });
    }
    for (const auto& f : floaters_) {
        const float t = f.age / f.life;
        const unsigned char a = (unsigned char)(255.0f * (1.0f - t));
        const int fs = 24;
        const int w = MeasureText(f.text.c_str(), fs);
        DrawText(f.text.c_str(), (int)f.pos.x - w / 2, (int)f.pos.y - 64, fs, Color{ 255, 235, 120, a });
    }
}
