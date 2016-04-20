#ifndef _CELL_
#define _CELL_
#include <windows.h>  
#include "resource.h"
class Cell {
private:
	bool _isBomb = false;
	bool isOpened = false;
	bool _isMarked = false;

	int nLeftRect, nTopRect, nRightRect, nBottomRect;

public:
	bool isBomb() {
		return _isBomb;
	}

	void setBomb() {
		_isBomb = true;
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

	// геттеры координат €чейки
	int getCoordX() {
		return nLeftRect;
	}

	int getCoordY() {
		return nTopRect;
	}

	int getCoordRight() {
		return nRightRect;
	}

	int getCoordBottom() {
		return nBottomRect;
	}

	void open() {
		isOpened = true;
	}

	bool IsOpened() {
		return isOpened;
	}

	void close() {
		isOpened = false;
	}

	void initializeCell(int _nLeftRect, int _nTopRect, int _nRightRect, int _nBottomRect);  // задает координаты €чейки
};

#endif // _CELL_
