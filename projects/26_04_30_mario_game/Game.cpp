#include "Game.hpp"
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <cstring>

using namespace GameConstants;

GameState::GameState() : score(0), currentLevel(1), isRunning(true), isDead(false), levelComplete(false), marioMoveX(0.0f) {}

void GameState::ResetForNewLevel() {
    bricks.clear();
    movings.clear();
    isDead = false;
    levelComplete = false;
    marioMoveX = 0.0f;
}

void GameMap::Clear() {
    for (int j = 0; j < MAP_HEIGHT; j++) {
        for (int i = 0; i < MAP_WIDTH; i++) {
            m_grid[j][i] = CHAR_EMPTY;
        }
        m_grid[j][MAP_WIDTH] = '\0';
    }
}

void GameMap::PutObject(const TObject& obj) {
    int ix = static_cast<int>(std::round(obj.x));
    int iy = static_cast<int>(std::round(obj.y));
    int iWidth = static_cast<int>(std::round(obj.width));
    int iHeight = static_cast<int>(std::round(obj.height));

    for (int i = ix; i < (ix + iWidth); i++) {
        for (int j = iy; j < (iy + iHeight); j++) {
            if (i >= 0 && i < MAP_WIDTH && j >= 0 && j < MAP_HEIGHT) {
                m_grid[j][i] = obj.cType;
            }
        }
    }
}

void GameMap::PutScore(int score) {
    char buffer[30];
    snprintf(buffer, sizeof(buffer), "Счет: %d", score);
    int len = strlen(buffer);
    for (int i = 0; i < len; i++) {
        if (i + 5 < MAP_WIDTH) m_grid[1][i + 5] = buffer[i];
    }
}

const char* GameMap::GetRow(int index) const {
    return m_grid[index];
}

void Renderer::ShowMap(const GameMap& map) const {
    for (int j = 0; j < MAP_HEIGHT; j++) {
        std::cout << map.GetRow(j) << "\n";
    }
    std::cout << std::flush;
}

void Renderer::ShowMenu() const {
    SetColor("0");
    ClearTerminal();
    std::cout << "========================================" << std::endl;
    std::cout << "       Добро пожаловать в Марио!        " << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Управление:" << std::endl;
    std::cout << "  [A] / [D] - Движение" << std::endl;
    std::cout << "  [ПРОБЕЛ]  - Прыжок" << std::endl;
    std::cout << "  [ESC]     - Выход" << std::endl;
    std::cout << "Введите 1 для старта игры: ";
    int choice = 0;
    while (std::cin >> choice && choice != 1) {
        std::cout << "Неверный ввод! Введите 1 для старта: ";
    }
}

void Renderer::SetCursor(int x, int y) const {
    std::cout << "\033[" << (y + 1) << ";" << (x + 1) << "H" << std::flush;
}

void Renderer::SetColor(const std::string& colorCode) const {
    std::cout << "\033[" << colorCode << "m" << std::flush;
}

void Renderer::ClearTerminal() const {
    std::cout << "\033[2J\033[1;1H" << std::flush;
}

bool PhysicsEngine::IsCollision(const TObject& o1, const TObject& o2) const {
    return ((o1.x + o1.width) > o2.x) && (o1.x < (o2.x + o2.width)) &&
    ((o1.y + o1.height) > o2.y) && (o1.y < (o2.y + o2.height));
}

void PhysicsEngine::VertMoveObject(TObject& obj, GameState& state) {
    obj.isFly = true;
    obj.vertSpeed += SPEED_GRAVITY;
    obj.y += obj.vertSpeed;

    for (auto& brick : state.bricks) {
        if (IsCollision(obj, brick)) {
            if (obj.vertSpeed > 0.0f) obj.isFly = false;

            bool isMario = (&obj == &state.mario);
            if (isMario && brick.cType == CHAR_MYSTERY && obj.vertSpeed < 0.0f) {
                brick.cType = CHAR_USED;
                TObject coin = {brick.x, brick.y - 3.0f, 3.0f, 2.0f, SPEED_BOUNCE, SPEED_HORIZ, true, CHAR_COIN};
                state.movings.push_back(coin);
            }

            obj.y -= obj.vertSpeed;
            obj.vertSpeed = 0.0f;

            if (isMario && brick.cType == CHAR_FINISH) {
                state.levelComplete = true;
            }
            break;
        }
    }
}

