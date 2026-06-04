#pragma once
#include "shared/entities/Hero.h"
#include <vector>
#include <string>

struct ShopItem { std::string name; int cost; std::string desc; };

// A minimal item shop: spend gold to permanently boost the hero.
class Shop {
public:
    Shop();
    bool open = false;
    void Toggle() { open = !open; }
    const std::vector<ShopItem>& Items() const { return items_; }
    bool Buy(int index, shared::Hero& hero); // true if purchased

private:
    std::vector<ShopItem> items_;
};
