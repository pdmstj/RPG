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
        // ���� ����...
    }

    return 0;
}
