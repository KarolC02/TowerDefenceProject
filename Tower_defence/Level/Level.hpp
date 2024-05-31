#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <vector>
#include <SFML/System/Vector2.hpp>

class Level {
public:
    int levelNumber;
    float maxDuration;
    int numberOfEnemies;
    bool group;
    bool immune;
    bool fast;
    bool flying;
    bool spawn;
    int health;
    int value;
    // Constructor
    Level(int lvlNumber, float maxDur, int numEnemies, bool grp, int hp, bool imm, bool fst, bool fly, bool spn, int val)
        : levelNumber(lvlNumber), maxDuration(maxDur), numberOfEnemies(numEnemies), group(grp), health(hp), immune(imm), fast(fst), flying(fly), spawn(spn), value(val) {}
    
    Level()
        : levelNumber(-1), maxDuration(1000), numberOfEnemies(0), group(false), health(0), immune(false), fast(false), flying(false), spawn(false), value(0) {}
};

#endif // LEVEL_HPP
