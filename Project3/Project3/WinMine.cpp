#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include "resource.h"

#define NUM 100
#define TIME_SEC 1

#define WLine1 3
#define WLine2 5
#define WLine3 2
#define WMenu 0
#define space 10
#define DisArea 30
#define sizeBox 30

int xDivision = 9;
int yDivision = 9;
int Level = 9;
int cxClient, cyClient;
int cxStart = WLine2 + space + WLine1;
int cyStart = WMenu + WLine2 + space * 2 + DisArea + WLine1 * 3;
HPEN whitePen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
HPEN grayPen = CreatePen(PS_SOLID, 1, RGB(120, 120, 120));
COLORREF gray = RGB(200, 200, 200);
HPEN tPen = CreatePen(PS_SOLID, 1, gray);
HBRUSH grayBrush = CreateSolidBrush(gray);
HFONT hFont = CreateFont(DisArea, sizeBox*7/12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, TEXT("Impact"));
HBRUSH patternBrush;

bool isShowed[NUM][NUM];
bool flag[NUM][NUM];
int remainder = 0;
int tt = 0;
bool trigger = false;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("WinMine");
	HWND         hwnd;
	MSG          msg;
	WNDCLASS     wndclass;

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	//wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	//wndclass.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wndclass.hbrBackground = grayBrush;
	wndclass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	wndclass.lpszClassName = szAppName;

	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("This program requires Windows 10!"),
			szAppName, MB_ICONERROR);
		return 0;
	}


	cxClient = cxStart * 2 + sizeBox*xDivision;
	cyClient = cyStart + sizeBox*yDivision + cxStart;

	hwnd = CreateWindow(szAppName,                  // window class name
		TEXT("Mine-547"), // window caption
		//WS_OVERLAPPEDWINDOW,
		WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX),        // window style
		500,              // initial x position
		10,              // initial y position
		cxClient + 16,              // initial x size
		cyClient + 40,              // initial y size
		NULL,                       // parent window handle
		NULL,                       // window menu handle
		hInstance,                  // program instance handle
		NULL);                     // creation parameters

	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

void drawRect(HDC hdc, int cxbegin, int cybegin, int width, int height, int wPen, bool isWhite) {
	if (isWhite) {
		SelectObject(hdc, whitePen);
	}
	else
	{
		SelectObject(hdc, grayPen);
	}
	for (int i = 0; i < wPen; ++i) {
		MoveToEx(hdc, cxbegin + i, cybegin + i, NULL);
		LineTo(hdc, cxbegin + width - i, cybegin + i);
		MoveToEx(hdc, cxbegin + i, cybegin + i, NULL);
		LineTo(hdc, cxbegin + i, cybegin + height - i);
	}

	if (isWhite) {
		SelectObject(hdc, grayPen);
	}
	else
	{
		SelectObject(hdc, whitePen);
	}
	for (int i = 0; i < wPen; ++i) {
		MoveToEx(hdc, cxbegin + width -1 - i, cybegin + height -1 - i, NULL);
		LineTo(hdc, cxbegin + width - 1 - i, cybegin + i);
		MoveToEx(hdc, cxbegin + width - 1 - i, cybegin + height - 1 - i, NULL);
		LineTo(hdc, cxbegin + i, cybegin + height - 1 - i);
	}
}

void updateScore(HWND hwnd)
{
	int disx, disy;
	TCHAR buffer[100];
	HDC hdc;

	hdc = GetDC(hwnd);
	disx = disy = cxStart;
	SelectObject(hdc, tPen);
	SelectObject(hdc, grayBrush);
	Rectangle(hdc, disx + sizeBox, disy, disx + sizeBox * 3, disy + sizeBox);
	SelectObject(hdc, hFont);
	SetBkColor(hdc, gray);
	SetTextColor(hdc, RGB(200, 110, 230));
	TextOut(hdc, disx + sizeBox, disy, buffer, wsprintf(buffer, "%d", remainder));

	ReleaseDC(hwnd, hdc);
}

