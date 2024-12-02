#include <limits> // 반드시 위쪽에 배치
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <windows.h> // Sleep 사용
#include <string>
#include <algorithm> // std::min 사용
#include <thread>
#include <sstream>
#include <vector>

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

void printCentered(const string& message) {
    // 콘솔 화면 너비 계산
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns;

    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }
    else {
        columns = 80; // 기본값
    }

    // 여러 줄의 문자열을 지원하기 위해 스트림 사용
    istringstream messageStream(message);
    string line;

    while (getline(messageStream, line)) {
        // 각 줄에 대해 중앙 정렬 계산
        int padding = (columns - line.length()) / 2;

        // 패딩이 음수일 경우에는 0으로 설정
        if (padding < 0) {
            padding = 0;
        }

        // 중앙 정렬된 메시지 출력 (빈 문자열도 포함)
        if (line.empty()) {
            cout << string(columns / 2, ' ') << " " << endl;
        }
        else {
            cout << string(padding, ' ') << line << endl;
        }
    }
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


class Character //모험가의 상태와 동작 관리
{
    string name;
    int level = 1;
    int experience = 0;
    int experience_needed = 50;
    int item_num = 3;
    int hp = 100;
    int max_hp = 100;
    int hp_item = 5;
    int attack_power = 10;
    int defense = 5;

public:
    Character(string name) // 생성자
    {
        this->name = name;
        printCentered(" ");
        printCentered(" 모험가 닉네임은 " + name + "입니다.");
        printCentered(" ");
        printCentered(" ");
        printCentered("[ 기초 아이템 ]");
        printCentered(" ");
        printCentered("체력 회복 포션: " + to_string(hp_item) + "개, 시야확장 아이템: " + to_string(item_num) + "개");
    }

    void setName(string name)
    {
        this->name = name;
        printCentered("\n▶ 모험가 닉네임이 " + name + "(으)로 변경되었습니다.");
    }

    const int MAX_LEVEL = 50; // 최대 레벨 설정

    void level_up() {
        int levels_gained = 0;
        while (experience >= experience_needed && level < MAX_LEVEL) {
            experience -= experience_needed;
            level++;
            levels_gained++;
            experience_needed = experience_needed + 50 + (level * 10);
            max_hp += 20;
            attack_power += 5;
            defense += 3;
            hp = max_hp;

            // 레벨 보너스 지급
            if (level % 5 == 0) {
                int bonus_potions = 2;
                int bonus_items = 1;
                hp_item += bonus_potions;
                item_num += bonus_items;
                printCentered("▶ 레벨 보너스: 체력 포션 " + to_string(bonus_potions) + "개, 아이템 " + to_string(bonus_items) + "개 획득!");
            }
        }

        if (levels_gained > 0) {
            printCentered("▶ 레벨이 " + to_string(levels_gained) + " 만큼 상승하여 현재 레벨: " + to_string(level) + " 입니다.");
        }

        if (level == MAX_LEVEL) {
            printCentered("▶ 축하합니다! 최대 레벨인 " + to_string(MAX_LEVEL) + "에 도달했습니다.");
            printCentered(" 괴물들의 보스와 싸울 준비가 되었습니다!");
            boss_fight();
        }
    }

    void boss_fight() {
        printCentered("\n▶ '용'과 싸우시겠습니까? (1: 싸운다, 2: 싸우지 않는다)");
        int choice;
        cin >> choice;

        if (choice == 1) {
            system("cls");
            printCentered("▶ '용'과의 최종 결전이 시작됩니다!");
            final_boss_battle();
        }
        else if (choice == 2) {
            system("cls");
            printCentered("▶ 용과 싸우기를 거부했습니다.");
            printCentered("▶ 당신은 평생 용사를 양성하며 괴물들과의 싸움을 이어갑니다.");
            printCentered("▶ 평생 지속된 싸움은 당신의 이름을 전설로 남겼습니다.");
            printCentered("▶ 엔딩: '전설의 용사, 끝없는 싸움'");
            exit(0);
        }
        else {
            printCentered("▶ 잘못된 입력입니다. 다시 선택해주세요.");
            boss_fight();
        }
    }

