#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>

class Character {
    std::string name;
    int level;
    int item_num;
    int hp, max_hp;
    int hp_item;
    int attack_power;
    int defense;

public:
    Character(std::string name);
    void setName(std::string name);
    void level_up();
    void item_up();
    void item_down();
    void hp_up();
    void info();
    void victory_pose();
    int attack();
};

#endif // CHARACTER_H
