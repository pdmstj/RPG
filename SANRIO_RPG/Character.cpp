#include <iostream>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <windows.h> // Sleep 사용
#include <string>
#include <algorithm> // std::min 사용
#include <thread>
#include <sstream>
#include <limits>
#include <vector>
#define GAMECONSTANTS_H

using namespace std;

string ascii_art;

// 맵 크기
const int MAP_WIDTH = 20;
const int MAP_HEIGHT = 10;

// 맵 배열
char map[MAP_HEIGHT][MAP_WIDTH];

// 플레이어 위치를 저장하는 구조체
struct Position {
    int x;
    int y;
};

// 플레이어와 몬스터 및 아이템 정보
Position player = { MAP_WIDTH / 2, MAP_HEIGHT / 2 }; // 초기 위치
vector<Position> monsters;
vector<Position> items;

// 한 글자씩 천천히 출력하는 중앙 정렬 함수
void printCenteredSlowly(const string& message) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns;

    // 콘솔 크기 가져오기
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }
    else {
        columns = 80; // 기본값
    }

    // 중앙 정렬을 위한 패딩 계산
    int padding = (columns - message.length()) / 2;
    if (padding < 0) padding = 0; // 메시지가 콘솔보다 길 경우

    // 패딩 출력
    std::cout << string(padding, ' ');

    // 한 글자씩 출력
    for (char ch : message) {
        std::cout << ch << flush;
        this_thread::sleep_for(chrono::milliseconds(50)); // 50ms 지연을 줘서 한 글자씩 출력되도록 함
    }
    std::cout << endl;
}

// 출력물 중앙 정렬 함수 정의
void printCentered(const string& message) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns;

    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }
    else {
        columns = 80; // 기본값
    }

    int padding = (columns - message.length()) / 2;
    if (padding < 0) padding = 0; // 메시지가 콘솔보다 길 경우

    std::cout << string(padding, ' ') << message << endl;
}

// 입력 중앙 정렬 함수 정의
string getCenteredInput(int input_length = 20) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns;

    // 콘솔 크기 가져오기
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }
    else {
        columns = 80; // 기본값
    }

    // 입력을 위한 수평 중앙 정렬 패딩 계산
    int padding = (columns - input_length) / 2; // 입력 길이를 기준으로 중앙 정렬 패딩 계산
    if (padding < 0) padding = 0;

    // 패딩 출력
    std::cout << string(padding, ' ');

    string input;
    getline(cin, input); // 사용자 입력 받기
    return input;
}

void printEmptyLines(int count) {
    for (int i = 0; i < count; ++i) {
        std::cout << endl;
    }
}

void printCenteredASCII(const string& ascii_art) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns;

    // 콘솔 창 크기 가져오기
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }
    else {
        columns = 80; // 기본값
    }

    // ASCII 아트를 줄 단위로 중앙 정렬
    istringstream artStream(ascii_art);
    string line;
    while (getline(artStream, line)) {
        int padding = (columns - line.length()) / 2;
        if (padding < 0) padding = 0;
        std::cout << string(padding, ' ') << line << endl;
    }
}

void printCenteredArt(const std::string& art, int offset) {
    int width = 100; // 터미널 너비에 맞게 수정
    int artWidth = 5; // ASCII 아트의 너비 (적당히 추정)
    int padding = (width - artWidth) / 2 + offset;

    // 패딩이 음수가 되지 않도록 조정
    if (padding < 0) {
        padding = 0;
    }

    std::istringstream artStream(art);
    std::string line;
    while (std::getline(artStream, line)) {
        std::cout << std::string(padding, ' ') << line << std::endl;
    }
}

