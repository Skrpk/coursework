//#include <windows.h>         // подключение библиотеки с функциями API
#include "cell.h"
#include "field.h"

// Глобальные переменные:
HINSTANCE hInst;
Field myField;
// Указатель приложения
LPCTSTR szWindowClass = "QWERTY";
LPCTSTR szTitle = "САПЕР";
bool isOver = false;
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
WNDPROC mainProc;
HWND mainWindow;
HMENU menu;
HANDLE but;


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

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
//  FUNCTION: MyRegisterClass()

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

	mainWindow = hWnd;
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	return TRUE;

}


LRESULT CALLBACK cellProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {    // оконная процедура для кнопки
	TCHAR looseText[50] = "Вы проиграли!";
	static HDC hdc;
	but = LoadImage(hInst, MAKEINTRESOURCE(IDB_BITMAP3), IMAGE_BITMAP, 25, 25, LR_DEFAULTCOLOR);
	HANDLE flag = LoadImage(hInst, MAKEINTRESOURCE(IDB_BITMAP4), IMAGE_BITMAP, 25, 25, LR_DEFAULTCOLOR);
	switch (message) {
	case WM_LBUTTONDOWN:
		myField.openCell(hWnd);
		if (myField.getExplodeInfo()) {
			hdc = GetDC(mainWindow);
			TextOut(hdc, 450, 50, looseText, lstrlen(looseText));
			ReleaseDC(mainWindow, hdc);
			myField.blockField();
		}
	case WM_RBUTTONDOWN:
		for (int i = 0; i < myField.getSizeX(); i++) {
			for (int j = 0; j < myField.getSizeY(); j++) {
				if (myField.getField()[i][j].getButton() == hWnd) {
					if (!myField.getField()[i][j].IsOpened()) {
						if (!myField.getField()[i][j].isMarked()) {
							SendMessage(hWnd, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)flag);
							myField.markButton(hWnd);
							UpdateWindow(hWnd);
						}
						else {
							SendMessage(hWnd, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)but);
							myField.unmarkButton(hWnd);
							UpdateWindow(hWnd);
						}
					}

					if (myField.checkGameOver()) {
						isOver = true;
						myField.blockField();
						SendMessage(mainWindow, WM_PAINT, 0, 0);
					}
					break;
				}
			}
		}
		break;
	default:
		return CallWindowProc(mainProc, hWnd, message, wParam, lParam);
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM
	wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	static HDC hdc;
	RECT rt;
	static HINSTANCE inst;
	menu = GetMenu(hWnd);
	but = LoadImage(hInst, MAKEINTRESOURCE(IDB_BITMAP3), IMAGE_BITMAP, 25, 25, LR_DEFAULTCOLOR);
	TCHAR textBuffer[256] = "Вы победили!";
	TCHAR looseText[50] = "Вы проиграли!";
	static HWND buttonPlay, buttonExit, easyLevel, middleLevel, hardLevel;
	switch (message)
	{
	case WM_CREATE: // Сообщение приходит при создании окна
		easyLevel = CreateWindow("button", "Легкий", WS_CHILD, 420, 250, 150, 25, hWnd, (HMENU)1, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		middleLevel = CreateWindow("button", "Средний", WS_CHILD, 420, 290, 150, 25, hWnd, (HMENU)2, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		hardLevel = CreateWindow("button", "Сложный", WS_CHILD, 420, 330, 150, 25, hWnd, (HMENU)3, ((LPCREATESTRUCT)lParam)->hInstance, NULL);

		buttonPlay = CreateWindow("button", "Играть!", WS_CHILD | WS_VISIBLE, 420, 250, 150, 25, hWnd, (HMENU)100, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		buttonExit = CreateWindow("button", "Выход", WS_CHILD | WS_VISIBLE, 420, 300, 150, 25, hWnd, (HMENU)101, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
		inst = ((LPCREATESTRUCT)lParam)->hInstance;
		EnableMenuItem(menu, ID_40001, MF_DISABLED);
		EnableMenuItem(menu, ID_40002, MF_DISABLED);
		break;

	case WM_GETMINMAXINFO:
	{
		LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
		lpMMI->ptMinTrackSize.x = 1000;
		lpMMI->ptMinTrackSize.y = 700;

		lpMMI->ptMaxTrackSize.x = 1000;
		lpMMI->ptMaxTrackSize.y = 700;
	}
	break;
	case WM_COMMAND:
		if (LOWORD(wParam) == 101 || LOWORD(wParam) == ID_40003)
			SendMessage(hWnd, WM_CLOSE, 0, 0);

		if (LOWORD(wParam) == ID_40002) {
			if (myField.getSizeX() > 0) {
				for (int i = 0; i < myField.getSizeX(); i++) {
					for (int j = 0; j < myField.getSizeY(); j++) {
						ShowWindow(myField.getField()[i][j].getButton(), SW_HIDE);
					}
				}
			}
			ShowWindow(buttonPlay, SW_SHOWNORMAL);
			ShowWindow(buttonExit, SW_SHOWNORMAL);
			ShowWindow(easyLevel, SW_HIDE);
			ShowWindow(middleLevel, SW_HIDE);
			ShowWindow(hardLevel, SW_HIDE);
			UpdateWindow(buttonPlay);
			UpdateWindow(buttonExit);
		}

		if (LOWORD(wParam) == ID_40001) {
			for (int i = 0; i < myField.getSizeX(); i++) {
				for (int j = 0; j < myField.getSizeY(); j++) {
					ShowWindow(myField.getField()[i][j].getButton(), SW_HIDE);
				}
			}
			myField = Field(myField.getSizeX(), myField.getSizeY());
			myField.initImages(hInst);
			for (int i = 0; i < myField.getSizeX(); i++) {
				for (int j = 0; j < myField.getSizeY(); j++) {
					if (myField.getSizeX() == 10) {
						myField.getField()[i][j].initializeButton(hWnd, 170 + i * 25, 370 + j * 25, inst, myField.getButtonCode());
					}
					else if (myField.getSizeX() == 15) {
						myField.getField()[i][j].initializeButton(hWnd, 130 + i * 25, 310 + j * 25, inst, myField.getButtonCode());
					}
					else if (myField.getSizeX() == 20) {
						myField.getField()[i][j].initializeButton(hWnd, 100 + i * 25, 230 + j * 25, inst, myField.getButtonCode());
					}
					mainProc = (WNDPROC)SetWindowLong(myField.getField()[i][j].getButton(), GWL_WNDPROC, (LONG)cellProc);
					SendMessage(myField.getField()[i][j].getButton(), BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)but);
					UpdateWindow(myField.getField()[i][j].getButton());
				}
			}
		}

		if (LOWORD(wParam) == 100) {
			ShowWindow(buttonPlay, SW_HIDE);
			ShowWindow(buttonExit, SW_HIDE);
			ShowWindow(easyLevel, SW_SHOWNORMAL);
			ShowWindow(middleLevel, SW_SHOWNORMAL);
			ShowWindow(hardLevel, SW_SHOWNORMAL);
			UpdateWindow(easyLevel);
			UpdateWindow(middleLevel);
			UpdateWindow(hardLevel);
			EnableMenuItem(menu, ID_40002, MF_ENABLED);
			hdc = GetDC(hWnd);
			TCHAR text[] = "Выбери уровень сложности";
			TextOut(hdc, 400, 200, text, ARRAYSIZE(text));
			ReleaseDC(hWnd, hdc);
		}

		if (LOWORD(wParam) == 1) {
			EnableMenuItem(menu, ID_40001, MF_ENABLED);
			ShowWindow(easyLevel, SW_HIDE);
			ShowWindow(middleLevel, SW_HIDE);
			ShowWindow(hardLevel, SW_HIDE);

			myField = Field(10, 10);

			for (int i = 0; i < myField.getSizeX(); i++) {
				for (int j = 0; j < myField.getSizeY(); j++) {
					myField.getField()[i][j].initializeButton(hWnd, 170 + i * 25, 370 + j * 25, inst, myField.getButtonCode());
					mainProc = (WNDPROC)SetWindowLong(myField.getField()[i][j].getButton(), GWL_WNDPROC, (LONG)cellProc);
					SendMessage(myField.getField()[i][j].getButton(), BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)but);
  					UpdateWindow(myField.getField()[i][j].getButton());
				}
			}
		}

		if (LOWORD(wParam) == 2) {
			EnableMenuItem(menu, ID_40001, MF_ENABLED);
			ShowWindow(easyLevel, SW_HIDE);
			ShowWindow(middleLevel, SW_HIDE);
			ShowWindow(hardLevel, SW_HIDE);

			myField = Field(15, 15);

			for (int i = 0; i < myField.getSizeX(); i++) {
				for (int j = 0; j < myField.getSizeY(); j++) {
					myField.getField()[i][j].initializeButton(hWnd, 130 + i * 25, 310 + j * 25, inst, myField.getButtonCode());
					mainProc = (WNDPROC)SetWindowLong(myField.getField()[i][j].getButton(), GWL_WNDPROC, (LONG)cellProc);
					SendMessage(myField.getField()[i][j].getButton(), BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)but);
					UpdateWindow(myField.getField()[i][j].getButton());
				}
			}
		}

		if (LOWORD(wParam) == 3) {
			EnableMenuItem(menu, ID_40001, MF_ENABLED);
			ShowWindow(easyLevel, SW_HIDE);
			ShowWindow(middleLevel, SW_HIDE);
			ShowWindow(hardLevel, SW_HIDE);

			myField = Field(20, 20);

			for (int i = 0; i < myField.getSizeX(); i++) {
				for (int j = 0; j < myField.getSizeY(); j++) {
					myField.getField()[i][j].initializeButton(hWnd, 100 + i * 25, 230 + j * 25, inst, myField.getButtonCode());
					mainProc = (WNDPROC)SetWindowLong(myField.getField()[i][j].getButton(), GWL_WNDPROC, (LONG)cellProc);
					SendMessage(myField.getField()[i][j].getButton(), BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)but);
					UpdateWindow(myField.getField()[i][j].getButton());
				}
			}
		}
		myField.initImages(hInst);
		break;
	case WM_PAINT:
		if (isOver) {
			hdc = BeginPaint(hWnd, &ps);
			TextOut(hdc, 450, 50, textBuffer, lstrlen(textBuffer));
			EndPaint(hWnd, &ps);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}