    void final_boss_battle() {
        int dragon_hp = 500; // 보스 체력
        int dragon_attack_power = 50; // 보스 공격력
        bool is_defeated = false;

        while (true) {
            printCentered("\n▶ 무엇을 하시겠습니까? (1: 공격, 2: 방어)");
            int action;
            cin >> action;

            if (action == 1) {
                int damage_to_dragon = attack_power + (rand() % 10);
                dragon_hp -= damage_to_dragon;
                if (dragon_hp < 0) dragon_hp = 0;

                printCentered("▶ 용에게 " + to_string(damage_to_dragon) + "의 피해를 입혔습니다.");
                printCentered("▶ 남은 용의 체력: " + to_string(dragon_hp));

                if (dragon_hp == 0) {
                    printCentered("\n▶ 용을 처치했습니다! 마을을 구했습니다!");
                    printCentered("▶ 엔딩: '위대한 용사'");
                    exit(0);
                }
            }
            else if (action == 2) {
                printCentered("▶ 방어 자세를 취합니다. 피해가 절반으로 감소합니다.");
            }
            else {
                printCentered("▶ 잘못된 입력입니다. 다시 선택해주세요.");
                continue;
            }

            printCentered("\n▶ 용의 반격!");
            int damage_to_char = dragon_attack_power - defense + (rand() % 10);

            if (action == 2) { // 방어 시 피해 절반
                damage_to_char /= 2;
            }

            if (damage_to_char < 0) damage_to_char = 0;
            hp -= damage_to_char;

            printCentered("▶ 받은 피해: " + to_string(damage_to_char));
            printCentered("▶ 남은 체력: " + to_string(hp) + " / " + to_string(max_hp));

            if (hp <= 0) {
                printCentered("\n▶ 용에게 패배했습니다. 마을이 멸망했습니다.");
                printCentered("▶ 엔딩: '마을의 멸망'");
                exit(0);
            }
        }
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
        printCentered("▶ 경험치 : " + to_string(experience) + " / " + to_string(experience_needed));
        printCentered("▶ HP : " + to_string(hp) + " / " + to_string(max_hp));
        printCentered("▶ 공격력 : " + to_string(attack_power));
        printCentered("▶ 방어력 : " + to_string(defense));
        printCentered("▶ 아이템 : " + to_string(item_num));
        printCentered("▶ 포션 : " + to_string(hp_item));
    }


    void victory_pose()
    {
        printCentered("\n 모험가가 승리 포즈를 취합니다! \"이겼다!\"");
        string animation[3] = { "\(^o^)/", "<(￣︶￣)>", "\(^-^)/" };
        for (int i = 0; i < 3; i++) {
            printCentered(animation[i]);
            Sleep(500);
            cout << flush;
        }
        Sleep(2000);
    }

    void take_damage_animation()
    {
        printCentered("\n 모험가가 공격을 받고 흔들립니다!");
        string animation[3] = { "[\\^-^\\]", "[X_X]", "[\\^-^\\]" };
        for (int i = 0; i < 3; i++) {
            printCentered(animation[i]);
            Sleep(300);
        }
    }

    void use_potion_animation()
    {
        printCentered("\n 포션을 사용합니다! 체력이 회복됩니다.");
        string animation[3] = { "[HP++]", "[HP+++++]", "[FULL_HP!]" };
        for (int i = 0; i < 3; i++) {
            printCentered(animation[i]);
            Sleep(300);
        }
    }