void waitForEnter() {
    // 콘솔 창의 크기를 가져옵니다.
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns;

    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }
    else {
        columns = 80; // 기본값
    }

    // 메시지
    string message = "[Enter를 누르면 계속 진행합니다...]";

    // 중앙 정렬을 위한 패딩 계산
    int padding = (columns - message.length()) / 2;
    if (padding < 0) padding = 0;

    // 중앙 정렬 출력
    std::cout << string(padding, ' ') << message << flush;

    // Enter 키를 기다립니다.
    while (true) {
        if (_kbhit() && _getch() == '\r') { // Enter 키 감지
            break;
        }
    }

    std::cout << endl;
}

// 콘솔 창을 최대화하는 함수 정의
void maximizeConsoleWindow() {
    HWND consoleWindow = GetConsoleWindow();
    ShowWindow(consoleWindow, SW_MAXIMIZE);
}


// 화면 초기화 함수 (Windows와 Linux 모두 지원)
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// 맵 초기화 함수
void initializeMap() {
    srand(static_cast<unsigned>(time(0))); // 랜덤 시드 설정

    // 맵을 빈 칸으로 초기화
    for (int i = 0; i < MAP_HEIGHT; ++i) {
        for (int j = 0; j < MAP_WIDTH; ++j) {
            map[i][j] = ' ';
        }
    }

    // 플레이어 위치 설정
    map[player.y][player.x] = 'o';

    // 몬스터와 아이템 생성
    for (int i = 0; i < 5; ++i) {
        Position monster = { rand() % MAP_WIDTH, rand() % MAP_HEIGHT };
        monsters.push_back(monster);
        map[monster.y][monster.x] = 'M';

        Position item = { rand() % MAP_WIDTH, rand() % MAP_HEIGHT };
        items.push_back(item);
        map[item.y][item.x] = 'I';
    }
}

// 커서 위치 이동 함수
void moveCursorTo(int x, int y) {
    std::cout << "\033[" << y + 1 << ";" << x + 1 << "H";
}

// 맵 출력 함수 (버퍼 사용)
void displayMapBuffer() {
    // 버퍼에 모든 내용을 저장
    std::string buffer;

    for (int i = 0; i < MAP_HEIGHT; ++i) {
        for (int j = 0; j < MAP_WIDTH; ++j) {
            buffer += map[i][j];
        }
        buffer += '\n';
    }

    // 안내 메시지도 버퍼에 추가
    buffer += "\n'M' - 몬스터, 'I' - 아이템, 'o' - 플레이어";
    buffer += "\n화살표 키로 이동하세요. ESC로 종료.";

    // 버퍼를 한 번에 화면에 출력
    clearScreen();
    std::cout << buffer;
}

// 플레이어 이동 함수
void movePlayer(int dx, int dy) {
    // 새로운 위치 계산
    int newX = player.x + dx;
    int newY = player.y + dy;

    // 맵 경계 체크
    if (newX < 0 || newX >= MAP_WIDTH || newY < 0 || newY >= MAP_HEIGHT) {
        return; // 경계를 벗어나면 무시
    }

    // 현재 위치를 빈 칸으로 갱신하고 해당 위치에 커서 이동 후 출력
    map[player.y][player.x] = ' ';
    moveCursorTo(player.x, player.y);
    std::cout << ' ';

    // 새로운 위치에 대한 업데이트
    char targetCell = map[newY][newX];
    if (targetCell == 'M') {
        std::cout << "몬스터를 만났습니다!" << std::endl;
        // 몬스터를 만났을 때의 추가 로직을 여기에 추가 가능
    }
    else if (targetCell == 'I') {
        std::cout << "아이템을 획득했습니다!" << std::endl;
        // 아이템을 획득했을 때의 추가 로직을 여기에 추가 가능
    }

    // 플레이어 위치 갱신하고 해당 위치에 커서 이동 후 출력
    player.x = newX;
    player.y = newY;
    map[player.y][player.x] = 'o';
    moveCursorTo(player.x, player.y);
    std::cout << 'o';
}



