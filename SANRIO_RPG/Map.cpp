#include "Map.h"
#include "Globals.h"
#include "Utilities.h"
#include <iostream>
#include <algorithm>

using namespace std;

void initializeMap() {
    srand(static_cast<unsigned>(time(0)));

    for (int i = 0; i < MAP_HEIGHT; ++i) {
        for (int j = 0; j < MAP_WIDTH; ++j) {
            map[i][j] = ' ';
        }
    }

    map[player.y][player.x] = 'o';

    for (int i = 0; i < 5; ++i) {
        Position monster = { rand() % MAP_WIDTH, rand() % MAP_HEIGHT };
        monsters.push_back(monster);
        map[monster.y][monster.x] = 'M';

        Position item = { rand() % MAP_WIDTH, rand() % MAP_HEIGHT };
        items.push_back(item);
        map[item.y][item.x] = 'I';
    }
}

void displayMap() {
    for (int i = 0; i < MAP_HEIGHT; ++i) {
        for (int j = 0; j < MAP_WIDTH; ++j) {
            cout << map[i][j];
        }
        cout << endl;
    }
    printCentered("'M' - 몬스터, 'I' - 아이템, 'o' - 플레이어");
}

void movePlayer(int dx, int dy) {
    map[player.y][player.x] = ' ';
    player.x += dx;
    player.y += dy;
    map[player.y][player.x] = 'o';
}
