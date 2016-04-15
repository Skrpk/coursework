#ifndef _CELL_
#define _CELL_
#include <windows.h>  
class Cell {
private:
	bool _isBomb = false;
	bool isOpened = false;
	bool _isMarked = false;
	int coordX, coordY;
	HWND button;
	int buttonCode;

public:
	bool isBomb() {
		return _isBomb;
	}

	void setBomb() {
		_isBomb = true;
	}

	void setButton(HWND && but) {
		button = but;
	}

	int setButtonCode() {
		return buttonCode;
	}

	void setMarked() {
		_isMarked = true;
	}

	void setUnmarked() {
		_isMarked = false;
	}

	bool isMarked() {
		return _isMarked;
	}

	int getButtonCode() {
		return buttonCode;
	}

	int getCoordX() {
		return coordX;
	}

	int getCoordY() {
		return coordY;
	}

	HWND & getButton() {
		return button;
	}

	void open() {
		isOpened = true;
	}

	bool IsOpened() {
		return isOpened;
	}

	void initializeButton(HWND & _hwnd, int i, int j, HINSTANCE & hInst, int _buttonCode);

};

#endif // _CELL_