    int attack()
    {
        // 강력한 몬스터와 약한 몬스터 중 랜덤 선택
        bool isStrongMonster = (rand() % 2 == 0);
        int mon_hp = isStrongMonster ? 80 + (rand() % 50) + (level * 15) : 30 + (rand() % 30) + (level * 5);
        int mon_attack_power = isStrongMonster ? 10 + (rand() % 10) + (level * 3) : 5 + (rand() % 5) + (level * 2);

        printCentered(" 몬스터가 등장했습니다! " + string(isStrongMonster ? "강력한 몬스터" : "약한 몬스터") + "입니다.");

        while (true)
        {
            int action;
            printCentered("\n 무엇을 하시겠습니까? (1: 공격하기, 2: 도망가기)  ");
            cin >> action;

            if (action == 1)
            {
                printCentered(" 몬스터를 공격합니다.");
                int damage_to_mon = attack_power + (rand() % 5);
                mon_hp -= damage_to_mon;
                if (mon_hp < 0) mon_hp = 0;
                printCentered("▶ 몬스터에게 " + to_string(damage_to_mon) + "의 피해를 입혔습니다.");
                printCentered("▶ 남은 몬스터 체력 : " + to_string(mon_hp));

                if (mon_hp > 0)
                {
                    printCentered("\n 몬스터가 반격합니다!");
                    int damage_to_char = mon_attack_power - defense + (rand() % 5);

                    // 특수 공격 여부 랜덤 결정
                    if (rand() % 4 == 0) // 25% 확률로 특수 공격 발생
                    {
                        printCentered(" ");
                        printCentered(" ******* 주의 ******** ");
                        printCentered(" 몬스터가 '특수 공격'을 시도합니다! ");
                        printCentered("▶ 아이템을 사용하시겠습니까? (1: 사용, 2: 사용하지 않음) ");
                        int use_item;
                        cin >> use_item;

                        if (use_item == 1 && item_num > 0) // 아이템 사용 및 소지 확인
                        {
                            item_down();
                            printCentered(" 아이템을 사용하여 특수 공격을 막았습니다!");
                        }
                        else
                        {
                            if (use_item == 1 && item_num == 0) // 아이템 사용 시도했으나 없는 경우
                            {
                                printCentered(" 아이템이 없습니다. 피해를 2배로 받습니다!");
                            }
                            else if (use_item == 2) // 아이템 사용하지 않음
                            {
                                printCentered(" 아이템을 사용하지 않았습니다. 피해를 2배로 입습니다!");
                            }
                            damage_to_char *= 2; // 피해 3배 적용
                        }
                    }

                    if (damage_to_char < 0) damage_to_char = 0;
                    hp -= damage_to_char;
                    if (hp < 0) hp = 0;
                    take_damage_animation();
                    printCentered("▶ 받은 피해 : " + to_string(damage_to_char));
                    printCentered("▶ 남은 체력 : " + to_string(hp) + " / " + to_string(max_hp));

                    // 체력이 20% 이하일 경우 경고 메시지 출력
                    if (hp <= (0.2 * max_hp))
                    {
                        printCentered(" 체력이 얼마 남지 않았습니다!");
                        if (hp_item > 0)
                        {
                            printCentered("▶ 자동으로 물약을 사용합니다.");
                            hp_up();
                        }
                        else
                        {
                            printCentered("▶ 물약이 없습니다. 주의하세요!");
                        }
                    }
                }
            }
            else if (action == 2)
            {
                if (rand() % 2 == 0)
                {
                    printCentered(" 도망에 성공했습니다!");
                    return 1;
                }
                else
                {
                    printCentered(" 도망에 실패했습니다! 몬스터가 공격합니다!");
                    int damage_to_char = mon_attack_power - defense + (rand() % 5);
                    if (damage_to_char < 0) damage_to_char = 0;
                    hp -= damage_to_char;
                    if (hp < 0) hp = 0;
                    take_damage_animation();
                    printCentered("▶ 받은 피해 : " + to_string(damage_to_char));
                    printCentered("▶ 남은 체력 : " + to_string(hp) + " / " + to_string(max_hp));

                    // 체력이 20% 이하일 경우 경고 메시지 출력
                    if (hp <= (0.2 * max_hp))
                    {
                        printCentered(" 체력이 얼마 남지 않았습니다!");
                        if (hp_item > 0)
                        {
                            printCentered("▶ 자동으로 물약을 사용합니다.");
                            hp_up();
                        }
                        else
                        {
                            printCentered("▶ 물약이 없습니다. 주의하세요!");
                        }
                    }
                }
            }
            else
            {
                printCentered("\n▶ 잘못된 입력입니다. 다시 선택해주세요.");
                continue;
            }

            Sleep(2000);
            if (mon_hp == 0)
            {
                printCentered("\n▶ 몬스터를 처치했습니다!");
                victory_pose();
                printCentered(" ");
                waitForEnter();

                system("cls");

                // 경험치 로직
                int exp_gain = 0;
                if (!isStrongMonster) {
                    exp_gain = rand() % 2 == 0 ? 20 + rand() % 11 : 40 + rand() % 51;
                }
                else {
                    exp_gain = rand() % 2 == 0 ? 100 + rand() % 51 : 170 + rand() % 31;
                }
                printCentered("▶ " + to_string(exp_gain) + " 경험치를 획득했습니다!");
                experience += exp_gain;

                int initial_level = level; // 레벨업 이전 레벨 저장
                level_up();

                if (level > initial_level) {
                    printCentered("▶ 레벨이 " + to_string(level - initial_level) + "만큼 상승하여 현재 레벨: " + to_string(level) + "입니다!");
                }

                // 강력한 몬스터 처치 시 5% 확률로 아무 보상도 받지 않음
                if (isStrongMonster && rand() % 100 < 5)
                {
                    printCentered("\n▶ 아무 것도 얻지 못했습니다.");
                    printCentered(" ");
                    waitForEnter();
                }
                else
                {
                    // 몬스터 처치 후 랜덤 보상 획득
                    int potionReward = isStrongMonster ? (rand() % 5 + 3) : (rand() % 4);  // 강력한 몬스터: 3~7개, 약한 몬스터: 0~3개
                    int itemReward = isStrongMonster ? (rand() % 5 + 3) : (rand() % 4);    // 강력한 몬스터: 3~7개, 약한 몬스터: 0~3개

                    hp_item += potionReward;
                    item_num += itemReward;

                    printCentered("\n▶ 몬스터를 처치하여 보상을 획득했습니다!");
                    if (potionReward > 0)
                        printCentered("▶ 체력 포션: " + to_string(potionReward) + "개 획득! (총 포션: " + to_string(hp_item) + ")");
                    if (itemReward > 0)
                        printCentered("▶ 시야확장 아이템: " + to_string(itemReward) + "개 획득! (총 아이템: " + to_string(item_num) + ")");
                    if (potionReward == 0 && itemReward == 0)
                        printCentered("▶ 아무 것도 얻지 못했습니다.");
                    printCentered(" ");
                    waitForEnter();
                }

                level_up();
                return 1;
            }

            if (hp == 0)
            {
                printCentered("\n▶ 모험가가 쓰러졌습니다. 게임 오버.");
                printCentered(" ");
                waitForEnter();
                return 0;
            }
        }
    }

};