void PhysicsEngine::HorizonMoveObject(TObject& obj, GameState& state) {
    obj.x += obj.horizSpeed;
    for (const auto& brick : state.bricks) {
        if (IsCollision(obj, brick)) {
            obj.x -= obj.horizSpeed;
            obj.horizSpeed = -obj.horizSpeed;
            return;
        }
    }
    if (obj.cType == CHAR_ENEMY) {
        TObject tmp = obj;
        VertMoveObject(tmp, state);
        if (tmp.isFly) {
            obj.x -= obj.horizSpeed;
            obj.horizSpeed = -obj.horizSpeed;
        }
    }
}

void PhysicsEngine::HorizonMoveMap(GameState& state, float dx) {
    state.mario.x -= dx;
    for (const auto& brick : state.bricks) {
        if (IsCollision(state.mario, brick)) {
            state.mario.x += dx;
            return;
        }
    }
    state.mario.x += dx; // Возврат фантомного смещения

    for (auto& brick : state.bricks) brick.x += dx;
    for (auto& moving : state.movings) moving.x += dx;
}

void PhysicsEngine::CheckMarioCollisions(GameState& state) {
    for (auto it = state.movings.begin(); it != state.movings.end(); ) {
        if (IsCollision(state.mario, *it)) {
            if (it->cType == CHAR_ENEMY) {
                if (state.mario.isFly && state.mario.vertSpeed > 0.0f && (state.mario.y + state.mario.height) < (it->y + it->height * 0.5f)) {
                    state.score += SCORE_ENEMY;
                    it = state.movings.erase(it);
                    continue;
                } else {
                    state.isDead = true;
                }
            } else if (it->cType == CHAR_COIN) {
                state.score += SCORE_COIN;
                it = state.movings.erase(it);
                continue;
            }
        }
        ++it;
    }
}

void LevelBuilder::SpawnObject(std::vector<TObject>& list, float x, float y, float w, float h, char type) {
    list.push_back({x, y, w, h, 0.0f, SPEED_HORIZ, false, type});
}

void LevelBuilder::CreateLevel(int lvl, GameState& state) {
    state.ResetForNewLevel();
    state.mario = {39.0f, 10.0f, 3.0f, 3.0f, 0.0f, SPEED_HORIZ, false, CHAR_MARIO};

    if (lvl == 1) {
        SpawnObject(state.bricks, 20.0f, 20.0f, 40.0f, 5.0f, CHAR_BRICK);
        SpawnObject(state.bricks, 30.0f, 8.0f, 5.0f, 3.0f, CHAR_MYSTERY);
        SpawnObject(state.bricks, 50.0f, 8.0f, 5.0f, 3.0f, CHAR_MYSTERY);
        SpawnObject(state.bricks, 60.0f, 15.0f, 40.0f, 10.0f, CHAR_BRICK);
        SpawnObject(state.bricks, 100.0f, 20.0f, 20.0f, 5.0f, CHAR_BRICK);
        SpawnObject(state.bricks, 140.0f, 20.0f, 40.0f, 5.0f, CHAR_BRICK);
        SpawnObject(state.bricks, 190.0f, 15.0f, 10.0f, 10.0f, CHAR_FINISH);

        SpawnObject(state.movings, 25.0f, 10.0f, 3.0f, 2.0f, CHAR_ENEMY);
        SpawnObject(state.movings, 80.0f, 10.0f, 3.0f, 2.0f, CHAR_ENEMY);
    }
    else if (lvl == 2) {
        SpawnObject(state.bricks, 20.0f, 20.0f, 40.0f, 5.0f, CHAR_BRICK);
        SpawnObject(state.bricks, 80.0f, 20.0f, 20.0f, 5.0f, CHAR_BRICK);
        SpawnObject(state.bricks, 130.0f, 20.0f, 40.0f, 5.0f, CHAR_BRICK);
        SpawnObject(state.bricks, 180.0f, 15.0f, 10.0f, 10.0f, CHAR_FINISH);

        SpawnObject(state.movings, 25.0f, 10.0f, 3.0f, 2.0f, CHAR_ENEMY);
        SpawnObject(state.movings, 80.0f, 10.0f, 3.0f, 2.0f, CHAR_ENEMY);
        SpawnObject(state.movings, 140.0f, 10.0f, 3.0f, 2.0f, CHAR_ENEMY);
    }
    else if (lvl == 3) {
        SpawnObject(state.bricks, 20.0f, 20.0f, 40.0f, 5.0f, CHAR_BRICK);
        SpawnObject(state.bricks, 70.0f, 20.0f, 15.0f, 5.0f, CHAR_BRICK);
        SpawnObject(state.bricks, 120.0f, 10.0f, 15.0f, 15.0f, CHAR_FINISH);

        SpawnObject(state.movings, 25.0f, 10.0f, 3.0f, 2.0f, CHAR_ENEMY);
        SpawnObject(state.movings, 45.0f, 10.0f, 3.0f, 2.0f, CHAR_ENEMY);
        SpawnObject(state.movings, 80.0f, 10.0f, 3.0f, 2.0f, CHAR_ENEMY);
    }
}