void setBox(HWND hwnd, int state[][100], int x, int y, RECT rect) {
	int dir[4][2] = { { 0, -1 } ,{ -1, 0 },{ 1, 0 }, { 0, 1 }};
	char buffer[100];
	RECT rect2;
	HDC hdc;

	hdc = GetDC(hwnd);
	if (!isShowed[x][y]) {
		isShowed[x][y] = true;
		if (state[x][y] == 9) {
			//Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
			for (int i = 0; i < xDivision; i++) {
				for (int j = 0; j < yDivision; j++)
				{
					if (state[i][j] == 9) {
						rect2.left = cxStart + i*sizeBox;
						rect2.top = cyStart + j*sizeBox;
						rect2.right = cxStart + (i + 1)*sizeBox;
						rect2.bottom = cyStart + (j + 1)*sizeBox;
						FillRect(hdc, &rect2, (HBRUSH)GetStockObject(BLACK_BRUSH));
					}
				}
			}
			trigger = false;
			MessageBox(hwnd, TEXT("Sorry, You FAIL !"), TEXT("Tip"), MB_ICONERROR);
		}
		else
		{
			if (state[x][y] == 0) {
				for (int k = 0; k < 4; ++k) {
					if ((x + dir[k][0] >= 0) && (x + dir[k][0] < xDivision) &&
						(y + dir[k][1] >= 0) && (y + dir[k][1] < yDivision)) {
						drawRect(hdc, rect.left, rect.top, sizeBox, sizeBox, WLine3, false);
						rect2.left = rect.left + dir[k][0] * sizeBox;
						rect2.top = rect.top + dir[k][1] * sizeBox;
						rect2.right = rect2.left + sizeBox;
						rect2.bottom = rect2.top + sizeBox;
						setBox(hwnd, state, x + dir[k][0], y + dir[k][1], rect2);
					}
				}
			}
			else
			{
				drawRect(hdc, rect.left, rect.top, sizeBox, sizeBox, WLine3, false);
				SetBkColor(hdc, RGB(200, 200, 200));
				switch (state[x][y])
				{
				case 1:
					SetTextColor(hdc, RGB(10, 10, 255));
					break;
				case 2:
					SetTextColor(hdc, RGB(10, 205, 10));
					break;
				default:
					SetTextColor(hdc, RGB(255, 10, 10));
					break;
				}
				
				DrawText(hdc, buffer, wsprintf(buffer, "%d", state[x][y]), &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
			}
		}
	}
	ReleaseDC(hwnd, hdc);
	return;
}

void initial(HWND hwnd, int state[][NUM]) {
	trigger = false;
	tt = 0;
	remainder = xDivision*yDivision / Level;
	cxClient = cxStart * 2 + sizeBox*xDivision;
	cyClient = cyStart + sizeBox*yDivision + cxStart;
	MoveWindow(hwnd, 500, 10, cxClient + 16, cyClient + 40, TRUE);
	int dir[8][2] = { { -1, -1 },{ 0, -1 },{ 1, -1 } ,{ -1, 0 },{ 1, 0 },{ -1, 1 },{ 0, 1 },{ 1, 1 } };
	srand(time(0));
	//initiate the arrays
	for (int i = 0; i < xDivision; ++i) {
		for (int j = 0; j < yDivision; j++)
		{
			state[i][j] = -1;
			isShowed[i][j] = false;
			flag[i][j] = false;
		}
	}
	//generate the mines
	for (int i = 0; i < xDivision*yDivision / Level; ++i) {
		int m, n;
		while (true)
		{
			m = rand() % xDivision;
			n = rand() % yDivision;
			if (state[m][n] == -1) {
				state[m][n] = 9;
				break;
			}
		}
	}
	//count the mine
	for (int i = 0; i < xDivision; ++i) {
		for (int j = 0; j < yDivision; ++j) {
			if (state[i][j] != 9) {
				int s = 0;
				for (int k = 0; k < 8; k++)
				{
					if ((i + dir[k][0] >= 0) && (i + dir[k][0] < xDivision) &&
						(j + dir[k][1] >= 0) && (j + dir[k][1] < yDivision)) {
						if (state[i + dir[k][0]][j + dir[k][1]] == 9) {
							s++;
						}
					}
				}
				state[i][j] = s;
			}
		}
	}
	MoveWindow(hwnd, 500, 10, cxClient + 17, cyClient + 59, FALSE);
	return;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC         hdc;
	PAINTSTRUCT ps;
	RECT        rect;
	static int state[NUM][NUM];
	static int x, y;
	char buffer[100];
	int s = 0;

	switch (message)
	{
	case WM_CREATE:
		SetTimer(hwnd, TIME_SEC, 1000, NULL);
		initial(hwnd, state);
		return 0;
		
	case WM_TIMER:
		if (trigger)
		{
			hdc = GetDC(hwnd);
			SelectObject(hdc, tPen);
			SelectObject(hdc, grayBrush);
			Rectangle(hdc, cxClient - cxStart - sizeBox * 3, cxStart, cxClient - cxStart - sizeBox, cxStart + sizeBox);
			SelectObject(hdc, hFont);
			SetBkColor(hdc, gray);
			SetTextColor(hdc, RGB(30, 50, 255));
			TextOut(hdc, cxClient - cxStart - sizeBox * 3, cxStart, buffer, wsprintf(buffer, "%d", tt++));

			ReleaseDC(hwnd, hdc);
		}
		
		return 0;

	case WM_COMMAND:
		switch (wParam)
		{
		case ID_START:
			initial(hwnd, state);
			InvalidateRect(hwnd, NULL, true);
			break;
		case ID_9:
			xDivision = 9;
			yDivision = 9;
			initial(hwnd, state);
			InvalidateRect(hwnd, NULL, true);
			break;
		case ID_19:
			xDivision = 19;
			yDivision = 19;
			initial(hwnd, state);
			InvalidateRect(hwnd, NULL, true);
			break;
		case ID_15:
			xDivision = 15;
			yDivision = 15;
			initial(hwnd, state);
			InvalidateRect(hwnd, NULL, true);
			break;
		case ID_MAKE:
			//
			break;
		case ID_RULE:
			MessageBox(hwnd, TEXT("第一条：基本定式不要忘，现场推理真够呛。\n第二条：鼠标点击不要快，稳定节奏把空开。\n第三条：顺手标雷不要惯，积累下来记录悬。\n第四条：无从下手不要愣，就近猜雷把心横。\n第五条：遇到猜雷不要怕，爆了脸上不留疤。\n	第六条：猜雷猜错不要悔，哭天抢地也白费。\n	第七条：碰上好局不要慌，紧盯局部慢扩张。\n	第八条：痛失好局不要恨，既然有缘定有份。"), TEXT("Rule"), MB_ICONINFORMATION);
			break;
		case ID_ABOUT:
			MessageBox(hwnd, TEXT("关于扫雷(copyright)\n游戏制作人：*天霸 时间：2016年12月10日20"), TEXT("About"), MB_ICONINFORMATION);
			break;
		case ID_QUIT:
			PostQuitMessage(0);
			break;
		case ID_EASY:
			Level = 9;
			initial(hwnd, state);
			InvalidateRect(hwnd, NULL, true);
			break;
		case ID_NORMAL:
			Level = 6;
			initial(hwnd, state);
			InvalidateRect(hwnd, NULL, true);
			break;
		case ID_HARD:
			Level = 3;
			initial(hwnd, state);
			InvalidateRect(hwnd, NULL, true);
			break;
		case ID_IMPOSSIBLE:
			Level = 2;
			initial(hwnd, state);
			InvalidateRect(hwnd, NULL, true);
			break;
		default:
			break;
		}
		return 0;

	case WM_LBUTTONDOWN:
		trigger = true;
		if ((LOWORD(lParam) >= cxStart) && (LOWORD(lParam) <= cxStart + sizeBox*xDivision)
			&& (HIWORD(lParam) >= cyStart) && (HIWORD(lParam) <= cyStart + sizeBox*yDivision)) {
			x = (LOWORD(lParam) - cxStart) / sizeBox;
			y = (HIWORD(lParam) - cyStart) / sizeBox;
			if ((!isShowed[x][y]) && !flag[x][y]) {
				hdc = GetDC(hwnd);
				drawRect(hdc, cxStart + x*sizeBox, cyStart + y*sizeBox, sizeBox, sizeBox, WLine3, false);
				ReleaseDC(hwnd, hdc);
			}
		}
		
		if ((LOWORD(lParam) >= cxStart + sizeBox*(xDivision - 1) / 2)
			&& (LOWORD(lParam) <= cxStart + sizeBox*(xDivision + 1) / 2)
			&& (HIWORD(lParam) >= WMenu + WLine2 + space + WLine1)
			&& (HIWORD(lParam) <= WMenu + WLine2 + space + WLine1 + DisArea)) {
			hdc = GetDC(hwnd);
			drawRect(hdc, cxStart + sizeBox*(xDivision - 1) / 2, WMenu + WLine2 + space + WLine1, sizeBox, DisArea, WLine1, false);
			Sleep(150);
			drawRect(hdc, cxStart + sizeBox*(xDivision - 1) / 2, WMenu + WLine2 + space + WLine1, sizeBox, DisArea, WLine1, true);
			initial(hwnd, state);
			InvalidateRect(hwnd, NULL, true);
			ReleaseDC(hwnd, hdc);
		}
			
		return 0;

	case WM_MOUSEMOVE:
		if ((LOWORD(lParam) >= cxStart) && (LOWORD(lParam) <= cxStart + sizeBox*xDivision)
			&& (HIWORD(lParam) >= cyStart) && (HIWORD(lParam) <= cyStart + sizeBox*yDivision))
		{
			if ((wParam & MK_LBUTTON) > 0) {
				hdc = GetDC(hwnd);
				if (!isShowed[x][y] && (x<xDivision) && (y<yDivision) && !flag[x][y]) {
					drawRect(hdc, cxStart + x*sizeBox, cyStart + y*sizeBox, sizeBox, sizeBox, WLine3, true);
				}
				if ((LOWORD(lParam) >= cxStart) && (LOWORD(lParam) <= cxStart + sizeBox*xDivision)
					&& (HIWORD(lParam) >= cyStart) && (HIWORD(lParam) <= cyStart + sizeBox*yDivision)) {

					x = (LOWORD(lParam) - cxStart) / sizeBox;
					y = (HIWORD(lParam) - cyStart) / sizeBox;
					if ((x<xDivision) && (y<yDivision) && !flag[x][y]) {
						drawRect(hdc, cxStart + x*sizeBox, cyStart + y*sizeBox, sizeBox, sizeBox, WLine3, false);
					}
				}
				ReleaseDC(hwnd, hdc);
			}
		}		
		return 0;

	case WM_LBUTTONUP:
		if ((LOWORD(lParam) >= cxStart) && (LOWORD(lParam) <= cxStart + sizeBox*xDivision)
			&& (HIWORD(lParam) >= cyStart) && (HIWORD(lParam) <= cyStart + sizeBox*yDivision)) {
			if ((!isShowed[x][y]) && (!flag[x][y])) {
				rect.left = cxStart + x*sizeBox;
				rect.top = cyStart + y*sizeBox;
				rect.right = cxStart + (x + 1)*sizeBox;
				rect.bottom = cyStart + (y + 1)*sizeBox;

				setBox(hwnd, state, x, y, rect);

			}
		}
		
		return 0;

	case WM_RBUTTONDOWN:
		trigger = true;
		x = (LOWORD(lParam) - cxStart) / sizeBox;
		y = (HIWORD(lParam) - cyStart) / sizeBox;

		if ((LOWORD(lParam) >= cxStart) && (LOWORD(lParam) <= cxStart + sizeBox*xDivision) && (!isShowed[x][y])
			&& (HIWORD(lParam) >= cyStart) && (HIWORD(lParam) <= cyStart + sizeBox*yDivision)) {
			flag[x][y] = !flag[x][y];
			rect.left = cxStart + x*sizeBox;
			rect.top = cyStart + y*sizeBox;
			rect.right = cxStart + (x + 1)*sizeBox;
			rect.bottom = cyStart + (y + 1)*sizeBox;

			hdc = GetDC(hwnd);
			SetBkColor(hdc, RGB(200, 200, 200));
			if (flag[x][y]) {
				SetTextColor(hdc, RGB(255, 0, 0));
				DrawText(hdc, buffer, wsprintf(buffer, "#"), &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
				remainder--;
			}
			else
			{
				SetTextColor(hdc, RGB(200, 200, 200));
				DrawText(hdc, buffer, wsprintf(buffer, "#"), &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
				remainder++;
			}
			updateScore(hwnd);
			ReleaseDC(hwnd, hdc);

			for (int i = 0; i < xDivision; ++i) {
				for (int j = 0; j < yDivision; ++j) {
					if (flag[i][j] && (state[i][j] == 9)) {
						s++;
					}
				}
			}

			if (s == xDivision*yDivision / Level) {
				trigger = false;
				MessageBox(hwnd, "Congratulations! Your challenge SUCCEED !",
					"Tip", MB_ICONINFORMATION);
			}
		}
		

		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		
		drawRect(hdc, 0, WMenu, cxClient, cyClient - WMenu - 1, WLine2, true);
		drawRect(hdc, WLine2 + space, WMenu + WLine2 + space, 
			sizeBox*xDivision + 2 * WLine1, WLine1 * 2 + DisArea, WLine1, false);
		drawRect(hdc, cxStart - WLine1, cyStart - WLine1, WLine1 * 2 + sizeBox*xDivision, 
			WLine1 * 2 + sizeBox*yDivision, WLine1, false);
		drawRect(hdc, cxStart + sizeBox*(xDivision - 1) / 2, WMenu + WLine2 + space + WLine1, sizeBox, DisArea, WLine1, true);
		
		for (int i = 0; i < xDivision; i++) {
			for (int j = 0; j < yDivision; j++) {
				drawRect(hdc, cxStart + i*sizeBox, cyStart + j*sizeBox, 
					sizeBox, sizeBox, WLine3, true);
			}
		}
		updateScore(hwnd);
		SelectObject(hdc, tPen);
		SelectObject(hdc, grayBrush);
		Rectangle(hdc, cxClient - cxStart - sizeBox * 3, cxStart, cxClient - cxStart - sizeBox, cxStart + sizeBox);
		SelectObject(hdc, hFont);
		SetBkColor(hdc, gray);
		SetTextColor(hdc, RGB(30, 50, 255));
		TextOut(hdc, cxClient - cxStart - sizeBox * 3, cxStart, buffer, wsprintf(buffer, "%d", tt++));

		EndPaint(hwnd, &ps);
		return 0;

	case WM_CLOSE:
		if (IDYES==MessageBox(hwnd, TEXT("你要抛弃我了吗T_T"), TEXT("Quit"), MB_ICONEXCLAMATION | MB_YESNO))
		{
			SendMessage(hwnd, WM_DESTROY, wParam, lParam);
		}
		return 0;

	case WM_DESTROY:
		DeleteObject(grayPen);
		DeleteObject(whitePen);
		DeleteObject(tPen);
		DeleteObject(hFont);
		DeleteObject(grayBrush);
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}