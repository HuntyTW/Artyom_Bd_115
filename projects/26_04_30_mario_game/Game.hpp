#ifndef GAME_HPP
#define GAME_HPP

#include <chrono>
#include <cmath>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

namespace GameConstants {
    const char CHAR_BRICK = '#';
    const char CHAR_COIN = '$';
    const char CHAR_EMPTY = ' ';
    const char CHAR_ENEMY = 'o';
    const char CHAR_FINISH = '+';
    const char CHAR_MARIO = '@';
    const char CHAR_MYSTERY = '?';
    const char CHAR_USED = '-';

    constexpr float SPEED_BOUNCE = -0.7f;
    constexpr float SPEED_GRAVITY = 0.05f;
    constexpr float SPEED_HORIZ = 0.2f;
    constexpr float SPEED_JUMP = -1.25f;

    const int MAP_HEIGHT = 25;
    const int MAP_WIDTH = 80;
    const int MAX_LEVEL = 3;
    const int SCORE_COIN = 100;
    const int SCORE_ENEMY = 50;
}

struct TObject {
    float x, y, width, height;
    float vertSpeed, horizSpeed;
    bool isFly;
    char cType;
};

class GameState {
public:
    int score;
    int currentLevel;
    bool isRunning;
    bool isDead;
    bool levelComplete;
    float marioMoveX;

    TObject mario;
    std::vector<TObject> bricks;
    std::vector<TObject> movings;

    GameState();
    void ResetForNewLevel();
};

class GameMap {
private:
    char m_grid[GameConstants::MAP_HEIGHT][GameConstants::MAP_WIDTH + 1];
public:
    void Clear();
    void PutObject(const TObject& obj);
    void PutScore(int score);
    const char* GetRow(int index) const;
};

class Renderer {
public:
    void ShowMap(const GameMap& map) const;
    void ShowMenu() const;
    void SetCursor(int x, int y) const;
    void SetColor(const std::string& colorCode) const;
    void ClearTerminal() const;
};

class PhysicsEngine {
public:
    void VertMoveObject(TObject& obj, GameState& state);
    void HorizonMoveObject(TObject& obj, GameState& state);
    void HorizonMoveMap(GameState& state, float dx);
    void CheckMarioCollisions(GameState& state);
private:
    bool IsCollision(const TObject& o1, const TObject& o2) const;
};

class LevelBuilder {
public:
    void CreateLevel(int lvl, GameState& state);
private:
    void SpawnObject(std::vector<TObject>& list, float x, float y, float w, float h, char type);
};

class InputHandler {
public:
    static bool CheckInput(char& outChar);
};

class Game {
private:
    GameState m_state;
    GameMap m_map;
    Renderer m_renderer;
    PhysicsEngine m_physics;
    LevelBuilder m_builder;
public:
    void Run();
};

#endif // GAME_HPP
