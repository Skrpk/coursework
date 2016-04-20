#define _CRT_SECURE_NO_WARNINGS
#include "field.h"


Field::Field(int _sizeX, int _sizeY) 
	: gameStarted(false)
{
	if (_sizeX < 0 || _sizeX > 25 || _sizeY < 0 || _sizeY > 25) {
		sizeX = 10;
		sizeY = 10;
	}
	else {
		sizeX = _sizeX;
		sizeY = _sizeY;
	}


	area = new Cell*[sizeX];
	for (int i = 0; i < sizeX; i++) {
		area[i] = new Cell[sizeY];
	}

	bombsCount = (sizeX * sizeY - (sizeX + sizeY)) / 5;

	srand(time(0));

	for (int i = 0; i < bombsCount; i++) {
		int bombXCoord = rand() % (sizeX - 1) + 0;
		int bombYCoord = rand() % (sizeY - 1) + 0;

		while (area[bombXCoord][bombYCoord].isBomb()) {
			bombXCoord = rand() % (sizeX - 1) + 0;
			bombYCoord = rand() % (sizeY - 1) + 0;
		}

		area[bombXCoord][bombYCoord].setBomb();
	}
}

Field::~Field() {

	for (int i = 0; i < sizeX; i++) {
		delete []area[i];
	}

	delete []area;
}

Cell ** Field::getField() {
	return area;
}

Field & Field::operator=(Field & other) {


	if (!area) {
		for (int i = 0; i < sizeX; i++) {
			delete area[i];
		}

		delete area;
	}
	sizeX = other.getSizeX();
	sizeY = other.getSizeY();
	gameStarted = false;

	area = new Cell*[sizeX];
	for (int i = 0; i < sizeX; i++) {
		area[i] = new Cell[sizeY];
	}

	for (int i = 0; i < other.getSizeX(); i++) {
		for (int j = 0; j < other.getSizeY(); j++) {
			area[i][j] = other.getField()[i][j];
		}
	}

	bombsCount = other.getBombsCount();
	explode = false;
	return *this;
}

int Field::checkSiblings(int _coordX, int _coordY) {
	int counter = 0;

	if (_coordX != 0 && _coordX < (sizeX - 1) && _coordY != 0 && _coordY < (sizeY - 1) ) {
		if (area[_coordX - 1][_coordY - 1].isBomb())
			counter++;
		if (area[_coordX - 1][_coordY].isBomb())
			counter++;
		if (area[_coordX - 1][_coordY + 1].isBomb())
			counter++;
		if (area[_coordX][_coordY - 1].isBomb())
			counter++;
		if (area[_coordX][_coordY + 1].isBomb())
			counter++;
		if (area[_coordX + 1][_coordY - 1].isBomb())
			counter++;
		if (area[_coordX + 1][_coordY].isBomb())
			counter++;
		if (area[_coordX + 1][_coordY + 1].isBomb())
			counter++;

		return counter;
	}

	if (_coordX == 0 && _coordY == 0) {
		if (area[_coordX][_coordY + 1].isBomb())
			counter++;
		if (area[_coordX + 1][_coordY].isBomb())
			counter++;
		if (area[_coordX + 1][_coordY + 1].isBomb())
			counter++;
	}

	if (_coordX == 0 && _coordY != 0 && _coordY < (sizeY - 1)) {
		if (area[_coordX][_coordY - 1].isBomb())
			counter++;
		if (area[_coordX][_coordY + 1].isBomb())
			counter++;
		if (area[_coordX + 1][_coordY - 1].isBomb())
			counter++;
		if (area[_coordX + 1][_coordY].isBomb())
			counter++;
		if (area[_coordX + 1][_coordY + 1].isBomb())
			counter++;
	}

	if (_coordX == 0 && _coordY == (sizeY - 1)) {
		if (area[_coordX][_coordY - 1].isBomb())
			counter++;
		if (area[_coordX + 1][_coordY].isBomb())
			counter++;
		if (area[_coordX + 1][_coordY - 1].isBomb())
			counter++;
	}

	if (_coordY == (sizeY - 1) && _coordX != 0 && _coordX < (sizeX - 1)) {
		if (area[_coordX - 1][_coordY - 1].isBomb())
			counter++;
		if (area[_coordX - 1][_coordY].isBomb())
			counter++;
		if (area[_coordX][_coordY - 1].isBomb())
			counter++;
		if (area[_coordX + 1][_coordY - 1].isBomb())   
			counter++;
		if (area[_coordX + 1][_coordY].isBomb())
			counter++;
	}

	if (_coordX == (sizeX - 1) && _coordY == (sizeY - 1)) {
		if (area[_coordX - 1][_coordY - 1].isBomb())
			counter++;
		if (area[_coordX - 1][_coordY].isBomb())
			counter++;
		if (area[_coordX][_coordY - 1].isBomb())
			counter++;
	}

	if (_coordX == (sizeX - 1) && _coordY > 0 && _coordY < (sizeY - 1)) {
		if (area[_coordX - 1][_coordY - 1].isBomb())
			counter++;
		if (area[_coordX - 1][_coordY].isBomb())  
			counter++;
		if (area[_coordX - 1][_coordY + 1].isBomb())
			counter++;
		if (area[_coordX][_coordY - 1].isBomb())
			counter++;
		if (area[_coordX][_coordY + 1].isBomb())
			counter++;
	}

	if (_coordY == 0 && _coordX == (sizeX - 1)) {
		if (area[_coordX - 1][_coordY].isBomb())
			counter++;
		if (area[_coordX - 1][_coordY + 1].isBomb())
			counter++;
		if (area[_coordX][_coordY + 1].isBomb())
			counter++;
	}

	if (_coordY == 0 && _coordX > 0 && _coordX < (sizeX - 1)) {
		if (area[_coordX - 1][_coordY + 1].isBomb())
			counter++;
		if (area[_coordX - 1][_coordY].isBomb())
			counter++;
		if (area[_coordX][_coordY + 1].isBomb())
			counter++;
		if (area[_coordX + 1][_coordY].isBomb())
			counter++;
		if (area[_coordX + 1][_coordY + 1].isBomb())
			counter++;
	}

	return counter;
}

