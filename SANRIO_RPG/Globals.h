#ifndef GLOBALS_H
#define GLOBALS_H

#include <vector>
#include <string>

// �� ũ��
const int MAP_WIDTH = 20;
const int MAP_HEIGHT = 10;

// �� �迭
extern char map[MAP_HEIGHT][MAP_WIDTH];

// �÷��̾� ��ġ�� �����ϴ� ����ü
struct Position {
    int x;
    int y;
};

// ���� ����
extern Position player;
extern std::vector<Position> monsters;
extern std::vector<Position> items;

#endif // GLOBALS_H
