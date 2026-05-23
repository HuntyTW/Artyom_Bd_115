#ifndef GAME_HPP
#define GAME_HPP

#include <chrono>
#include <cmath>
#include <iostream>
#include <string>
#include <thread> // Добавлено для std::this_thread::sleep_for

// Структура игрового объекта
struct TObject {
    float x;
    float y;
    float width;
    float height;
    float vertSpeed;
    bool isFly;
    char cType;
    float horizSpeed;
};

// Класс управления игрой и рендерингом (ООП)
class Game {
public:
    // Константы типов объектов
    static const char CHAR_BRICK = '#';
    static const char CHAR_COIN = '$';
    static const char CHAR_EMPTY = ' ';
    static const char CHAR_ENEMY = 'o';
    static const char CHAR_FINISH = '+';
    static const char CHAR_MARIO = '@';
    static const char CHAR_MYSTERY = '?';
    static const char CHAR_USED = '-';

    // Константы физики и логики
    static constexpr float SPEED_BOUNCE = -0.7f;
    static constexpr float SPEED_GRAVITY = 0.05f;
    static constexpr float SPEED_HORIZ = 0.2f;
    static constexpr float SPEED_JUMP = -1.25f; // Было -1.0f. Сделали прыжок выше

    // Константы окружения
    static const int MAP_HEIGHT = 25;
    static const int MAP_WIDTH = 80;
    static const int MAX_LEVEL = 3;
    static const int SCORE_COIN = 100;
    static const int SCORE_ENEMY = 50;
    static const int TIME_SLEEP_MS = 10;
    static const int TIME_SLEEP_DEATH_MS = 500;

    Game();
    ~Game();

    void Run();

private:
    // Поля класса (Инкапсуляция данных)
    int m_brickLength;
    TObject* m_bricks;
    int m_currentLevel;
    char m_map[MAP_HEIGHT][MAP_WIDTH + 1];
    TObject m_mario;
    int m_movingLength;
    TObject* m_movings;
    int m_score;

    // Внутренние методы управления памятью и уровнем
    TObject* AddBrick();
    TObject* AddMoving();
    void ClearMap();
    void CreateLevel(const int lvl);
    void DeleteMoving(const int i);

    // Внутренние методы перемещения и коллизий
    void HorizonMoveMap(const float dx);
    void HorizonMoveObject(TObject* obj);
    void InitObject(TObject* obj, const float xPos, const float yPos, const float oWidth, const float oHeight, const char inType);
    bool IsCollision(const TObject o1, const TObject o2) const;
    bool IsPosInMap(const int x, const int y) const;
    void MarioCollision();
    void PlayerDead();
    void VertMoveObject(TObject* obj);
    void SetObjectPos(TObject* obj, const float xPos, const float yPos); // Добавлено объявление метода

    // Методы отрисовки и вывода на экран
    void PutObjectOnMap(const TObject obj);
    void PutScoreOnMap();
    void ShowMap() const;
    void ShowMenu() const;

    // Специфичные для Linux утилиты
    void SetCursor(const int x, const int y) const;
    void SetTerminalColor(const std::string& colorCode) const;
    void ClearTerminal() const;
    void SleepMs(const int ms) const;
    bool CheckInput(char& outChar) const;
};

#endif // GAME_HPP
