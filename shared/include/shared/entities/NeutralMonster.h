#pragma once
#include "shared/Entity.h"

namespace shared {

// A jungle monster: guards its camp, attacks enemies that come into range.
class NeutralMonster : public Entity {
public:
    NeutralMonster() { team = Team::Neutral; radius = 36.0f; }

    EntityType Type() const override { return EntityType::NeutralMonster; }
    void Update(World& world, float dt) override;
};

} // namespace shared
