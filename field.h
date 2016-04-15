#ifndef _FIELD_
#define _FIELD_

#include "cell.h"
#include <ctime>
#include "resource.h"

class Field {
private:
	Cell ** area;
	int sizeX, sizeY, bombsCount;
	bool explode = false;
	int buttonCode = 1000;
	int checkSiblings(int _coordX, int _coordY);
	HANDLE images[11];
public:
	Field(int _sizeX, int _sizeY);
	Field() {};
	~Field();

	Cell ** getField();

	Field & operator=(Field && other);

	int getSizeX() { return sizeX; }

	int getSizeY() { return sizeY; }

	int getButtonCode() { return buttonCode++; }

	int getBombsCount() { return bombsCount; }

	void openCell(HWND & hWnd);

	void markButton(HWND & hwnd);

	void unmarkButton(HWND & hwnd);

	bool getExplodeInfo() {
		return explode;
	}

	bool checkGameOver();

	void blockField();

	void initImages(HINSTANCE & hInst);

};

#endif // _FIELD_