class Character // 모험가의 상태와 동작 관리
{
    string name;
    int level = 1;
    int item_num = 0;
    int hp = 100;
    int max_hp = 100;
    int hp_item = 5;
    int attack_power = 10;
    int defense = 5;
    int mon_hp = 100;

public:
    Character(string name) // 생성자
    {
        this->name = name;

        // 출력할 메시지
        string msg = "▶ 모험가 닉네임은 " + name + "입니다.";
        printCentered(msg);
    }

    void setName(string name)
    {
        this->name = name;

        // 출력할 메시지
        printCentered("");
        printCentered("");
        string msg = "▶ 모험가 닉네임은 " + this->name + "입니다.";
        printCentered("");
        printCentered(msg);
    }

    void level_up()
    {
        level++;
        max_hp += 20;
        attack_power += 5;
        defense += 3;
        hp = max_hp;
        printCentered("\n▶ 모험가의 레벨이 증가했습니다.");
        printCentered("▶ 현재 모험가 level : " + to_string(level) + " (HP: " + to_string(max_hp) +
            ", 공격력: " + to_string(attack_power) + ", 방어력: " + to_string(defense) + ")");
    }

    void item_up()
    {
        item_num++;
        printCentered("\n▶ 아이템을 획득했습니다.");
        printCentered("▶ 현재 아이템 개수 : " + to_string(item_num));
    }

    void item_down()
    {
        if (item_num < 1)
        {
            printCentered("\n▶ 아이템이 없습니다.");
            return;
        }
        item_num--;
        printCentered("\n▶ 아이템을 사용하여 시야를 밝힙니다.");
        printCentered("▶ 현재 아이템 개수 : " + to_string(item_num));
    }

    void hp_up()
    {
        if (hp_item < 1)
        {
            printCentered("\n▶ 포션이 없습니다.");
            return;
        }
        hp = min(hp + 30, max_hp);
        hp_item--;
        printCentered("\n▶ 포션을 사용하여 체력을 회복했습니다.");
        printCentered("▶ 현재 HP : " + to_string(hp) + " / " + to_string(max_hp));
        printCentered("▶ 남은 포션 갯수 : " + to_string(hp_item));
    }

    void info()
    {
        printCentered("\n< 상태 정보 >");
        printCentered("▶ 이름 : " + name);
        printCentered("▶ 레벨 : " + to_string(level));
        printCentered("▶ HP : " + to_string(hp) + " / " + to_string(max_hp));
        printCentered("▶ 공격력 : " + to_string(attack_power));
        printCentered("▶ 방어력 : " + to_string(defense));
        printCentered("▶ 아이템 : " + to_string(item_num));
        printCentered("▶ 포션 : " + to_string(hp_item));
    }

    void victory_pose()
    {
        printCentered("\n▶ 모험가가 승리 포즈를 취합니다! \"이겼다!\"");
        string animation[3] = { "\(^o^)/", "<(￣︶￣)>", "\(^-^)/" };
        for (int i = 0; i < 3; i++) {
            printCentered(animation[i]);
            Sleep(500);
        }
    }

    void take_damage_animation()
    {
        printCentered("\n▶ 모험가가 공격을 받고 흔들립니다!");
        string animation[3] = { "[\\^-^\\]", "[X_X]", "[\\^-^\\]" };
        for (int i = 0; i < 3; i++) {
            printCentered(animation[i]);
            Sleep(300);
        }
    }

    void use_potion_animation()
    {
        printCentered("\n▶ 포션을 사용합니다! 체력이 회복됩니다.");
        string animation[3] = { "[HP++]", "[HP+++++]", "[FULL_HP!]" };
        for (int i = 0; i < 3; i++) {
            printCentered(animation[i]);
            Sleep(300);
        }
    }

