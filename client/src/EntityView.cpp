#include "EntityView.h"
#include "AssetManager.h"

void ViewRegistry::Sync(const shared::World& world, const AssetManager& assets, float dt) {
    std::unordered_map<int, char> present;

    for (auto& e : world.entities) {
        if (!e) continue;
        present[e->id] = 1;

        auto it = views_.find(e->id);
        if (it == views_.end()) {
            EntityView v;
            v.key = e->visualKey;
            v.player.SetSet(assets.Anim(v.key));
            it = views_.emplace(e->id, std::move(v)).first;
        }

        EntityView& view = it->second;
        const AnimationSet* set = view.player.Set();

        std::string st;
        if (!e->alive)                                                 st = "death";
        else if (e->attackAnimTime > 0.0f && set && set->Has("attack")) st = "attack";
        else if (e->hurtAnimTime > 0.0f && set && set->Has("hurt"))    st = "hurt";
        else if (e->moving && set && set->Has("move"))                st = "move";
        else                                                          st = "idle";
        if (set && !set->Has(st) && set->Has("idle")) st = "idle";

        view.player.Play(st, /*loop=*/st != "death");
        view.player.Update(dt);
    }

    for (auto it = views_.begin(); it != views_.end();) {
        if (!present.count(it->first)) it = views_.erase(it);
        else ++it;
    }
}

const EntityView* ViewRegistry::Get(int id) const {
    auto it = views_.find(id);
    return it != views_.end() ? &it->second : nullptr;
}
