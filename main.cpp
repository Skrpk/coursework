#include "cell.h"
#include "field.h"
#include <fstream>
#include <vector>

// Глобальные переменные:
HINSTANCE hInst;
Field myField;
// Указатель приложения
LPCTSTR szWindowClass = "QWERTY";
LPCTSTR szTitle = "САПЕР";
bool isOver = false, win = false;
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
HMENU menu;
HANDLE images[11], but, flag;	// хэндлы для изображений

struct Time {
	int hours = 0;
	int minutes = 0;
	int seconds = 0;
};

Time myTime;
bool choseLevel = false;  // флаг для окна выбора уровная

void initImages() {
	flag = LoadImage(hInst, MAKEINTRESOURCE(IDB_BITMAP4), IMAGE_BITMAP, 25, 25, LR_DEFAULTCOLOR);
	but = LoadImage(hInst, MAKEINTRESOURCE(IDB_BITMAP3), IMAGE_BITMAP, 25, 25, LR_DEFAULTCOLOR);
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
}	 // инициализация битмапов

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void DrawBitmap(HDC hdc, HBITMAP hBitmap, int xStart, int yStart)
{
	BITMAP bm;
	HDC hdcMem;
	DWORD dwSize;
	POINT ptSize, ptOrg;
	hdcMem = CreateCompatibleDC(hdc);
	SelectObject(hdcMem, hBitmap);
	SetMapMode(hdcMem, GetMapMode(hdc));
	GetObject(hBitmap, sizeof(BITMAP), (LPVOID)&bm);
	ptSize.x = bm.bmWidth;
	ptSize.y = bm.bmHeight;
	DPtoLP(hdc, &ptSize, 1);
	ptOrg.x = 0;
	ptOrg.y = 0;
	DPtoLP(hdcMem, &ptOrg, 1);
	BitBlt(
		hdc, xStart, yStart, ptSize.x, ptSize.y,
		hdcMem, ptOrg.x, ptOrg.y, SRCCOPY
		);
	DeleteDC(hdcMem);
}	 // функция, рисующая изображение