    int attack()
    {
        int mon_hp = 50 + (rand() % 50); // 몬스터 체력 랜덤 설정
        printCentered("\n▶ 몬스터를 발견했습니다!");
        while (true)
        {
            int action;
            printCentered("\n▶ 몬스터와 만났습니다. 무엇을 하시겠습니까? (1: 공격하기, 2: 도망가기) : ");
            cin >> action;

            if (action == 1)
            {
                printCentered("\n▶ 몬스터를 공격합니다.");
                int damage_to_mon = attack_power + (rand() % 5);
                mon_hp -= damage_to_mon;
                if (mon_hp < 0) mon_hp = 0;
                printCentered("▶ 몬스터에게 " + to_string(damage_to_mon) + "의 피해를 입혔습니다.");
                printCentered("▶ 남은 몬스터 체력 : " + to_string(mon_hp));

                if (mon_hp > 0)
                {
                    printCentered("\n▶ 몬스터가 반격합니다!");
                    int damage_to_char = (rand() % 15) - defense;
                    if (damage_to_char < 0) damage_to_char = 0;
                    hp -= damage_to_char;
                    if (hp < 0) hp = 0;
                    take_damage_animation();
                    printCentered("▶ 받은 피해 : " + to_string(damage_to_char));
                    printCentered("▶ 남은 체력 : " + to_string(hp) + " / " + to_string(max_hp));
                }
            }
            else if (action == 2)
            {
                if (rand() % 2 == 0)
                {
                    printCentered("\n▶ 도망에 성공했습니다!");
                    return 1;
                }
                else
                {
                    printCentered("\n▶ 도망에 실패했습니다! 몬스터가 공격합니다!");
                    int damage_to_char = (rand() % 15) - defense;
                    if (damage_to_char < 0) damage_to_char = 0;
                    hp -= damage_to_char;
                    if (hp < 0) hp = 0;
                    take_damage_animation();
                    printCentered("▶ 받은 피해 : " + to_string(damage_to_char));
                    printCentered("▶ 남은 체력 : " + to_string(hp) + " / " + to_string(max_hp));
                }
            }
            else
            {
                printCentered("\n▶ 잘못된 입력입니다. 다시 선택해주세요.");
            }

            if (mon_hp == 0)
            {
                printCentered("\n▶ 몬스터를 처치했습니다!");
                victory_pose();
                level_up();
                return 1;
            }
            if (hp == 0)
            {
                printCentered("\n▶ 모험가가 쓰러졌습니다. 게임 오버.");
                return 0;
            }
        }
    }
};

void displayEnvironment(string description)
{
    system("cls");
    printCentered("================================================================================================");
    std::cout << description << endl;
    printCentered("================================================================================================");
}

// 점 애니메이션과 중앙 정렬된 메시지를 출력하는 함수
void showProgress(string message) {
    system("cls");

    // 중앙 정렬된 메시지 출력
    printCentered(message);

    // 점 애니메이션
    for (int i = 0; i < 3; i++) {
        std::cout << "." << flush;
        Sleep(500); // 점 출력 후 500ms 대기
    }
    std::cout << endl; // 줄바꿈
}

// 캐릭터 이동 애니메이션 함수
void animateCharacterMovement() {
    string animation[5] = { "^", "-^", "^-", "^", "-^" };

    for (int i = 0; i < 5; i++) {
        // 중앙 정렬된 애니메이션 출력
        printCentered(animation[i]);
        Sleep(300); // 각 프레임 출력 후 300ms 대기
    }
    std::cout << endl; // 줄바꿈
}


