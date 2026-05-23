#include <cmath>
#include <cstdio>
#include <cstring>
#include <iostream>

#include <windows.h>

// ==========================================
// 7.2 Объявление глобальных «констант»
// (Блоки по типу, в блоке по алфавиту)
// ==========================================

static const char CHAR_BRICK = '#';
static const char CHAR_COIN = '$';
static const char CHAR_EMPTY = ' ';
static const char CHAR_ENEMY = 'o';
static const char CHAR_FINISH = '+';
static const char CHAR_MARIO = '@';
static const char CHAR_MYSTERY = '?';
static const char CHAR_USED = '-';

static const float SPEED_BOUNCE = -0.7f;
static const float SPEED_GRAVITY = 0.05f;
static const float SPEED_HORIZ = 0.2f;
static const float SPEED_JUMP = -1.0f;

static const int MAP_HEIGHT = 25;
static const int MAP_WIDTH = 80;
static const int MAX_LEVEL = 3;
static const int SCORE_COIN = 100;
static const int SCORE_ENEMY = 50;
static const int TIME_SLEEP = 10;
static const int TIME_SLEEP_DEATH = 500;

// ==========================================
// Структуры
// ==========================================

struct TObject {
	float x, y;
	float width, height;
	float vertSpeed;
	bool isFly;
	char cType;
	float horizSpeed;
};

// ==========================================
// 7.3 Объявление вынесенных функций (по алфавиту)
// ==========================================

TObject* AddBrick(TObject*& bricks, int& brickLength);
TObject* AddMoving(TObject*& movings, int& movingLength);
void ClearMap(char map[MAP_HEIGHT][MAP_WIDTH + 1]);
void CreateLevel(const int lvl, int& brickLength, TObject*& bricks, int& movingLength, TObject*& movings, TObject& mario, int& score);
void DeleteMoving(const int i, TObject*& movings, int& movingLength);
void HorizonMoveMap(const float dx, TObject& mario, TObject* bricks, const int brickLength, TObject* movings, const int movingLength);
void HorizonMoveObject(TObject* obj, TObject*& bricks, int& brickLength, TObject& mario, TObject*& movings, int& movingLength, int& currentLevel, int& score);
void InitObject(TObject* obj, const float xPos, const float yPos, const float oWidth, const float oHeight, const char inType);
bool IsCollision(const TObject o1, const TObject o2);
bool IsPosInMap(const int x, const int y);
void MarioCollision(TObject& mario, TObject*& movings, int& movingLength, int& score, int& currentLevel, int& brickLength, TObject*& bricks);
void PlayerDead(int& currentLevel, int& brickLength, TObject*& bricks, int& movingLength, TObject*& movings, TObject& mario, int& score);
void PutObjectOnMap(const TObject obj, char map[MAP_HEIGHT][MAP_WIDTH + 1]);
void PutScoreOnMap(const int score, char map[MAP_HEIGHT][MAP_WIDTH + 1]);
void SetCursor(const int x, const int y);
void SetObjectPos(TObject* obj, const float xPos, const float yPos);
void ShowMap(char map[MAP_HEIGHT][MAP_WIDTH + 1]);
void ShowMenu();
void VertMoveObject(TObject* obj, TObject*& bricks, int& brickLength, TObject& mario, TObject*& movings, int& movingLength, int& currentLevel, int& score);

// ==========================================
// 7.4 main
// ==========================================

