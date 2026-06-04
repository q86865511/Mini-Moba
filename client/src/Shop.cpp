#include "Shop.h"

Shop::Shop() {
    items_ = {
        { "Longsword",     350, "+25 Attack Damage" },
        { "Vitality Gem",  400, "+200 Max HP" },
        { "Swift Boots",   300, "+60 Move Speed" },
        { "Power Crystal", 550, "+40 Attack, shorter Q cooldown" },
    };
}

bool Shop::Buy(int index, shared::Hero& hero) {
    if (index < 0 || index >= (int)items_.size()) return false;
    const ShopItem& it = items_[index];
    if (!hero.SpendGold(it.cost)) return false;

    if (it.name == "Longsword")          hero.attackDamage += 25.0f;
    else if (it.name == "Vitality Gem")  { hero.maxHp += 200.0f; hero.hp += 200.0f; }
    else if (it.name == "Swift Boots")   hero.moveSpeed += 60.0f;
    else if (it.name == "Power Crystal")  { hero.attackDamage += 40.0f; if (hero.qCooldown > 1.0f) hero.qCooldown -= 0.5f; }

    hero.items.push_back(it.name);
    return true;
}