bool InputHandler::CheckInput(char& outChar) {
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    int oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    int ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    if (ch != EOF) { outChar = static_cast<char>(ch); return true; }
    return false;
}

void Game::Run() {
    m_renderer.ShowMenu();
    m_renderer.ClearTerminal();
    m_renderer.SetColor("34");
    m_builder.CreateLevel(m_state.currentLevel, m_state);

    while (m_state.isRunning) {
        m_map.Clear();
        char inputChar = '\0';

        if (InputHandler::CheckInput(inputChar)) {
            if (inputChar == 'a' || inputChar == 'A') m_state.marioMoveX = m_state.mario.isFly ? -1.4f : -1.0f;
            if (inputChar == 'd' || inputChar == 'D') m_state.marioMoveX = m_state.mario.isFly ? 1.4f : 1.0f;
            if (inputChar == ' ' && !m_state.mario.isFly) m_state.mario.vertSpeed = SPEED_JUMP;
            if (inputChar == 27) m_state.isRunning = false;
        }

        // Горизонтальная инерция
        if (std::abs(m_state.marioMoveX) > 0.05f) {
            m_physics.HorizonMoveMap(m_state, -m_state.marioMoveX);
            m_state.marioMoveX *= (m_state.mario.isFly ? 0.92f : 0.40f);
        } else {
            m_state.marioMoveX = 0.0f;
        }

        // Вертикальное перемещение и столкновения
        m_physics.VertMoveObject(m_state.mario, m_state);
        m_physics.CheckMarioCollisions(m_state);

        if (m_state.mario.y > MAP_HEIGHT) m_state.isDead = true;

        // Смерть персонажа
        if (m_state.isDead) {
            m_renderer.SetColor("31");
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            m_renderer.SetColor("34");
            m_builder.CreateLevel(m_state.currentLevel, m_state);
            continue;
        }

        // Переход на следующий уровень
        if (m_state.levelComplete) {
            m_state.currentLevel = (m_state.currentLevel >= MAX_LEVEL) ? 1 : m_state.currentLevel + 1;
            m_renderer.SetColor("32");
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            m_renderer.SetColor("34");
            m_builder.CreateLevel(m_state.currentLevel, m_state);
            continue;
        }

        // Перемещение врагов и монет
        for (auto it = m_state.movings.begin(); it != m_state.movings.end(); ) {
            m_physics.VertMoveObject(*it, m_state);
            m_physics.HorizonMoveObject(*it, m_state);
            if (it->y > MAP_HEIGHT) {
                it = m_state.movings.erase(it);
            } else {
                ++it;
            }
        }

        // Отрисовка
        for (const auto& brick : m_state.bricks) m_map.PutObject(brick);
        for (const auto& moving : m_state.movings) m_map.PutObject(moving);
        m_map.PutObject(m_state.mario);
        m_map.PutScore(m_state.score);

        m_renderer.SetCursor(0, 0);
        m_renderer.ShowMap(m_map);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    m_renderer.SetColor("0");
    m_renderer.ClearTerminal();
}

// Точка входа
int main() {
    Game marioGame;
    marioGame.Run();
    return 0;
}