int main() {
	// ВСЕ глобальные переменные перенесены сюда (стали локальными)
	int brickLength = 0;
	TObject* bricks = nullptr;

	int currentLevel = 1;
	char map[MAP_HEIGHT][MAP_WIDTH + 1];
	TObject mario;

	int movingLength = 0;
	TObject* movings = nullptr;

	int score = 0;

	ShowMenu();
	CreateLevel(currentLevel, brickLength, bricks, movingLength, movings, mario, score);

	do {
		ClearMap(map);

		if ((mario.isFly == false) && (GetKeyState(VK_SPACE) < 0)) {
			mario.vertSpeed = SPEED_JUMP;
		}

		if (GetKeyState('A') < 0) {
			HorizonMoveMap(1.0f, mario, bricks, brickLength, movings, movingLength);
		}

		if (GetKeyState('D') < 0) {
			HorizonMoveMap(-1.0f, mario, bricks, brickLength, movings, movingLength);
		}

		if (mario.y > MAP_HEIGHT) {
			PlayerDead(currentLevel, brickLength, bricks, movingLength, movings, mario, score);
		}

		VertMoveObject(&mario, bricks, brickLength, mario, movings, movingLength, currentLevel, score);
		MarioCollision(mario, movings, movingLength, score, currentLevel, brickLength, bricks);

		for (int i = 0; i < brickLength; i++) {
			PutObjectOnMap(bricks[i], map);
		}

		for (int i = 0; i < movingLength; i++) {
			VertMoveObject(&movings[i], bricks, brickLength, mario, movings, movingLength, currentLevel, score);
			HorizonMoveObject(&movings[i], bricks, brickLength, mario, movings, movingLength, currentLevel, score);

			if (movings[i].y > MAP_HEIGHT) {
				DeleteMoving(i, movings, movingLength);
				i--;
				continue;
			}

			PutObjectOnMap(movings[i], map);
		}

		PutObjectOnMap(mario, map);
		PutScoreOnMap(score, map);

		SetCursor(0, 0);
		ShowMap(map);

		Sleep(TIME_SLEEP);

	} while (GetKeyState(VK_ESCAPE) >= 0);

	// Очистка выделенной памяти перед выходом
	delete[] bricks;
	delete[] movings;

	return 0;
}

// ==========================================
// 7.5 Реализация вынесенных функций (по алфавиту)
// ==========================================

TObject* AddBrick(TObject*& bricks, int& brickLength) {
	TObject* temp = new TObject[brickLength + 1];

	for (int i = 0; i < brickLength; i++) {
		temp[i] = bricks[i];
	}

	delete[] bricks;
	bricks = temp;
	brickLength++;

	return &bricks[brickLength - 1];
}

TObject* AddMoving(TObject*& movings, int& movingLength) {
	TObject* temp = new TObject[movingLength + 1];

	for (int i = 0; i < movingLength; i++) {
		temp[i] = movings[i];
	}

	delete[] movings;
	movings = temp;
	movingLength++;

	return &movings[movingLength - 1];
}

void ClearMap(char map[MAP_HEIGHT][MAP_WIDTH + 1]) {
	for (int i = 0; i < MAP_WIDTH; i++) {
		map[0][i] = CHAR_EMPTY;
	}

	map[0][MAP_WIDTH] = '\0';

	for (int j = 1; j < MAP_HEIGHT; j++) {
		for (int i = 0; i <= MAP_WIDTH; i++) {
			map[j][i] = map[0][i];
		}
	}
}

