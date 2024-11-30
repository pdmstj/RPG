#include "Globals.h"

// 전역 변수 정의
char map[MAP_HEIGHT][MAP_WIDTH];
Position player = { MAP_WIDTH / 2, MAP_HEIGHT / 2 }; // 초기 위치
std::vector<Position> monsters;
std::vector<Position> items;
