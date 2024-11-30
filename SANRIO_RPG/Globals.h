#ifndef GLOBALS_H
#define GLOBALS_H

#include <vector>
#include <string>

// 맵 크기
const int MAP_WIDTH = 20;
const int MAP_HEIGHT = 10;

// 맵 배열
extern char map[MAP_HEIGHT][MAP_WIDTH];

// 플레이어 위치를 저장하는 구조체
struct Position {
    int x;
    int y;
};

// 전역 변수
extern Position player;
extern std::vector<Position> monsters;
extern std::vector<Position> items;

#endif // GLOBALS_H
