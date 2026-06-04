#pragma once
#include "raylib.h"
#include "shared/Vec2.h"
#include <vector>
#include <string>

// Transient visual feedback: floating damage numbers + hit rings (world space).
class Effects {
public:
    void SpawnDamage(shared::Vec2 pos, float amount);
    void Update(float dt);
    void DrawWorld() const; // call inside BeginMode2D(camera)

private:
    struct Floater { shared::Vec2 pos; std::string text; float age; float life; };
    struct Ring    { shared::Vec2 pos; float age; float life; float maxR; };
    std::vector<Floater> floaters_;
    std::vector<Ring>    rings_;
};
