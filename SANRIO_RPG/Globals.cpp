#include "Globals.h"

// ���� ���� ����
char map[MAP_HEIGHT][MAP_WIDTH];
Position player = { MAP_WIDTH / 2, MAP_HEIGHT / 2 }; // �ʱ� ��ġ
std::vector<Position> monsters;
std::vector<Position> items;
