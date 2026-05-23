#include "Game.hpp"
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <cstring>

// Конструктор инициализирует начальное состояние
Game::Game()
: m_brickLength(0)
, m_bricks(nullptr)
, m_currentLevel(1)
, m_marioMoveX(0.0f)
, m_movingLength(0)
, m_movings(nullptr)
, m_score(0) {
    InitObject(&m_mario, 39.0f, 10.0f, 3.0f, 3.0f, CHAR_MARIO);
}

// Деструктор гарантирует очистку динамических массивов
Game::~Game() {
    delete[] m_bricks;
    delete[] m_movings;
}

TObject* Game::AddBrick() {
    TObject* temp = new TObject[m_brickLength + 1];

    for (int i = 0; i < m_brickLength; i++) {
        temp[i] = m_bricks[i];
    }

    delete[] m_bricks;
    m_bricks = temp;
    m_brickLength++;

    return &m_bricks[m_brickLength - 1];
}

TObject* Game::AddMoving() {
    TObject* temp = new TObject[m_movingLength + 1];

    for (int i = 0; i < m_movingLength; i++) {
        temp[i] = m_movings[i];
    }

    delete[] m_movings;
    m_movings = temp;
    m_movingLength++;

    return &m_movings[m_movingLength - 1];
}

void Game::ClearMap() {
    for (int i = 0; i < MAP_WIDTH; i++) {
        m_map[0][i] = CHAR_EMPTY;
    }

    m_map[0][MAP_WIDTH] = '\0';

    for (int j = 1; j < MAP_HEIGHT; j++) {
        for (int i = 0; i <= MAP_WIDTH; i++) {
            m_map[j][i] = m_map[0][i];
        }
    }
}