int main() {
    maximizeConsoleWindow();
    srand(time(0)); // 랜덤 시드 설정
    int game_start;
    char key;
    bool selected = false;
    int selection = 1;

    while (!selected) {
        system("cls");

        // 로고와 타이틀 출력
        printCentered("================================================================================================");
        printCentered("                                                                                               ");
        printCentered("                                    WELCOME TO THE RPG GAME                                    ");
        printCentered("                                                                                               ");
        printCentered("================================================================================================");
        printCentered("");
        printCentered("");
        printCentered("RRRRRRRRRRRRRRRRR   PPPPPPPPPPPPPPPPP           GGGGGGGGGGGGG ");
        printCentered("R::::::::::::::::R  P::::::::::::::::P       GGG::::::::::::G");
        printCentered("R::::::RRRRRR:::::R P::::::PPPPPP:::::P    GG:::::::::::::::G");
        printCentered("RR:::::R     R:::::RPP:::::P     P:::::P  G:::::GGGGGGGG::::G");
        printCentered("  R::::R     R:::::R  P::::P     P:::::P G:::::G       GGGGGG");
        printCentered("  R::::R     R:::::R  P::::P     P:::::PG:::::G              ");
        printCentered("  R::::RRRRRR:::::R   P::::PPPPPP:::::P G:::::G              ");
        printCentered("  R:::::::::::::RR    P:::::::::::::PP  G:::::G    GGGGGGGGGG");
        printCentered("  R::::RRRRRR:::::R   P::::PPPPPPPPP    G:::::G    G::::::::G");
        printCentered("  R::::R     R:::::R  P::::P            G:::::G    GGGGG::::G");
        printCentered("  R::::R     R:::::R  P::::P            G:::::G        G::::G");
        printCentered("  R::::R     R:::::R  P::::P             G:::::G       G::::G");
        printCentered("RR:::::R     R:::::RPP::::::PP            G:::::GGGGGGGG::::G");
        printCentered("R::::::R     R:::::RP::::::::P             GG:::::::::::::::G");
        printCentered("R::::::R     R:::::RP::::::::P               GGG::::::GGG:::G");
        printCentered("RRRRRRRR     RRRRRRRPPPPPPPPPP                  GGGGGG   GGGG");
        printCentered("");
        printCentered("");
        printCentered("================================================================================================");


        // 선택지 출력
        string startText = (selection == 1 ? "▶ " : "  ") + string("시작하기");
        string exitText = (selection == 2 ? "▶ " : "  ") + string("종료하기");

        printCentered(startText);
        printCentered(exitText);
        printCentered("================================================================================================");

        // 사용자 입력 처리
        key = _getch();
        if (key == 80) { // 아래 화살표
            selection++;
            if (selection > 2) selection = 1;
        }
        else if (key == 72) { // 위 화살표
            selection--;
            if (selection < 1) selection = 2;
        }
        else if (key == 13) { // Enter 키
            selected = true;
        }
    }

    if (selection == 2)
    {
        std::cout << "▶ 게임이 종료되었습니다." << endl;
        return 0;
    }

    // 게임 시작 선택 시 스토리 출력
    system("cls");

    // 구분선 및 스킵 안내 출력 (중앙 정렬)
    printCentered("================================================================================================");
    printCentered("");
    printCentered("스토리를 읽는 도중 Enter 키를 누르면 바로 스킵됩니다.");
    printCentered("");
    printCentered("================================================================================================");

    string story[] = {
     "  ",
     "  ",
     "옛날, 신들이 세상을 창조하고 인간들에게 축복을 내리던 시절이 있었습니다.",
     "하지만 인간의 탐욕은 끝이 없었고, 그로 인해 신들은 분노하여 세상에서 물러나게 되었습니다.",
     "신들의 축복이 사라지자, 어둠의 세력이 세상을 집어삼키기 시작했고, 괴물들이 인간들의 마을을 공격했습니다.",
     "당신이 태어난 마을, 에델하임은 이러한 혼란 속에서도 평화로운 안식처로 남아 있었습니다.",
     "그러나 최근, 어둠의 세력인 '그림자 군단'이 마을 경계에 나타나 주민들을 위협하기 시작했습니다.",
     "'용감한 모험가여, 우리의 희망은 당신에게 달려 있습니다!' 촌장의 간절한 외침이 들려옵니다.",
     "이제 당신은 마을을 구하기 위해 모험의 길을 떠나야 합니다. 하지만 이 여정은 단순한 전투만이 아닙니다.",
     "고대의 유물과 신들의 비밀을 찾아내어, 잃어버린 축복을 되찾아야 합니다.",
     "당신의 선택이 세상의 운명을 결정짓습니다. 용기와 지혜로 가득한 여정을 시작해 보세요.",
     "신비로운 동료들과 강력한 적들이 기다리고 있는 이 길에, 여정을 떠날 준비가 되셨나요?",
     "  ",
     "  ",
     "  "
    };


    bool skip = false; // 스토리 스킵 여부를 나타내는 플래그

    // 스토리 한 줄씩 출력
    for (const string& line : story) {
        if (skip) {
            // 스킵 모드가 활성화되었으면 한 번에 출력
            printCentered(line);
            continue;
        }

        // 콘솔 크기 정보 가져오기
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        int columns;

        if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
            columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        }
        else {
            columns = 80; // 기본값
        }

        // 중앙 정렬을 위한 패딩 계산
        int padding = (columns - line.length()) / 2;
        if (padding < 0) padding = 0; // 메시지가 콘솔보다 길 경우

        // 중앙 정렬 패딩 출력 및 한 글자씩 출력
        std::cout << string(padding, ' ');  // 먼저 패딩을 출력
        for (size_t i = 0; i < line.length(); ++i) {
            std::cout << line[i] << flush;
            Sleep(50);

            // Enter 키 입력 시 스토리 스킵
            if (_kbhit() && _getch() == '\r') {
                skip = true;
                std::cout << line.substr(i + 1); // 현재 줄 나머지 부분을 바로 출력
                break;
            }
        }
        std::cout << endl;
    }

    // 중앙 정렬된 구분선 출력
    printCentered("================================================================================================");

    // Enter 키 입력을 기다리기
    printCentered("계속하려면 Enter 키를 눌러주세요...");
    std::cin.ignore(); // 기존 입력 버퍼 정리
    std::cin.get();    // Enter 키 입력 대기

    // 여정을 준비하는 진행 표시
    system("cls");
    printCenteredSlowly("여정을 준비하고 있습니다. 조금만 기다려주세요 :) ...");
    Sleep(1500);
    system("cls");
    printCenteredSlowly("여정을 준비하고 있습니다. 조금만 기다려주세요 :) ...");
    Sleep(1500);

    system("cls");


    printCentered("================================================================================================");
    printCentered("");

    int terminalHeight = 20; // 터미널 높이 예시
    printEmptyLines(terminalHeight / 2 - 2);

    printCentered("모험가의 닉네임을 입력해주세요(공백 없이 작성해주세요)");
    printCentered("");
    cin.sync(); // 입력 버퍼 정리

    cin.sync(); // 입력 버퍼 정리
    std::cout << string((110 - 10) / 2, ' '); // 가로 중앙 정렬
    string name;
    cin >> name;

    Character char1(name);

    printEmptyLines(terminalHeight / 2 - 2);


    printCentered("");
    printCentered("");
    printCentered("================================================================================================");
    system("pause");
    system("cls");


    printCentered("================================================================================================");
    printCentered("");
    printCentered("");
    printCentered("RPG WORLD에 오신 것을 환영합니다!");
    printCentered("");
    printCentered("");
    printCentered("================================================================================================");
    printCentered("");
    printCentered("");
    printCentered("");
    printCentered("");
    printCentered("");
    printCentered("모험가 " + name + "님 어서오세요! 여정을 떠날 준비가 되셨나요?!?");
    printCentered("");
    printCentered("");
    printCentered("");
    printCentered("");
    printCentered("");
    printCentered("================================================================================================");

    Sleep(5000);
    system("cls");


    while (1) {
        int motion_in = 1;
        char key;
        bool selected = false;

        const int NUM_OPTIONS = 7;  // 선택지 개수 정의
        std::string options[NUM_OPTIONS] = {
            "모험가 닉네임 변경",
            "모험가 레벨업",
            "시야확장 물약 획득",
            "시야확장 물약 사용",
            "체력회복 포션 사용",
            "여정 떠나러 가기",
            "현재 상태창 보기"
        };

        while (!selected) {
            system("cls");
            printCentered("================================================================================================");
            printCentered("                                      어떤 행동을 하시겠나요?                                     ");
            printCentered("================================================================================================");

            // 모든 옵션을 출력하면서 선택된 항목에 '◀' 표시
            for (int i = 0; i < NUM_OPTIONS; i++) {
                std::string line = "  " + std::to_string(i + 1) + ". " + options[i];
                if (motion_in == i + 1) {
                    line += " ◀";
                }
                printCentered(line);
            }

            printCentered("\n                                                 (Enter: 선택, 0: 종료)");
            printCentered("================================================================================================");

            key = _getch();
            if (key == 80) { // 아래 화살표
                motion_in++;
                if (motion_in > NUM_OPTIONS) motion_in = 1;
            }
            else if (key == 72) { // 위 화살표
                motion_in--;
                if (motion_in < 1) motion_in = NUM_OPTIONS;
            }
            else if (key == 13) { // Enter 키
                selected = true;
            }
            else if (key == '0') { // 종료
                printCentered("▶ 게임이 종료되었습니다.");
                return 0;
            }
        }

        if (motion_in == 1) {
            printCentered("\n▶ 새로운 모험가의 닉네임을 입력해주세요 : ");
            std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n'); // 입력 버퍼 정리
            std::string name;
            std::getline(std::cin, name);
            char1.setName(name);
        }
        else if (motion_in == 2) {
            char1.level_up();
        }
        else if (motion_in == 3) {
            char1.item_up();
        }
        else if (motion_in == 4) {
            char1.item_down();
        }
        else if (motion_in == 5) {
            char1.use_potion_animation();
            char1.hp_up();
        }
        else if (motion_in == 6) {
            std::string location;
            const int NUM_DESTINATIONS = 5;
            std::string destinations[NUM_DESTINATIONS] = { "바닷가", "동굴", "숲", "마을", "돌아가기" };
            int currentSelection = 0; // 현재 선택된 여행지 인덱스
            bool selected = false;

            while (!selected) {
                system("cls");
                printCentered("================================================================================================");
                printCentered("           어디로 떠나시겠습니까?           ");
                printCentered("================================================================================================");

                // 여행지 출력 (현재 선택된 항목에 ◀ 표시)
                for (int i = 0; i < NUM_DESTINATIONS; ++i) {
                    std::string line = "  " + std::to_string(i + 1) + ". " + destinations[i];
                    if (i == currentSelection) {
                        line += " ◀"; // 선택된 항목 표시
                    }
                    printCentered(line);
                }

                printCentered("================================================================================================");
                printCentered("\n                                                 (Enter: 선택, 0: 종료)");

                // 키 입력 처리
                int key = _getch();
                if (key == 80) { // 아래 화살표
                    currentSelection++;
                    if (currentSelection >= NUM_DESTINATIONS) currentSelection = 0;
                }
                else if (key == 72) { // 위 화살표
                    currentSelection--;
                    if (currentSelection < 0) currentSelection = NUM_DESTINATIONS - 1;
                }
                else if (key == 13) { // Enter 키
                    location = destinations[currentSelection];
                    selected = true;
                }
                else if (key == '0') { // 종료
                    return 0;
                }
            }

            // 선택된 장소 출력
            system("cls");
            printCentered("================================================================================================");
            printCentered("           " + location + "으로 떠납니다!           ");
            printCentered("================================================================================================");
            std::cout << location << std::endl;

            system("cls");
            Sleep(1000);
            printCentered("================================================================================================");
            printCenteredSlowly(location + "(으)로 이동 중입니다... ");
            printCentered("================================================================================================");

            const std::string frames[] = {
                R"(
            O
           /|\
           / \
        )",
                R"(
           \O/
            |
           / \
        )",
                R"(
            O
           /|\
           / \
        )",
                R"(
           O
          /|\
          / \
        )"
            };

            for (int i = 0; i < 24; ++i) { // 반복 횟수 조정 가능
                system("cls");
                int offset = i * 2 - 20;

                printCenteredArt(frames[i % 4], offset);
                printCentered("================================================================================================");
                printCentered(location + "(으)로 이동 중입니다! 조금만 기다려 주세요");
                printCentered("================================================================================================");
                Sleep(300);
            }

            system("cls");
            // 여정 선택 로직...
            if (location == "바닷가") {
                std::string ascii_art = R"(

---------,..      .,,---~~~~~~~~~~~~~~~~
.----,,,,,..      ..,,,---------------~-
,---,,,,,,...    .....,,,,,,,,,-------. 
,,,,,,,,....       .............,,,,,,, 
............     .  ...   ......,..    .
,,,,,,........           ..-;:;,..,,----
:::::~~~---,,,...           , ,,~~;*****
:::::~~~~---,,,...    ....     .,,-;;!!!
::::~~~~---,,,,...    ...,,.    ..:;!!!!
~~~~~~----,,,....   .....,,,,..     ~::;
~~~~~-----,,,,............,,,-..     ,--
~~~~~------,,,,.......,,. ,,--..     ~;!
~~~~~~------,,,,.....,,,,..,,...    .;!!
~~~~~~~~~-------,.,,,,,,,,,. ..    .:!!!
~~~~~~~~~~~~-----,,,-,--,,.        ~;;;;
~::::::~~~~--------,-,--,,  .      :;;!;
:~~:~~~~~--------,-,---,.         -;;;;;
~~~~~~~~~--------,,,,,,,.  .      :;;;;;
~~~~-~~~~~--------,....          ,;;;;;;
:~~~~~~~~~~~--,--,,.... .        -;;;;;;
:::~:~::~~~--~---...             ~;;;;;;
~:::::~~~-:::~-,                -;;;;;;;
;;;:~:~-~:~~-- ..               ~;;;;;;;
;;;;~;:::-.,,.,..               -;;;;;;;
!;;;;;:-.  .,,,.                -;;;;;;;
!!;;:~.   .,,,. .               -;;;;;;;

        )";
                printCenteredASCII(ascii_art);

                Sleep(5000);
            }
            else if (location == "동굴") {
                std::string ascii_art = R"(
                               
          
                                                  
                                      
                                        
        )";
                printCenteredASCII(ascii_art);

                Sleep(5000);
            }
            else if (location == "숲") {
                std::string ascii_art = R"(
       



        )";
                printCenteredASCII(ascii_art);

                Sleep(5000);
            }
            else if (location == "마을") {
                std::string ascii_art = R"(



)";
                printCenteredASCII(ascii_art);

                Sleep(5000);
            }
            else {
                std::cout << "잘못된 입력입니다. 다시 선택해주세요." << std::endl;
                system("pause");
                continue; // continue를 사용하여 루프를 다시 시작
            }

            printCenteredSlowly(location + "에 도착하였습니다.");
            waitForEnter();
        }

        initializeMap();
        displayMapBuffer(); // 처음 한 번 전체 맵을 출력

        while (true) {
            // 키 입력 처리
            if (_kbhit()) {
                char key = _getch();

                if (key == 27) { // ESC 키
                    break;
                }
                else if (key == -32 || key == 224) { // 화살표 키 (확장 코드)
                    key = _getch(); // 방향키 확인
                    switch (key) {
                    case 72: // ↑
                        movePlayer(0, -1);
                        break;
                    case 80: // ↓
                        movePlayer(0, 1);
                        break;
                    case 75: // ←
                        movePlayer(-1, 0);
                        break;
                    case 77: // →
                        movePlayer(1, 0);
                        break;
                    }
                }

                // 몬스터와 아이템이 모두 없어지면 종료
                if (monsters.empty() && items.empty()) {
                    std::cout << "\n▶ 모든 몬스터를 처치하고 모든 아이템을 획득했습니다!" << std::endl;
                    break;
                }
            }
        }

        std::cout << "▶ 게임 종료" << std::endl;
        return 0;
    }
}