int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR
	lpCmdLine,
	int nCmdShow)
{
	MSG msg;

	MyRegisterClass(hInstance);

	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = (WNDPROC)WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(NULL, IDI_HAND);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = GetSysColorBrush(COLOR_WINDOW);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = NULL;

	return RegisterClassEx(&wcex);
}
// FUNCTION: InitInstance(HANDLE, int)
// Создает окно приложения и сохраняет указатель приложения в переменной 
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;
	hInst = hInstance; // сохраняет указатель приложения в переменной hInst
	hWnd = CreateWindow(szWindowClass,
		szTitle,
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,    // размер по Х
		CW_USEDEFAULT,    // размер по Y
		NULL,
		NULL,       // описатель меню окна
		hInstance,  // указатель приложения
		NULL);     // параметры создания.
	if (!hWnd) // Если окно не создалось, функция возвращает FALSE
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	return TRUE;

}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM
	wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc, hMemDC;
	RECT rt;
	HANDLE hMemBM;
	static HINSTANCE inst;
	menu = GetMenu(hWnd);
	int x, y, outPut;
	TCHAR textBuffer[256] = "Вы победили!";
	TCHAR looseText[50] = "Вы проиграли!";
	TCHAR choose[] = "Выбери уровень сложности";
	TCHAR timeBuffer[10];
	int ButSize = myField.getCellSize();
	static HWND buttonPlay, buttonExit, easyLevel, middleLevel, hardLevel;
	switch (message)
	{
	case WM_CREATE: // Сообщение приходит при создании окна
		easyLevel = CreateWindow("button", "Легкий", WS_CHILD, 420, 250, 150, 25, hWnd, (HMENU)1, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		middleLevel = CreateWindow("button", "Средний", WS_CHILD, 420, 290, 150, 25, hWnd, (HMENU)2, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		hardLevel = CreateWindow("button", "Сложный", WS_CHILD, 420, 330, 150, 25, hWnd, (HMENU)3, ((LPCREATESTRUCT)lParam)->hInstance, NULL);

		buttonPlay = CreateWindow("button", "Играть!", WS_CHILD | WS_VISIBLE, 420, 250, 150, 25, hWnd, (HMENU)100, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		buttonExit = CreateWindow("button", "Выход", WS_CHILD | WS_VISIBLE, 420, 300, 150, 25, hWnd, (HMENU)101, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		EnableMenuItem(menu, ID_40001, MF_DISABLED);		// отключить пункты меню для первого окна
		EnableMenuItem(menu, ID_40002, MF_DISABLED);
		initImages();
		break;

	case WM_GETMINMAXINFO:
	{
		LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;	// запретить изменение размеров окна
		lpMMI->ptMinTrackSize.x = 1000;
		lpMMI->ptMinTrackSize.y = 700;

		lpMMI->ptMaxTrackSize.x = 1000;
		lpMMI->ptMaxTrackSize.y = 700;
	}
	break;
	case WM_COMMAND:
		if (LOWORD(wParam) == 101 || LOWORD(wParam) == ID_40003)	// закрыть приложение если нажата кнопка выхода 
			SendMessage(hWnd, WM_CLOSE, 0, 0);						// или выбран соответствующий пункт меню

		if (LOWORD(wParam) == ID_40002) {							// вернуться к главному меню
			myField.stopGame();
			InvalidateRect(hWnd, 0, FALSE);
			EnableMenuItem(menu, ID_40001, MF_DISABLED);
			ShowWindow(buttonPlay, SW_SHOWNORMAL);
			ShowWindow(buttonExit, SW_SHOWNORMAL);
			ShowWindow(easyLevel, SW_HIDE);
			ShowWindow(middleLevel, SW_HIDE);
			ShowWindow(hardLevel, SW_HIDE);
			UpdateWindow(buttonPlay);
			UpdateWindow(buttonExit);
		}

		if (LOWORD(wParam) == ID_40001) {							// начать заново
			myTime.hours = 0;
			myTime.minutes = 0;
			myTime.seconds = 0;
			KillTimer(hWnd, 200);
			SetTimer(hWnd, 200, 1000, NULL);
			isOver = false;
			Field newField(myField.getSizeX(), myField.getSizeY());

			if (newField.getSizeX() == 10) {
				for (int i = 0; i < newField.getSizeX(); i++) {
					for (int j = 0; j < newField.getSizeY(); j++) {
						newField.getField()[i][j].initializeCell(370 + i * ButSize, 170 + j * ButSize,
							370 + i * ButSize + ButSize, 170 + j * ButSize + ButSize);
					}
				}
			}

			if (newField.getSizeX() == 15) {
				for (int i = 0; i < newField.getSizeX(); i++) {
					for (int j = 0; j < newField.getSizeY(); j++) {
						newField.getField()[i][j].initializeCell(320 + i * ButSize, 130 + j * ButSize,
							320 + i * ButSize + ButSize, 130 + j * ButSize + ButSize);
					}
				}
			}

			if (newField.getSizeX() == 20) {
				for (int i = 0; i < newField.getSizeX(); i++) {
					for (int j = 0; j < newField.getSizeY(); j++) {
						newField.getField()[i][j].initializeCell(250 + i * ButSize, 80 + j * ButSize,
							250 + i * ButSize + ButSize, 80 + j * ButSize + ButSize);
					}
				}
			}

			myField = newField;
			myField.startGame();
			InvalidateRect(hWnd, 0, FALSE);
		}

		if (LOWORD(wParam) == 100) {								// если нажата клавиша "Играть!"
			ShowWindow(buttonPlay, SW_HIDE);
			ShowWindow(buttonExit, SW_HIDE);
			ShowWindow(easyLevel, SW_SHOWNORMAL);
			ShowWindow(middleLevel, SW_SHOWNORMAL);
			ShowWindow(hardLevel, SW_SHOWNORMAL);
			UpdateWindow(easyLevel);
			UpdateWindow(middleLevel);
			UpdateWindow(hardLevel);
			EnableMenuItem(menu, ID_40002, MF_ENABLED);				// включить пункт меню "Главное меню"
			choseLevel = true;
			InvalidateRect(hWnd, 0, FALSE);
		}

		if (LOWORD(wParam) == 1) {
			EnableMenuItem(menu, ID_40001, MF_ENABLED);
			ShowWindow(easyLevel, SW_HIDE);
			ShowWindow(middleLevel, SW_HIDE);
			ShowWindow(hardLevel, SW_HIDE);

			SetTimer(hWnd, 200, 1000, NULL);

			isOver = false;											// если выбран легкий уровень сложности

			myField = Field(10, 10);								// создать поле размером 10х10

			for (int i = 0; i < myField.getSizeX(); i++) {
				for (int j = 0; j < myField.getSizeY(); j++) {
					myField.getField()[i][j].initializeCell(370 + i * ButSize, 170 + j * ButSize,
						370 + i * ButSize + ButSize, 170 + j * ButSize + ButSize);
				}
			}
			myField.startGame();
			InvalidateRect(hWnd, 0, FALSE);
		}

		if (LOWORD(wParam) == 2) {
			EnableMenuItem(menu, ID_40001, MF_ENABLED);
			ShowWindow(easyLevel, SW_HIDE);
			ShowWindow(middleLevel, SW_HIDE);
			ShowWindow(hardLevel, SW_HIDE);

			SetTimer(hWnd, 200, 1000, NULL);

			isOver = false;											// если выбран средний уровень сложности

			myField = Field(15, 15);								// создать поле размером 15х15

			for (int i = 0; i < myField.getSizeX(); i++) {
				for (int j = 0; j < myField.getSizeY(); j++) {
					myField.getField()[i][j].initializeCell(320 + i * ButSize, 130 + j * ButSize,
						320 + i * ButSize + ButSize, 130 + j * ButSize + ButSize);
				}
			}
			myField.startGame();
			InvalidateRect(hWnd, 0, FALSE);
			
		}

		if (LOWORD(wParam) == 3) {
			EnableMenuItem(menu, ID_40001, MF_ENABLED);
			ShowWindow(easyLevel, SW_HIDE);
			ShowWindow(middleLevel, SW_HIDE);
			ShowWindow(hardLevel, SW_HIDE);

			SetTimer(hWnd, 200, 1000, NULL);

			isOver = false;											// если выбран сложный уровень сложности

			myField = Field(20, 20);								// создать поле размером 20х20

			for (int i = 0; i < myField.getSizeX(); i++) {
				for (int j = 0; j < myField.getSizeY(); j++) {
					myField.getField()[i][j].initializeCell(250 + i * ButSize, 80 + j * ButSize,
						250 + i * ButSize + ButSize, 80 + j * ButSize + ButSize);
				}
			}
			myField.startGame();
			InvalidateRect(hWnd, 0, FALSE);
		}
		break;


	case WM_TIMER:
		myTime.seconds++;
		if (myTime.seconds == 59) {
			myTime.minutes++;
			myTime.seconds = 0;
		}

		if (myTime.minutes == 59) {
			myTime.hours++;
			myTime.minutes = 0;
		}

		InvalidateRect(hWnd, 0, FALSE);
		break;
	case WM_LBUTTONDOWN:
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		for (int i = 0; i < myField.getSizeX(); i++) {
			for (int j = 0; j < myField.getSizeY(); j++) {
				if (x > myField.getField()[i][j].getCoordX() && x < myField.getField()[i][j].getCoordRight() &&
					y > myField.getField()[i][j].getCoordY() && y < myField.getField()[i][j].getCoordBottom()) {
						if(!isOver)			// если игра не закончена, открыть ячейку
							myField.openCell(i, j);
						InvalidateRect(hWnd, 0, FALSE);
						break;
					}
				}
			}
		break;

	case WM_RBUTTONDOWN:
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		for (int i = 0; i < myField.getSizeX(); i++) {
			for (int j = 0; j < myField.getSizeY(); j++) {
				if (x > myField.getField()[i][j].getCoordX() && x < myField.getField()[i][j].getCoordRight() &&
					y > myField.getField()[i][j].getCoordY() && y < myField.getField()[i][j].getCoordBottom() && (!isOver || !win)) {
					if (!myField.getField()[i][j].isMarked()) {		// если ячейка не помечена, пометить
						myField.getField()[i][j].setMarked();
					}
					else {
						myField.getField()[i][j].setUnmarked();		// если помечена - убрать метку
					}
					if (myField.checkGameOver()) {
						win = true;
					}
					InvalidateRect(hWnd, 0, FALSE);
					break;
				}
			}
		}
		break;

	case WM_PAINT:
		
		hdc = BeginPaint(hWnd, &ps);

		RECT rect;
		GetClientRect(hWnd, &rect);
		hMemDC = CreateCompatibleDC(hdc);	// совместимый контекст устройства
		hMemBM = CreateCompatibleBitmap(hdc, GetSystemMetrics(SM_CXFULLSCREEN), GetSystemMetrics(SM_CYFULLSCREEN));

		SelectObject(hMemDC, hMemBM);

		FillRect(hMemDC, &rect, (HBRUSH)(COLOR_WINDOW + 1));	// очистить экран
		if (choseLevel) {										// если открыта страница выбора уровня, вывести текст
			TextOut(hMemDC, 400, 200, choose, ARRAYSIZE(choose));
			choseLevel = false;
		}
		
		if (win) { 
			char buff[50];
			char c, sep1, sep2;
			Time dataTime;
			std::string res, nick, date;
			TCHAR output[30];
			std::vector<std::pair<std::string, std::string>> vect = std::vector<std::pair<std::string, std::string>>();
			std::ifstream fin;
			if (myField.getSizeX() == 10) {
				fin.open("table1.txt", std::ios::in);
			}

			else if (myField.getSizeX() == 15) {
				fin.open("table2.txt", std::ios::in);
			}

			else if (myField.getSizeX() == 20) {
				fin.open("table3.txt", std::ios::in);
			}

			while (!fin.eof()) {
				fin.getline(buff, 50);
				int i = 0;
				c = buff[i];
				while (c != '\0') {
					res.append(1, c);
					c = buff[++i];
				}

				for (int i = 0; i < res.size(); i++) {
					if (res[i] == ' ') {
						nick = res.substr(0, i - 1);
						date = res.substr(i + 1, res.size());
						break;
					}
				}

				vect.push_back(std::pair<std::string, std::string>(nick, date));
			}

			for (int i = 0; i < vect.size(); i++) {
				TextOut(hMemDC, 700, 40 + i * 10, vect[i].first.c_str(), vect[i].first.size());
				TextOut(hMemDC, 750, 40 + i * 10, vect[i].second.c_str(), vect[i].second.size());
			}
			fin.close();
			
			TextOut(hMemDC, 500, 150, textBuffer, ARRAYSIZE(textBuffer));
			KillTimer(hWnd, 200);
			win = false;
			myField.stopGame();
		}

		if (myField.isGameStarted()) {
			TextOut(hMemDC, myField.getField()[0][0].getCoordX(), myField.getField()[0][0].getCoordY() - 15, timeBuffer, wsprintf(timeBuffer, "%d-%d-%d", myTime.hours, myTime.minutes, myTime.seconds));
			for (int i = 0; i < myField.getSizeX(); i++) {
				for (int j = 0; j < myField.getSizeY(); j++) {
					if (myField.getExplodeInfo() && myField.getField()[i][j].isBomb() && !myField.getField()[i][j].IsOpened()) {
						DrawBitmap(hMemDC, (HBITMAP)images[9], myField.getField()[i][j].getCoordX(), myField.getField()[i][j].getCoordY());
						TextOut(hMemDC, 450, 60, looseText, ARRAYSIZE(looseText));
						isOver = true;
						KillTimer(hWnd, 200);
						continue;
					}

					if (myField.getField()[i][j].IsOpened()) {
						if (myField.getField()[i][j].isBomb()) {
							DrawBitmap(hMemDC, (HBITMAP)images[10], myField.getField()[i][j].getCoordX(), myField.getField()[i][j].getCoordY());
						}
						else {
							outPut = myField.checkSiblings(i, j);
							if (!outPut) {
								DrawBitmap(hMemDC, (HBITMAP)images[0], myField.getField()[i][j].getCoordX(), myField.getField()[i][j].getCoordY());
							}
							else {
								DrawBitmap(hMemDC, (HBITMAP)images[outPut], myField.getField()[i][j].getCoordX(), myField.getField()[i][j].getCoordY());
							}
						}
					}
					else {
						if (myField.getField()[i][j].isMarked()) {
							DrawBitmap(hMemDC, (HBITMAP)flag, myField.getField()[i][j].getCoordX(), myField.getField()[i][j].getCoordY());
						} else {
							DrawBitmap(hMemDC, (HBITMAP)but, myField.getField()[i][j].getCoordX(), myField.getField()[i][j].getCoordY());
						}
					}
				}
			}
		}

		BitBlt(hdc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, hMemDC, 0, 0, SRCCOPY);	
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}