void CreateLevel(const int lvl, int& brickLength, TObject*& bricks, int& movingLength, TObject*& movings, TObject& mario, int& score) {
	system("color 9F");

	brickLength = 0;
	delete[] bricks;
	bricks = nullptr;

	movingLength = 0;
	delete[] movings;
	movings = nullptr;

	InitObject(&mario, 39.0f, 10.0f, 3.0f, 3.0f, CHAR_MARIO);
	score = 0;

	if (lvl == 1) {
		InitObject(AddBrick(bricks, brickLength), 20.0f, 20.0f, 40.0f, 5.0f, CHAR_BRICK);
		InitObject(AddBrick(bricks, brickLength), 30.0f, 10.0f, 5.0f, 3.0f, CHAR_MYSTERY);
		InitObject(AddBrick(bricks, brickLength), 50.0f, 10.0f, 5.0f, 3.0f, CHAR_MYSTERY);
		InitObject(AddBrick(bricks, brickLength), 60.0f, 15.0f, 40.0f, 10.0f, CHAR_BRICK);
		InitObject(AddBrick(bricks, brickLength), 60.0f, 5.0f, 10.0f, 3.0f, CHAR_USED);
		InitObject(AddBrick(bricks, brickLength), 70.0f, 5.0f, 5.0f, 3.0f, CHAR_MYSTERY);
		InitObject(AddBrick(bricks, brickLength), 75.0f, 5.0f, 5.0f, 3.0f, CHAR_USED);
		InitObject(AddBrick(bricks, brickLength), 80.0f, 5.0f, 5.0f, 3.0f, CHAR_MYSTERY);
		InitObject(AddBrick(bricks, brickLength), 85.0f, 5.0f, 10.0f, 3.0f, CHAR_USED);
		InitObject(AddBrick(bricks, brickLength), 100.0f, 20.0f, 20.0f, 5.0f, CHAR_BRICK);
		InitObject(AddBrick(bricks, brickLength), 120.0f, 15.0f, 10.0f, 10.0f, CHAR_BRICK);
		InitObject(AddBrick(bricks, brickLength), 150.0f, 20.0f, 40.0f, 5.0f, CHAR_BRICK);
		InitObject(AddBrick(bricks, brickLength), 210.0f, 15.0f, 10.0f, 10.0f, CHAR_FINISH);

		InitObject(AddMoving(movings, movingLength), 25.0f, 10.0f, 3.0f, 2.0f, CHAR_ENEMY);
		InitObject(AddMoving(movings, movingLength), 80.0f, 10.0f, 3.0f, 2.0f, CHAR_ENEMY);
	}

	if (lvl == 2) {
		InitObject(AddBrick(bricks, brickLength), 20.0f, 20.0f, 40.0f, 5.0f, CHAR_BRICK);
		InitObject(AddBrick(bricks, brickLength), 60.0f, 15.0f, 10.0f, 10.0f, CHAR_BRICK);
		InitObject(AddBrick(bricks, brickLength), 80.0f, 20.0f, 20.0f, 5.0f, CHAR_BRICK);
		InitObject(AddBrick(bricks, brickLength), 120.0f, 15.0f, 10.0f, 10.0f, CHAR_BRICK);
		InitObject(AddBrick(bricks, brickLength), 150.0f, 20.0f, 40.0f, 5.0f, CHAR_BRICK);
		InitObject(AddBrick(bricks, brickLength), 210.0f, 15.0f, 10.0f, 10.0f, CHAR_FINISH);

		InitObject(AddMoving(movings, movingLength), 25.0f, 10.0f, 3.0f, 2.0f, CHAR_ENEMY);
		InitObject(AddMoving(movings, movingLength), 80.0f, 10.0f, 3.0f, 2.0f, CHAR_ENEMY);
		InitObject(AddMoving(movings, movingLength), 65.0f, 10.0f, 3.0f, 2.0f, CHAR_ENEMY);
		InitObject(AddMoving(movings, movingLength), 120.0f, 10.0f, 3.0f, 2.0f, CHAR_ENEMY);
		InitObject(AddMoving(movings, movingLength), 160.0f, 10.0f, 3.0f, 2.0f, CHAR_ENEMY);
		InitObject(AddMoving(movings, movingLength), 175.0f, 10.0f, 3.0f, 2.0f, CHAR_ENEMY);
	}

	if (lvl == 3) {
		InitObject(AddBrick(bricks, brickLength), 20.0f, 20.0f, 40.0f, 5.0f, CHAR_BRICK);
		InitObject(AddBrick(bricks, brickLength), 80.0f, 20.0f, 15.0f, 5.0f, CHAR_BRICK);
		InitObject(AddBrick(bricks, brickLength), 120.0f, 15.0f, 15.0f, 10.0f, CHAR_BRICK);
		InitObject(AddBrick(bricks, brickLength), 160.0f, 10.0f, 15.0f, 15.0f, CHAR_FINISH);

		InitObject(AddMoving(movings, movingLength), 25.0f, 10.0f, 3.0f, 2.0f, CHAR_ENEMY);
		InitObject(AddMoving(movings, movingLength), 50.0f, 10.0f, 3.0f, 2.0f, CHAR_ENEMY);
		InitObject(AddMoving(movings, movingLength), 80.0f, 10.0f, 3.0f, 2.0f, CHAR_ENEMY);
		InitObject(AddMoving(movings, movingLength), 90.0f, 10.0f, 3.0f, 2.0f, CHAR_ENEMY);
		InitObject(AddMoving(movings, movingLength), 120.0f, 10.0f, 3.0f, 2.0f, CHAR_ENEMY);
		InitObject(AddMoving(movings, movingLength), 130.0f, 10.0f, 3.0f, 2.0f, CHAR_ENEMY);
	}
}