void Game::CreateLevel(const int lvl) {
    SetTerminalColor("34"); // Синий цвет фона/текста для Linux терминала

    m_brickLength = 0;
    delete[] m_bricks;
    m_bricks = nullptr;

    m_marioMoveX = 0.0f;

    m_movingLength = 0;
    delete[] m_movings;
    m_movings = nullptr;

    InitObject(&m_mario, 39.0f, 10.0f, 3.0f, 3.0f, CHAR_MARIO);
    m_score = 0;

    if (lvl == 1) {
        InitObject(AddBrick(), 20.0f, 20.0f, 40.0f, 5.0f, CHAR_BRICK);

        // Подняли блоки вопросов повыше (с y=10 на y=8), чтобы об них не биться головой при высоком прыжке
        InitObject(AddBrick(), 30.0f, 8.0f, 5.0f, 3.0f, CHAR_MYSTERY);
        InitObject(AddBrick(), 50.0f, 8.0f, 5.0f, 3.0f, CHAR_MYSTERY);

        InitObject(AddBrick(), 60.0f, 15.0f, 40.0f, 10.0f, CHAR_BRICK);

        // Подняли верхний ряд блоков, чтобы было место для приземления на стену
        InitObject(AddBrick(), 60.0f, 3.0f, 10.0f, 3.0f, CHAR_USED);
        InitObject(AddBrick(), 70.0f, 3.0f, 5.0f, 3.0f, CHAR_MYSTERY);
        InitObject(AddBrick(), 75.0f, 3.0f, 5.0f, 3.0f, CHAR_USED);
        InitObject(AddBrick(), 80.0f, 3.0f, 5.0f, 3.0f, CHAR_MYSTERY);
        InitObject(AddBrick(), 85.0f, 3.0f, 10.0f, 3.0f, CHAR_USED);

        InitObject(AddBrick(), 100.0f, 20.0f, 20.0f, 5.0f, CHAR_BRICK);
        InitObject(AddBrick(), 120.0f, 15.0f, 10.0f, 10.0f, CHAR_BRICK);

        // Уменьшили пропасть с 20 до 10 блоков (чтобы компенсировать задержку клавиатуры)
        InitObject(AddBrick(), 140.0f, 20.0f, 40.0f, 5.0f, CHAR_BRICK);

        // Придвинули финиш поближе
        InitObject(AddBrick(), 190.0f, 15.0f, 10.0f, 10.0f, CHAR_FINISH);

        InitObject(AddMoving(), 25.0f, 10.0f, 3.0f, 2.0f, CHAR_ENEMY);
        InitObject(AddMoving(), 80.0f, 10.0f, 3.0f, 2.0f, CHAR_ENEMY);
    }

    if (lvl == 2) {
        InitObject(AddBrick(), 20.0f, 20.0f, 40.0f, 5.0f, CHAR_BRICK);
        InitObject(AddBrick(), 60.0f, 15.0f, 10.0f, 10.0f, CHAR_BRICK);

        // Уменьшаем смертельные пропасти между платформами для 2 уровня
        InitObject(AddBrick(), 80.0f, 20.0f, 20.0f, 5.0f, CHAR_BRICK);
        InitObject(AddBrick(), 110.0f, 15.0f, 10.0f, 10.0f, CHAR_BRICK);
        InitObject(AddBrick(), 130.0f, 20.0f, 40.0f, 5.0f, CHAR_BRICK);
        InitObject(AddBrick(), 180.0f, 15.0f, 10.0f, 10.0f, CHAR_FINISH);

        InitObject(AddMoving(), 25.0f, 10.0f, 3.0f, 2.0f, CHAR_ENEMY);
        InitObject(AddMoving(), 80.0f, 10.0f, 3.0f, 2.0f, CHAR_ENEMY);
        InitObject(AddMoving(), 65.0f, 10.0f, 3.0f, 2.0f, CHAR_ENEMY);
        InitObject(AddMoving(), 110.0f, 10.0f, 3.0f, 2.0f, CHAR_ENEMY);
        InitObject(AddMoving(), 140.0f, 10.0f, 3.0f, 2.0f, CHAR_ENEMY);
        InitObject(AddMoving(), 160.0f, 10.0f, 3.0f, 2.0f, CHAR_ENEMY);
    }

    if (lvl == 3) {
        InitObject(AddBrick(), 20.0f, 20.0f, 40.0f, 5.0f, CHAR_BRICK);

        // Смягчаем сложность последнего уровня
        InitObject(AddBrick(), 70.0f, 20.0f, 15.0f, 5.0f, CHAR_BRICK);
        InitObject(AddBrick(), 95.0f, 15.0f, 15.0f, 10.0f, CHAR_BRICK);
        InitObject(AddBrick(), 120.0f, 10.0f, 15.0f, 15.0f, CHAR_FINISH);

        InitObject(AddMoving(), 25.0f, 10.0f, 3.0f, 2.0f, CHAR_ENEMY);
        InitObject(AddMoving(), 45.0f, 10.0f, 3.0f, 2.0f, CHAR_ENEMY);
        InitObject(AddMoving(), 70.0f, 10.0f, 3.0f, 2.0f, CHAR_ENEMY);
        InitObject(AddMoving(), 80.0f, 10.0f, 3.0f, 2.0f, CHAR_ENEMY);
        InitObject(AddMoving(), 95.0f, 10.0f, 3.0f, 2.0f, CHAR_ENEMY);
        InitObject(AddMoving(), 105.0f, 10.0f, 3.0f, 2.0f, CHAR_ENEMY);
    }
}

void Game::DeleteMoving(const int i) {
    m_movingLength--;
    m_movings[i] = m_movings[m_movingLength];

    TObject* temp = nullptr;

    if (m_movingLength > 0) {
        temp = new TObject[m_movingLength];

        for (int j = 0; j < m_movingLength; j++) {
            temp[j] = m_movings[j];
        }
    }

    delete[] m_movings;
    m_movings = temp;
}

void Game::HorizonMoveMap(const float dx) {
    m_mario.x -= dx;

    for (int i = 0; i < m_brickLength; i++) {
        if (IsCollision(m_mario, m_bricks[i])) {
            m_mario.x += dx;
            return;
        }
    }

    m_mario.x += dx;

    for (int i = 0; i < m_brickLength; i++) {
        m_bricks[i].x += dx;
    }

    for (int i = 0; i < m_movingLength; i++) {
        m_movings[i].x += dx;
    }
}

void Game::HorizonMoveObject(TObject* obj) {
    obj->x += obj->horizSpeed;

    for (int i = 0; i < m_brickLength; i++) {
        if (IsCollision(*obj, m_bricks[i])) {
            obj->x -= obj->horizSpeed;
            obj->horizSpeed = -obj->horizSpeed;
            return;
        }
    }

    if (obj->cType == CHAR_ENEMY) {
        TObject tmp = *obj;
        VertMoveObject(&tmp);

        if (tmp.isFly == true) {
            obj->x -= obj->horizSpeed;
            obj->horizSpeed = -obj->horizSpeed;
        }
    }
}

