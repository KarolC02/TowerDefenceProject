#pragma once

struct TowerUpgrade {
    std::string name;
    float attackSpeed;
    int range;
    int damage;
    int cost;

    TowerUpgrade(const std::string& n, float spd, int rng, int dmg, int cst)
        : name(n), attackSpeed(spd), range(rng), damage(dmg), cost(cst) {}
};
