#pragma once
#include "shared/Entity.h"

namespace shared {

// The team's core. Passive; destroying the enemy core ends the match.
class Nexus : public Entity {
public:
    Nexus() { radius = 70.0f; }

    EntityType Type() const override { return EntityType::Nexus; }
    // No Update: it just sits there and can be destroyed.
};

} // namespace shared