void displayEnvironment(string description)
{
    system("cls");
    printCentered("================================================================================================");
    printCentered(""); 
    printCentered(description); 
    printCentered(""); 
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
        // 화면을 지우고 새로운 프레임 출력
        system("cls");
        printCentered("================================================================================================");
        printCentered(""); 
        printCentered(animation[i]);
        printCentered(""); 
        printCentered("================================================================================================");

        Sleep(300); // 각 프레임 출력 후 300ms 대기
    }

    // 마지막에 줄바꿈 추가
    std::cout << endl;
}

int main()
{
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
        printCentered(" ");
        printCentered(" ");
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
        printCentered(" ");
        printCentered(" ");
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

    printCentered("================================================================================================");
    printCentered(" ");
    printCentered("스토리를 읽는 도중 Enter 키를 누르면 바로 스킵됩니다.");
    printCentered(" ");
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
            if (_kbhit()) {
                char key = _getch();
                if (key == '\r') { // Enter 키 감지
                    skip = true;
                    std::cout << line.substr(i + 1); // 현재 줄 나머지 부분을 바로 출력
                    break;
                }
            }
        }
        std::cout << endl;

        // 입력 버퍼 클리어 (엔터 키 입력으로 인한 추가 입력 방지)
        FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
    }

    // 중앙 정렬된 구분선 출력
    printCentered("================================================================================================");

    // Enter 키 입력을 기다리기
    waitForEnter();

    // 여정을 준비하는 진행 
    system("cls");
    printCenteredSlowly("여정을 준비하고 있습니다. 조금만 기다려주세요 :) ...");
    Sleep(1500);
    system("cls");
    printCenteredSlowly("여정을 준비하고 있습니다. 조금만 기다려주세요 :) ...");
    Sleep(1500);

    system("cls");

    printCentered("================================================================================================");

    int terminalHeight = 20; // 터미널 높이 예시
    printEmptyLines(terminalHeight / 2 - 2);

    printCentered("모험가의 닉네임을 입력해주세요(공백 없이 작성해주세요)");
    printCentered(" ");
    cin.sync(); // 입력 버퍼 정리
    cin.sync(); // 입력 버퍼 정리
    std::cout << string((110 - 10) / 2, ' '); // 가로 중앙 정렬
    string name;
    cin >> name;

    Character char1(name);

    printEmptyLines(terminalHeight / 2 - 2);

    printCentered("================================================================================================");
    waitForEnter();


    system("cls");
    printCentered("================================================================================================");
    printCentered(" ");
    printCentered(" ");
    printCentered("RPG WORLD에 오신 것을 환영합니다!");
    printCentered(" ");
    printCentered(" ");
    printCentered("================================================================================================");
    printCentered(" ");
    printCentered(" ");
    printCentered(" ");
    printCentered(" ");
    printCentered(" ");
    printCentered("모험가 " + name + "님 어서오세요! 모험을 시작해봅시다!!");
    printCentered(" ");
    printCentered(" ");
    printCentered(" ");
    printCentered(" ");
    printCentered(" ");
    printCentered("================================================================================================");

    Sleep(3000);
    system("cls");

    Character character(name);

    while (1) {
        int motion_in = 1;
        char key;
        bool selected = false;

        while (!selected) {
            system("cls");
            printCentered("================================================================================================");
            printCentered(" ");
            printCentered("                                      어떤 행동을 하시겠나요?                                     ");
            printCentered(" ");
            printCentered("================================================================================================");
            printCentered(" ");
            printCentered(" ");
            printCentered(" ");
            printCentered(" ");
            printCentered(" ");
            printCentered(" ");
            printCentered((motion_in == 1 ? " 1. 모험가 닉네임 변경 ◀" : " 1. 모험가 닉네임 변경"));
            printCentered((motion_in == 2 ? " 2. 시야확장 물약 사용 ◀" : " 2. 시야확장 물약 사용"));
            printCentered((motion_in == 3 ? " 3. 체력회복 포션 사용 ◀" : " 3. 체력회복 포션 사용"));
            printCentered((motion_in == 4 ? " 4. 여정 떠나러 가기 ◀" : " 4. 여정 떠나러 가기"));
            printCentered((motion_in == 5 ? " 5. 현재 상태창 보기 ◀" : " 5. 현재 상태창 보기"));
            printCentered(" ");
            printCentered(" ");
            printCentered(" ");
            printCentered(" ");
            printCentered(" ");
            printCentered(" ");
            printCentered(" (Enter: 선택, 0: 종료)");
            printCentered(" ");
            printCentered(" ");
            printCentered("================================================================================================");

            key = _getch();
            if (key == 80) { // 아래 화살표
                motion_in++;
                if (motion_in > 7) motion_in = 1;
            }
            else if (key == 72) { // 위 화살표
                motion_in--;
                if (motion_in < 1) motion_in = 7;
            }
            else if (key == 13) { // Enter 키
                selected = true;
            }
            else if (key == '0') { // 종료
                return 0;
            }
        }

        if (motion_in == 0) {
            printCentered("▶ 게임 종료");
            return -1;
        }
        else if (motion_in == 1) {
            system("cls");
            printCentered("\n▶ 새로운 모험가의 닉네임을 입력해주세요:");
            cin.ignore(); // 입력 버퍼 정리
            getline(cin, name);
            char1.setName(name);
        }
        else if (motion_in == 2) {
            system("cls");
            char1.item_down();
        }
        else if (motion_in == 3) {
            system("cls");
            char1.use_potion_animation();
            char1.hp_up();
        }
        else if (motion_in == 4) {

            system("cls");
            printCentered("================================================================================================");
            printCentered(" ");
            printCentered(" ");
            printCentered("           어디로 떠나시겠습니까?           ");
            printCentered(" ");
            printCentered(" ");
            printCentered("================================================================================================");
            printCentered(" ");
            printCentered(" ");
            printCentered(" ");
            printCentered(" ");
            printCentered("1. 바닷가");
            printCentered("2. 동굴");
            printCentered("3. 숲");
            printCentered("4. 마을");
            printCentered("5. 돌아가기");
            printCentered(" ");
            printCentered(" ");
            printCentered(" ");
            printCentered(" ");
            printCentered("번호에 맞게 창에 입력해주시면 됩니다! :)");
            printCentered("================================================================================================");

            int destination;
            cin >> destination;

            if (destination == 5) {
                continue;
            }
            else if (motion_in == 6)
            {
                string location;
                // 여정 선택 로직...
                if (destination == 1) {
                    location = "바닷가";
                    ascii_art = R"(     
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
                }
                else if (destination == 2) {
                    location = "동굴";
                    ascii_art = R"(                                                       
                         .,,,.          
                         .~~~;          
                   .,-~;*;~-::;;,       
             .,-,---~:-;-~;;-::;-       
            ,,~:.,,:~:::,-~;--:;!;~     
            .-~.,.,;;:~~.,::,-:!==!!    
           ,,:....;*;~-~.~:!,~!!=!!*.   
           ,-~,-,~;*;-,-,,:;:;;!=*!*.   
        .,-::-~.,~:*!-~-,.-!~;!*=*!!.   
       ,,:~,,:-.,~!==-~~,..!~;**:;!*.   
       ,,:,,:;-.,;=*==;;,.,!:*:,-!**;   
      .--;--:;,.;!$;=$;=~,,*:*,.-:=***  
      ,~~;,-;;.,!*$==#$$;--***-.,:***=  
     .-~!;,~!~,:!=$######$;=**-~-~!**=  
     ,~:!;;~~=::=$#####$##$;*!:-~-!=**  
    ,,~:!!:~!==*=####$#$###==;,.-~!==*  
    ,,:!!!;,~===####$$$$##$**:~.-;*!**  
    --!!:!!,;=$$#####$#$$$$=!!:.-!;**:  
    -~;!!!*-*=#####$#=$$$$##**;,-!;*=:  
   .~~!;***:*$#####==**$$$$#$:;~~!===*  
   .:;!;**=:=$###$$==!:=$#$$$;::~;====  
  ..;;!;**=;=$$##$=*=!-:$$$$$::~;!====  
.~!**!*;**=*=$$##$===*~~*$$$;:;;;:!*==. 
 .*====!=$*==$$$=*!******==!~::;;~:***= 
  .$!!!*=$==*!==*;;;:;:::;::;*:*!!!*==*,
,!*==*;::*!;::~:,......,:;;;;;*=!**===* 
 *$$$=!;*!~-        .     ...,~;;;;;;~. 
   ,--,-~:~,,,..,,        .- ....       
                 ............           
                                                      
        )";
                }
                else if (destination == 3) {
                    location = "숲";
                    ascii_art = R"(  
**==*=::;!**!;!!*==*=!****!;:**:!:;;**=!
=#==!==*!!=***!!!!;;;;:*!**!!=**!!;*===$
!$*!::***::~;;::~-,,--::;;!*!!*!!*****!=
-=;~--!;,.  ,,..      .,,,,--~!;;!***!~*
.*;:;,!,..            .  .....-::;:!::!=
.=*:,.;.....             .....,~!=;!*;**
-=!:,$~                ......,,~!=#;!:;*
$*;:,=~    ...      ......,--;;;:=$~;::;
$;:~,=~.................,-::;;;;~*!~;!~!
=;:--=-...............~~~::;;;;:~!!:;:~*
$;~,-=~-,.     ,---~~~::::;*!;!:-:!~::-*
=:~-~=~~~~~~~~---~~~~:::;!!*;!;--:!-:~~!
=:~-~=::::~~~::~~:::::;!!!!*:;;:~:;~:-~~
=~~-:=:::::::::::;;;::;;;::;!;::::~~;-~-
=~~,;=!;;;;;::~::::::~~~~::::::~~~-:;-~-
=~:-,=!~~,,,,,~~~;::::::;;;;!::~~~-!*;~~
=::-~:~-:-,......,,,,:*!;;!!:::~-~~*~--:
=;~-,,--------,.,--,,,,,,,,----~~~~---::
=;~-~:-------~~-~---,,,,,,,,,,~~~:-,,-::
$*:~~--~~-------~:;:-,,,,,,,...-:;,---::
$=;~-...,-~-------~~:,.......,-~:*-~~~;:
$=!~--.....,:~~--~~-~::.,,----- , .-;-;.
$**!-~~,,...,~~------~~:~-----   .=-:,, 
=;!;:-;:~-..--::~-----~-:~,,----.:~--:~-
;!!;~;;!!;,,:,-~:~----~~~:~,  -;-:-,.,-:
!;;;:;!;;:--!-~;~~~~~~--~~,.~..,.~;,. ..
*;==;=*=*;;:*!**~,~~~~~~,   ..  --..:~, 

        )";
                }
                else if (destination == 4) {
                    location = "마을";
                    ascii_art = R"(
----------------------------------------
----------------------------------------
----------------------------------------
----------------------------------------
----------------------------------------
----------------------------------------
----------------------------------------
------------~~-----------~--------------
---------~------------~-~~~-------------
--------~-~~~------~--~~~~-~~~--~-------
-------~-~~~~~~-~----~~~~~~~~~~~~~----~~
------~~~-~~~~~~-~-~-~;~~~~~~~~~~~~~~~~~
-----~~~~::~~~~:~;~~~:!:~~~~~~~!;:~~~~~~
~-~~-~~~~*:*!!~!~!~;~;;*!!!~~::~;:~~~~~~
~~~~~~~~~*;!!::;;:~-~~;!**!!:~~~!~~~~~~~
~~~~~~~~~;;:!!;!*!-~;::!$$*=;;;;;:~~~~~~
~~~~~~~~::;:;::;::::::::::;:;;;;;:~~~~~~
~-~~~~~~~---~--------------------~~~~~~~
~~~~~~~~------------------------~~~~~~~~
~~~~~~~~~~-------------------~-~~~~~~~~~
~~~~~~~~~~~~~~~~-~~-~--~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

)";
                }
                else {
                    cout << "잘못된 입력입니다.다시 선택해주세요." << endl;
                    waitForEnter();
                    continue;
                }
                // 이후에 ascii_art 출력하는 부분 사용...

                system("cls");
                printCentered("================================================================================================");
                printCentered("           " + location + "(으)로 떠납니다!           ");
                printCentered("================================================================================================");

                Sleep(1000);
                system("cls");
                printCentered("================================================================================================");
                printCenteredSlowly(location + "(으)로 이동 중입니다... ");
                printCentered("================================================================================================");

                // 이동 중 애니메이션 프레임
                const string frames[] = {
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

                // 애니메이션 출력
                for (int i = 0; i < 24; ++i) {
                    system("cls");
                    int offset = i * 2 - 20;
                    printCenteredArt(frames[i % 4], offset);
                    printCentered("================================================================================================");
                    printCentered(location + "(으)로 이동 중입니다! 조금만 기다려 주세요");
                    printCentered("================================================================================================");
                    Sleep(300);
                }

                // 도착 후 환경 설명
                system("cls");
                printCenteredASCII(ascii_art);
                Sleep(3000);
                printCenteredSlowly(location + "에 도착했습니다.");
                waitForEnter();
            }

            while (1) {
                int direction;
                system("cls");
                printCentered("\n▶ 어디로 이동하시겠습니까? (1: 위, 2: 왼쪽, 3: 아래, 4: 오른쪽, 0: 나가기)  ");
                cin >> direction;

                if (direction == 0) {
                    break;
                }

                string movementMessage;

                if (direction == 1) {
                    movementMessage = "당신은 위쪽으로 이동하고 있습니다...";
                }
                else if (direction == 2) {
                    movementMessage = "당신은 왼쪽으로 이동하고 있습니다...";
                }
                else if (direction == 3) {
                    movementMessage = "당신은 아래쪽으로 이동하고 있습니다...";
                }
                else if (direction == 4) {
                    movementMessage = "당신은 오른쪽으로 이동하고 있습니다...";
                }
                else {
                    printCentered(" 잘못된 입력입니다. 다시 선택해주세요.");
                    system("pause");
                    continue; // 잘못된 입력 시 루프 처음으로 돌아감
                }

                // 이동 메시지와 애니메이션 출력
                displayEnvironment(movementMessage);
                animateCharacterMovement();

                // 몬스터 발견 여부 결정 (애니메이션 이후에 실행)
                if (rand() % 3 == 0) {
                    system("cls");
                    printCentered(" 몬스터를 발견했습니다!");
                  

                    int result = char1.attack();
                    if (result == 0) {
                        printCentered("▶ 게임 종료");
                        return -1;
                    }
                }
            }
        }
        else if (motion_in == 5)
        {
            system("cls");
            char1.info();
        }
        else {
            printCentered(" 잘못된 입력입니다. 다시 입력해주세요!");
        }

        system("pause");
    }
    return 0;
}