void Game::InitObject(TObject* obj, const float xPos, const float yPos, const float oWidth, const float oHeight, const char inType) {
    SetObjectPos(obj, xPos, yPos);

    obj->width = oWidth;
    obj->height = oHeight;
    obj->vertSpeed = 0.0f;
    obj->cType = inType;
    obj->horizSpeed = SPEED_HORIZ;
    obj->isFly = false;
}

bool Game::IsCollision(const TObject o1, const TObject o2) const {
    const bool horizIntersect = ((o1.x + o1.width) > o2.x) && (o1.x < (o2.x + o2.width));
    const bool vertIntersect = ((o1.y + o1.height) > o2.y) && (o1.y < (o2.y + o2.height));

    return horizIntersect && vertIntersect;
}

bool Game::IsPosInMap(const int x, const int y) const {
    const bool validX = (x >= 0) && (x < MAP_WIDTH);
    const bool validY = (y >= 0) && (y < MAP_HEIGHT);

    return validX && validY;
}

void Game::MarioCollision() {
    for (int i = 0; i < m_movingLength; i++) {
        if (IsCollision(m_mario, m_movings[i])) {

            if (m_movings[i].cType == CHAR_ENEMY) {
                const bool isFalling = (m_mario.isFly == true) && (m_mario.vertSpeed > 0.0f);
                const bool isAbove = (m_mario.y + m_mario.height) < (m_movings[i].y + m_movings[i].height * 0.5f);

                if (isFalling && isAbove) {
                    m_score += SCORE_ENEMY;
                    DeleteMoving(i);
                    i--;
                    continue;
                } else {
                    PlayerDead();
                }
            }

            if (m_movings[i].cType == CHAR_COIN) {
                m_score += SCORE_COIN;
                DeleteMoving(i);
                i--;
                continue;
            }
        }
    }
}

void Game::PlayerDead() {
    SetTerminalColor("31"); // Красный цвет смерти в Linux
    SleepMs(TIME_SLEEP_DEATH_MS);
    CreateLevel(m_currentLevel);
}

void Game::PutObjectOnMap(const TObject obj) {
    const int ix = static_cast<int>(std::round(obj.x));
    const int iy = static_cast<int>(std::round(obj.y));
    const int iWidth = static_cast<int>(std::round(obj.width));
    const int iHeight = static_cast<int>(std::round(obj.height));

    for (int i = ix; i < (ix + iWidth); i++) {
        for (int j = iy; j < (iy + iHeight); j++) {
            if (IsPosInMap(i, j)) {
                m_map[j][i] = obj.cType;
            }
        }
    }
}

void Game::PutScoreOnMap() {
    char buffer[30];
    snprintf(buffer, sizeof(buffer), "Счет: %d", m_score);

    const int len = strlen(buffer);

    for (int i = 0; i < len; i++) {
        m_map[1][i + 5] = buffer[i];
    }
}

void Game::Run() {
    ShowMenu();
    ClearTerminal();
    CreateLevel(m_currentLevel);

    bool isRunning = true;

    while (isRunning) {
        ClearMap();

        char inputChar = '\0';

        if (CheckInput(inputChar)) {
            // Задаем импульс движения при нажатии клавиш
            if (inputChar == 'a' || inputChar == 'A') {
                m_marioMoveX = m_mario.isFly ? -1.4f : -1.0f; // В воздухе импульс чуть сильнее
            }

            if (inputChar == 'd' || inputChar == 'D') {
                m_marioMoveX = m_mario.isFly ? 1.4f : 1.0f;
            }

            if (inputChar == ' ') {
                if (m_mario.isFly == false) {
                    m_mario.vertSpeed = SPEED_JUMP;
                }
            }

            if (inputChar == 27) { // Код клавиши ESC в ASCII
                isRunning = false;
            }
        }

        // ФИЗИКА ДВИЖЕНИЯ: Применяем горизонтальную скорость КАЖДЫЙ КАДР
        if (std::abs(m_marioMoveX) > 0.05f) {
            HorizonMoveMap(-m_marioMoveX);

            // Эффект трения: на земле Марио останавливается почти сразу (коэффициент 0.40)
            // А в воздухе (isFly) трения почти нет (0.92) — Марио плавно летит по красивой дуге!
            float friction = m_mario.isFly ? 0.92f : 0.40f;
            m_marioMoveX *= friction;
        } else {
            m_marioMoveX = 0.0f;
        }

        if (m_mario.y > MAP_HEIGHT) {
            PlayerDead();
        }

        VertMoveObject(&m_mario);
        MarioCollision();

        for (int i = 0; i < m_brickLength; i++) {
            PutObjectOnMap(m_bricks[i]);
        }

        for (int i = 0; i < m_movingLength; i++) {
            VertMoveObject(&m_movings[i]);
            HorizonMoveObject(&m_movings[i]);

            if (m_movings[i].y > MAP_HEIGHT) {
                DeleteMoving(i);
                i--;
                continue;
            }

            PutObjectOnMap(m_movings[i]);
        }

        PutObjectOnMap(m_mario);
        PutScoreOnMap();

        SetCursor(0, 0);
        ShowMap();

        SleepMs(TIME_SLEEP_MS);
    }

    SetTerminalColor("0"); // Сброс цвета терминала
    ClearTerminal();
}

