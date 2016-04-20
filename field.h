#ifndef _FIELD_
#define _FIELD_

#include "cell.h"
#include <ctime>

class Field {
private:
	Cell ** area;					// ��������� �� ������� ������
	int sizeX, sizeY, bombsCount;	// ����������� ������� � ���������� ���� � ���
	bool explode = false;			// ���� ��� �������� �������� ������������� ������ � ������ 
	const int _CELL_SIZE_ = 25;		// ������ ������
	bool gameStarted;				// ���� ��� �������� ������ ����

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

	void openCell(int i, int j);	// ����� �������� ������

	void markCell(int x, int y);	// �������� ������

	void unmarkCell(int x, int y);	// ������ �����

	bool getExplodeInfo() {
		return explode;
	}	 

	bool checkGameOver();

	int getCellSize() {
		return _CELL_SIZE_;
	}

	int checkSiblings(int _coordX, int _coordY);	// ��������� �������� ������ �� ������� ���� � ������� �� ����������

};

#endif // _FIELD_