#pragma once
#include <cmath>

// Minimal 2D vector for the simulation. Intentionally independent of raylib so
// that `shared` stays pure logic and can be reused on the server.
namespace shared {

struct Vec2 {
    float x = 0.0f;
    float y = 0.0f;
};

inline Vec2 operator+(Vec2 a, Vec2 b) { return { a.x + b.x, a.y + b.y }; }
inline Vec2 operator-(Vec2 a, Vec2 b) { return { a.x - b.x, a.y - b.y }; }
inline Vec2 operator*(Vec2 v, float s) { return { v.x * s, v.y * s }; }

inline float Length(Vec2 v) { return std::sqrt(v.x * v.x + v.y * v.y); }

inline Vec2 Normalized(Vec2 v) {
    const float len = Length(v);
    if (len <= 1e-6f) return { 0.0f, 0.0f };
    return { v.x / len, v.y / len };
}

inline float Distance(Vec2 a, Vec2 b) { return Length(b - a); }

} // namespace shared
