#define _CRT_SECURE_NO_WARNINGS
#include "field.h"


Field::Field(int _sizeX, int _sizeY) {
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

Field & Field::operator=(Field && other) {


	if (!area) {
		for (int i = 0; i < sizeX; i++) {
			delete area[i];
		}

		delete area;
	}
	sizeX = other.getSizeX();
	sizeY = other.getSizeY();

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

void Field::openCell(HWND & hWnd) {
	for (int i = 0; i < sizeX; i++) {
		for (int j = 0; j < sizeY; j++) {
			if (area[i][j].getButton() == hWnd) {

				if (area[i][j].IsOpened()) {
					return;
				}

				if (area[i][j].isBomb()) {
					area[i][j].open();
					SendMessage(hWnd, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)images[10]);
					UpdateWindow(hWnd);
					explode = true;
					return;
				}

				int outPut = checkSiblings(i, j);
				if (!outPut) {
					SendMessage(hWnd, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)images[0]);
					EnableWindow(hWnd, 0);
					area[i][j].open();
					if ((i - 1) >= 0 && (j - 1) >= 0 && !area[i - 1][j - 1].isBomb())
						openCell(area[i - 1][j - 1].getButton());

					if ((i - 1) >= 0 && !area[i - 1][j].isBomb())
						openCell(area[i - 1][j].getButton());

					if ((i - 1) >= 0 && (j + 1) >= 0 && !area[i - 1][j + 1].isBomb())
						openCell(area[i - 1][j + 1].getButton());

					if ((j - 1) >= 0 && !area[i][j - 1].isBomb())
						openCell(area[i][j - 1].getButton());

					if ((j + 1) >= 0 && !area[i][j + 1].isBomb())
						openCell(area[i][j + 1].getButton());

					if ((i + 1) < sizeX && (j - 1) >= 0 && !area[i + 1][j - 1].isBomb())
						openCell(area[i + 1][j - 1].getButton());

					if ((i + 1) < sizeX && !area[i + 1][j].isBomb())
						openCell(area[i + 1][j].getButton());

					if ((i + 1) < sizeX && (j + 1) < sizeY && !area[i + 1][j + 1].isBomb())
						openCell(area[i + 1][j + 1].getButton());
				}
				else {
					SendMessage(hWnd, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)images[outPut]);
					EnableWindow(area[i][j].getButton(), 0);
					area[i][j].open();
				}
				UpdateWindow(hWnd);
				break;
			}
		}
	}
}

void Field::markButton(HWND & hwnd) {
	for (int i = 0; i < sizeX; i++) {
		for (int j = 0; j < sizeY; j++) {
			if (area[i][j].getButton() == hwnd) {
				area[i][j].setMarked();
			}
		}
	}
}

void Field::unmarkButton(HWND & hwnd) {
	for (int i = 0; i < sizeX; i++) {
		for (int j = 0; j < sizeY; j++) {
			if (area[i][j].getButton() == hwnd) {
				area[i][j].setUnmarked();
			}
		}
	}
}

bool Field::checkGameOver() {
	int markedButtonsCount = 0;

	for (int i = 0; i < sizeX; i++) {
		for (int j = 0; j < sizeY; j++) {
			if (area[i][j].isMarked() && !area[i][j].isBomb()) {
				return false;
			}

			if (area[i][j].isMarked()) {
				markedButtonsCount++;
			}
		}
	}

	return (markedButtonsCount == bombsCount) ? true : false;
}

void Field::blockField() {
	for (int i = 0; i < sizeX; i++) {
		for (int j = 0; j < sizeY; j++) {
			EnableWindow(area[i][j].getButton(), 0);
		}
	}
}

void Field::initImages(HINSTANCE & hInst) {
	images[0] = LoadImage(hInst, MAKEINTRESOURCE(IDB_BITMAP1), IMAGE_BITMAP, 25, 25, LR_DEFAULTCOLOR);
	images[1] = LoadImage(hInst, MAKEINTRESOURCE(IDB_BITMAP2), IMAGE_BITMAP, 25, 25, LR_DEFAULTCOLOR);
	images[2] = LoadImage(hInst, MAKEINTRESOURCE(IDB_BITMAP5), IMAGE_BITMAP, 25, 25, LR_DEFAULTCOLOR);
	images[3] = LoadImage(hInst, MAKEINTRESOURCE(IDB_BITMAP6), IMAGE_BITMAP, 25, 25, LR_DEFAULTCOLOR);
	images[4] = LoadImage(hInst, MAKEINTRESOURCE(IDB_BITMAP7), IMAGE_BITMAP, 25, 25, LR_DEFAULTCOLOR);
	images[5] = LoadImage(hInst, MAKEINTRESOURCE(IDB_BITMAP8), IMAGE_BITMAP, 25, 25, LR_DEFAULTCOLOR);
	images[6] = LoadImage(hInst, MAKEINTRESOURCE(IDB_BITMAP9), IMAGE_BITMAP, 25, 25, LR_DEFAULTCOLOR);
	images[7] = LoadImage(hInst, MAKEINTRESOURCE(IDB_BITMAP10), IMAGE_BITMAP, 25, 25, LR_DEFAULTCOLOR);
	images[8] = LoadImage(hInst, MAKEINTRESOURCE(IDB_BITMAP11), IMAGE_BITMAP, 25, 25, LR_DEFAULTCOLOR);
	images[9] = LoadImage(hInst, MAKEINTRESOURCE(IDB_BITMAP12), IMAGE_BITMAP, 25, 25, LR_DEFAULTCOLOR);
	images[10] = LoadImage(hInst, MAKEINTRESOURCE(IDB_BITMAP13), IMAGE_BITMAP, 25, 25, LR_DEFAULTCOLOR);
}