void Game::SetObjectPos(TObject* obj, const float xPos, const float yPos) {
    obj->x = xPos;
    obj->y = yPos;
}

void Game::ShowMap() const {
    // В Linux-консоли временная замена последнего символа не требуется при очистке эскейп-кодами
    for (int j = 0; j < MAP_HEIGHT; j++) {
        std::cout << m_map[j] << "\n";
    }

    std::cout << std::flush;
}

void Game::ShowMenu() const {
    SetTerminalColor("0");
    ClearTerminal();

    std::cout << "========================================" << std::endl;
    std::cout << "       Добро пожаловать в Марио!        " << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Управление:" << std::endl;
    std::cout << "  [A] / [D] - Движение влево / вправо" << std::endl;
    std::cout << "  [ПРОБЕЛ]  - Прыжок" << std::endl;
    std::cout << "  [ESC]     - Выход из игры" << std::endl;
    std::cout << "\nВаша цель: собирать монетки ($), прыгать на врагов (o) и дойти до конца (+)." << std::endl;
    std::cout << "Введите 1 для старта игры: ";

    int choice = 0;

    while (true) {
        std::cin >> choice;

        if (choice == 1) {
            break;
        }

        std::cout << "Неверный ввод! Введите 1 для старта: ";
    }
}

void Game::VertMoveObject(TObject* obj) {
    obj->isFly = true;
    obj->vertSpeed += SPEED_GRAVITY;

    SetObjectPos(obj, obj->x, obj->y + obj->vertSpeed);

    for (int i = 0; i < m_brickLength; i++) {
        if (IsCollision(*obj, m_bricks[i])) {

            if (obj->vertSpeed > 0.0f) {
                obj->isFly = false;
            }

            const bool isMysteryBox = (m_bricks[i].cType == CHAR_MYSTERY);
            const bool isMovingUp = (obj->vertSpeed < 0.0f);
            const bool isMario = (obj == &m_mario);

            if (isMysteryBox && isMovingUp && isMario) {
                m_bricks[i].cType = CHAR_USED;

                TObject* newMoving = AddMoving();
                InitObject(newMoving, m_bricks[i].x, m_bricks[i].y - 3.0f, 3.0f, 2.0f, CHAR_COIN);
                newMoving->vertSpeed = SPEED_BOUNCE;
            }

            obj->y -= obj->vertSpeed;
            obj->vertSpeed = 0.0f;

            if (m_bricks[i].cType == CHAR_FINISH) {
                m_currentLevel++;

                if (m_currentLevel > MAX_LEVEL) {
                    m_currentLevel = 1;
                }

                SetTerminalColor("32"); // Зеленый цвет триумфа в Linux
                SleepMs(TIME_SLEEP_DEATH_MS);
                CreateLevel(m_currentLevel);
            }

            break;
        }
    }
}

// ==========================================
// Реализация Linux-специфичных функций
// ==========================================

// Проверка неблокирующего ввода клавиш (замена GetKeyState для Linux)
bool Game::CheckInput(char& outChar) const {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        outChar = static_cast<char>(ch);
        return true;
    }

    return false;
}

void Game::ClearTerminal() const {
    std::cout << "\033[2J\033[1;1H" << std::flush;
}

void Game::SetCursor(const int x, const int y) const {
    std::cout << "\033[" << (y + 1) << ";" << (x + 1) << "H" << std::flush;
}

void Game::SetTerminalColor(const std::string& colorCode) const {
    std::cout << "\033[" << colorCode << "m" << std::flush;
}

void Game::SleepMs(const int ms) const {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

// Точка входа вынесена в самый низ файла для соответствия правилу 7
int main() {
    Game marioGame;
    marioGame.Run();
    return 0;
}