void DeleteMoving(const int i, TObject*& movings, int& movingLength) {
	movingLength--;
	movings[i] = movings[movingLength];

	TObject* temp = nullptr;

	if (movingLength > 0) {
		temp = new TObject[movingLength];

		for (int j = 0; j < movingLength; j++) {
			temp[j] = movings[j];
		}
	}

	delete[] movings;
	movings = temp;
}

void HorizonMoveMap(const float dx, TObject& mario, TObject* bricks, const int brickLength, TObject* movings, const int movingLength) {
	mario.x -= dx;

	for (int i = 0; i < brickLength; i++) {
		if (IsCollision(mario, bricks[i])) {
			mario.x += dx;
			return;
		}
	}

	mario.x += dx;

	for (int i = 0; i < brickLength; i++) {
		bricks[i].x += dx;
	}

	for (int i = 0; i < movingLength; i++) {
		movings[i].x += dx;
	}
}

void HorizonMoveObject(TObject* obj, TObject*& bricks, int& brickLength, TObject& mario, TObject*& movings, int& movingLength, int& currentLevel, int& score) {
	obj->x += obj->horizSpeed;

	for (int i = 0; i < brickLength; i++) {
		if (IsCollision(*obj, bricks[i])) {
			obj->x -= obj->horizSpeed;
			obj->horizSpeed = -obj->horizSpeed;
			return;
		}
	}

	if (obj->cType == CHAR_ENEMY) {
		TObject tmp = *obj;
		VertMoveObject(&tmp, bricks, brickLength, mario, movings, movingLength, currentLevel, score);

		if (tmp.isFly == true) {
			obj->x -= obj->horizSpeed;
			obj->horizSpeed = -obj->horizSpeed;
		}
	}
}

void InitObject(TObject* obj, const float xPos, const float yPos, const float oWidth, const float oHeight, const char inType) {
	SetObjectPos(obj, xPos, yPos);

	obj->width = oWidth;
	obj->height = oHeight;
	obj->vertSpeed = 0.0f;
	obj->cType = inType;
	obj->horizSpeed = SPEED_HORIZ;
	obj->isFly = false;
}

bool IsCollision(const TObject o1, const TObject o2) {
	const bool horizIntersect = ((o1.x + o1.width) > o2.x) && (o1.x < (o2.x + o2.width));
	const bool vertIntersect = ((o1.y + o1.height) > o2.y) && (o1.y < (o2.y + o2.height));

	return horizIntersect && vertIntersect;
}

bool IsPosInMap(const int x, const int y) {
	const bool validX = (x >= 0) && (x < MAP_WIDTH);
	const bool validY = (y >= 0) && (y < MAP_HEIGHT);

	return validX && validY;
}

void MarioCollision(TObject& mario, TObject*& movings, int& movingLength, int& score, int& currentLevel, int& brickLength, TObject*& bricks) {
	for (int i = 0; i < movingLength; i++) {
		if (IsCollision(mario, movings[i])) {

			if (movings[i].cType == CHAR_ENEMY) {
				const bool isFalling = (mario.isFly == true) && (mario.vertSpeed > 0.0f);
				const bool isAbove = (mario.y + mario.height) < (movings[i].y + movings[i].height * 0.5f);

				if (isFalling && isAbove) {
					score += SCORE_ENEMY;
					DeleteMoving(i, movings, movingLength);
					i--;
					continue;
				} else {
					PlayerDead(currentLevel, brickLength, bricks, movingLength, movings, mario, score);
				}
			}

			if (movings[i].cType == CHAR_COIN) {
				score += SCORE_COIN;
				DeleteMoving(i, movings, movingLength);
				i--;
				continue;
			}
		}
	}
}