void Field::openCell(int i, int j) {
	if (area[i][j].IsOpened()) {	// если €чейка открыта, выйти с функии
		return;
	}

	area[i][j].open();

	if (area[i][j].isBomb()) {
		explode = true;
		return;
	}

	int outPut = checkSiblings(i, j);										// проверка, есть ли вокруг €чейки бомбы
	if (!outPut) {															// если нету, рекурсивно вызываем функцию дл€ соседей пустой €чейки
		if ((i - 1) >= 0 && (j - 1) >= 0 && !area[i - 1][j - 1].isBomb())
			openCell(i - 1, j - 1);

		if ((i - 1) >= 0 && !area[i - 1][j].isBomb())
			openCell(i - 1, j);

		if ((i - 1) >= 0 && (j + 1) >= 0 && !area[i - 1][j + 1].isBomb())
			openCell(i - 1, j + 1);

		if ((j - 1) >= 0 && !area[i][j - 1].isBomb())
			openCell(i, j - 1);

		if ((j + 1) >= 0 && !area[i][j + 1].isBomb())
			openCell(i, j + 1);

		if ((i + 1) < sizeX && (j - 1) >= 0 && !area[i + 1][j - 1].isBomb())
			openCell(i + 1, j - 1);

		if ((i + 1) < sizeX && !area[i + 1][j].isBomb())
			openCell(i + 1, j);

		if ((i + 1) < sizeX && (j + 1) < sizeY && !area[i + 1][j + 1].isBomb())
			openCell(i + 1, j + 1);
	}
}

void Field::markCell(int x, int y) {
	for (int i = 0; i < sizeX; i++) {
		for (int j = 0; j < sizeY; j++) {
			if (x > area[i][j].getCoordX() && x < area[i][j].getCoordRight() &&
				y < area[i][j].getCoordY() && y < area[i][j].getCoordBottom()) {
				area[i][j].setMarked();
			}
		}
	}
}

void Field::unmarkCell(int x, int y) {
	for (int i = 0; i < sizeX; i++) {
		for (int j = 0; j < sizeY; j++) {
			if (x > area[i][j].getCoordX() && x < area[i][j].getCoordRight() &&
				y < area[i][j].getCoordY() && y < area[i][j].getCoordBottom()) {
				area[i][j].setUnmarked();
			}
		}
	}
}

bool Field::checkGameOver() {
	int markedCellsCount = 0;

	for (int i = 0; i < sizeX; i++) {
		for (int j = 0; j < sizeY; j++) {
			if (area[i][j].isMarked() && !area[i][j].isBomb()) {
				return false;
			}

			if (area[i][j].isMarked()) {
				markedCellsCount++;
			}
		}
	}

	return (markedCellsCount == bombsCount) ? true : false;
}