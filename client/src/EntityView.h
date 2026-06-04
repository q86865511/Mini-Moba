#pragma once
#include "AnimatedSprite.h"
#include "shared/World.h"
#include <unordered_map>
#include <string>

class AssetManager;

// Client-side animation state for one entity instance.
struct EntityView {
    std::string     key;     // sprite-sheet key (== entity.visualKey)
    AnimationPlayer player;
};

// Keeps one EntityView per entity id; chooses the animation state each frame
// from the entity's logic flags (alive / moving / attackAnimTime).
class ViewRegistry {
public:
    void Sync(const shared::World& world, const AssetManager& assets, float dt);
    const EntityView* Get(int id) const;

private:
    std::unordered_map<int, EntityView> views_;
};