void PlayerDead(int& currentLevel, int& brickLength, TObject*& bricks, int& movingLength, TObject*& movings, TObject& mario, int& score) {
	system("color 4F");
	Sleep(TIME_SLEEP_DEATH);
	CreateLevel(currentLevel, brickLength, bricks, movingLength, movings, mario, score);
}

void PutObjectOnMap(const TObject obj, char map[MAP_HEIGHT][MAP_WIDTH + 1]) {
	const int ix = static_cast<int>(std::round(obj.x));
	const int iy = static_cast<int>(std::round(obj.y));
	const int iWidth = static_cast<int>(std::round(obj.width));
	const int iHeight = static_cast<int>(std::round(obj.height));

	for (int i = ix; i < (ix + iWidth); i++) {
		for (int j = iy; j < (iy + iHeight); j++) {
			if (IsPosInMap(i, j)) {
				map[j][i] = obj.cType;
			}
		}
	}
}

void PutScoreOnMap(const int score, char map[MAP_HEIGHT][MAP_WIDTH + 1]) {
	char buffer[30];
	snprintf(buffer, sizeof(buffer), "Счет: %d", score);

	const int len = strlen(buffer);

	for (int i = 0; i < len; i++) {
		map[1][i + 5] = buffer[i];
	}
}

void SetCursor(const int x, const int y) {
	COORD coord;
	coord.X = static_cast<short>(x);
	coord.Y = static_cast<short>(y);

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void SetObjectPos(TObject* obj, const float xPos, const float yPos) {
	obj->x = xPos;
	obj->y = yPos;
}

void ShowMap(char map[MAP_HEIGHT][MAP_WIDTH + 1]) {
	map[MAP_HEIGHT - 1][MAP_WIDTH - 1] = '\0';

	for (int j = 0; j < MAP_HEIGHT; j++) {
		std::cout << map[j];
	}
}

void ShowMenu() {
	system("color 0F");

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

void VertMoveObject(TObject* obj, TObject*& bricks, int& brickLength, TObject& mario, TObject*& movings, int& movingLength, int& currentLevel, int& score) {
	obj->isFly = true;
	obj->vertSpeed += SPEED_GRAVITY;

	SetObjectPos(obj, obj->x, obj->y + obj->vertSpeed);

	for (int i = 0; i < brickLength; i++) {
		if (IsCollision(*obj, bricks[i])) {

			if (obj->vertSpeed > 0.0f) {
				obj->isFly = false;
			}

			const bool isMysteryBox = (bricks[i].cType == CHAR_MYSTERY);
			const bool isMovingUp = (obj->vertSpeed < 0.0f);
			const bool isMario = (obj == &mario);

			if (isMysteryBox && isMovingUp && isMario) {
				bricks[i].cType = CHAR_USED;

				TObject* newMoving = AddMoving(movings, movingLength);
				InitObject(newMoving, bricks[i].x, bricks[i].y - 3.0f, 3.0f, 2.0f, CHAR_COIN);
				newMoving->vertSpeed = SPEED_BOUNCE;
			}

			obj->y -= obj->vertSpeed;
			obj->vertSpeed = 0.0f;

			if (bricks[i].cType == CHAR_FINISH) {
				currentLevel++;

				if (currentLevel > MAX_LEVEL) {
					currentLevel = 1;
				}

				system("color 2F");
				Sleep(TIME_SLEEP_DEATH);
				CreateLevel(currentLevel, brickLength, bricks, movingLength, movings, mario, score);
			}

			break;
		}
	}
}
