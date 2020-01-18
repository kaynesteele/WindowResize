#include <windows.h>
#include <iostream>
#include <windowsx.h>
#include <vector>

using namespace std;

HWND hwnd;
POINT p;
vector<int> xCoords;
vector<int> yCoords;
void SetHook();
int xDiff;
int yDiff;
int topX, topY;


int main()
{
	cout << "Right click window you want to resize, then click and drag to resize\n";
	cout << "After each resize you have to right click the next window you want to resize";
	
	SetHook();
	MSG msg;

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	return msg.wParam;
}


HHOOK mouseHook;

LRESULT CALLBACK MouseHookCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode >= 0)
	{
		switch (wParam)
		{
		case WM_LBUTTONDOWN:
			GetCursorPos(&p);
			xCoords.push_back(p.x);
			yCoords.push_back(p.y);
			break;

		case WM_LBUTTONUP:
			GetCursorPos(&p);
			xCoords.push_back(p.x);
			yCoords.push_back(p.y);

			if (xCoords.size()!=2)
			{
				xCoords.clear();
				yCoords.clear();
			}
			else
			{
				if (xCoords[0] > xCoords[1])
				{
					xDiff = xCoords[0] - xCoords[1];
					topX = xCoords[1];
				
				}
				else
				{
					xDiff = xCoords[1] - xCoords[0];
					topX = xCoords[0];
				}
				if (yCoords[0] > yCoords[1])
				{
					yDiff = yCoords[0] - yCoords[1];
					topY = yCoords[1];
				}
				else
				{
					yDiff = yCoords[1] - yCoords[0];
					topY = yCoords[0];
				}

				if (xDiff < 50 || yDiff < 50 )
				{

					cout << "Too small\n";
					xDiff = 200;
					yDiff = 200;

				}

				p.x = xDiff;
				p.y = yDiff;
				SetWindowPos(hwnd, HWND_NOTOPMOST, topX, topY, xDiff, yDiff, SWP_SHOWWINDOW | SWP_DRAWFRAME);
				xCoords.clear();
				yCoords.clear();
				hwnd = NULL;
			}
			break;

		case WM_RBUTTONDOWN:
			GetCursorPos(&p);
			hwnd = WindowFromPoint(p);
			break;
		}
	}
	return CallNextHookEx(mouseHook, nCode, wParam, lParam);
}

void SetHook()
{
	if (!(mouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseHookCallback, NULL, 0)))
	{
		cout << "Failed to install mouse hook!" << endl;
	}
}

void ReleaseHook()
{
	UnhookWindowsHookEx(mouseHook);
}
