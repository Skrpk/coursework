#ifndef _FIELD_
#define _FIELD_

#include "cell.h"
#include <ctime>

class Field {
private:
	Cell ** area;					// указатель на матрицу €чеейк
	int sizeX, sizeY, bombsCount;	// размерность матрицы и количество бомб в ней
	bool explode = false;			// флаг дл€ проверки открыти€ пользователем €чейки с бомбой 
	const int _CELL_SIZE_ = 25;		// размер €чейки
	bool gameStarted;				// флаг дл€ проверки начала игры

public:
	Field(int _sizeX, int _sizeY);
	Field() {};
	~Field();

	Cell ** getField();

	Field & operator=(Field & other);

	void startGame() {
		gameStarted = true;
	}

	void stopGame() {
		gameStarted = false;

	}

	bool isGameStarted() {
		return gameStarted;
	}

	int getSizeX() { return sizeX; }

	int getSizeY() { return sizeY; }

	int getBombsCount() { return bombsCount; }

	void openCell(int i, int j);	// метод открыват €чейку

	void markCell(int x, int y);	// пометить €чейку

	void unmarkCell(int x, int y);	// убрать метку

	bool getExplodeInfo() {
		return explode;
	}	 

	bool checkGameOver();

	int getCellSize() {
		return _CELL_SIZE_;
	}

	int checkSiblings(int _coordX, int _coordY);	// проверить соседние €чейки на наличие бомб и вернуть их количество

};

#endif // _FIELD_