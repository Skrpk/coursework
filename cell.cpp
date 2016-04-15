#include "cell.h"

void Cell::initializeButton(HWND & _hwnd, int i, int j, HINSTANCE & hInst, int _buttonCode)
{
	if (_isBomb) {
		button = CreateWindow("button", "", WS_CHILD | WS_VISIBLE | BS_BITMAP, j, i, 25, 25, _hwnd, (HMENU)_buttonCode, hInst, NULL);
	}
	else {
		button = CreateWindow("button", "", WS_CHILD | WS_VISIBLE | BS_BITMAP, j, i, 25, 25, _hwnd, (HMENU)_buttonCode, hInst, NULL);
	}

	coordX = i; 
	coordY = j;

	buttonCode = _buttonCode;
}

