#include "Globals.h"
#include "Utilities.h"
#include "Character.h"
#include "Map.h"

using namespace std;

int main() {
    maximizeConsoleWindow();
    initializeMap();

    Character hero("Player");
    hero.info();

    while (true) {
        displayMap();
        // 게임 루프...
    }

    return 0;
}
