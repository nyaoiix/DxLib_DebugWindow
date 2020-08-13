#include "DxLib.h"

#include <string>
#include <vector>
#include <fstream>

using namespace std;

#include "DebugWindow.h"

vector<DebugWindow::TextData> DebugWindow::textData;
HWND DebugWindow::child = nullptr;
HINSTANCE DebugWindow::hInst = nullptr;


struct Circle
{
	int x, y;
};


// Dxライブラリのメイン
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpsCmdLine, int nCmdShow)
{

	HWND hWnd = GetMainWindowHandle();

	DebugWindow::Start(hWnd, hInstance);		// デバッグウィンドウ作成

	ChangeWindowMode(true);		// ウィンドウモード変更
	SetGraphMode(640, 480, 32);	// ウィンドウのサイズ


	// DxLib の初期化
	if (DxLib_Init() < 0)
	{
		return -1;
	}

	SetDrawScreen(DX_SCREEN_BACK);		// 裏画面設定

	Circle circle;
	circle.x = 320;
	circle.y = 240;

	long long push_time[4] = {0};
	

	// メインループ
	for (; ProcessMessage()==0 && ClearDrawScreen()==0; ScreenFlip())
	{
		if (CheckHitKey(KEY_INPUT_LEFT))
		{
			circle.x--;
			push_time[0]++;
		}
		else
		{
			push_time[0] = 0;
		}

		if (CheckHitKey(KEY_INPUT_RIGHT))
		{
			circle.x++;
			push_time[1]++;
		}
		else
		{
			push_time[1] = 0;
		}

		if (CheckHitKey(KEY_INPUT_UP))
		{
			circle.y--;
			push_time[2]++;
		}
		else
		{
			push_time[2] = 0;
		}

		if (CheckHitKey(KEY_INPUT_DOWN))
		{
			circle.y++;
			push_time[3]++;
		}
		else
		{
			push_time[3] = 0;
		}

		for (int i = 0; i < 4; i++)
		{
			if (push_time[i] == 1)
			{
				switch (i)
				{
					case 0:
						DebugWindow::Log("左に進んでいます", DebugWindow::Red);
						break;

					case 1:
						DebugWindow::Log("右に進んでいます", DebugWindow::Blue);
						break;

					case 2:
						DebugWindow::Log("上に進んでいます", DebugWindow::Green);
						break;

					case 3:
						DebugWindow::Log("下に進んでいます", DebugWindow::Yellow);
						break;

				}
			}
		}

		DrawCircle(circle.x, circle.y, 20, GetColor(255,0,0), true);

	}

	DxLib_End(); // DXライブラリ終了処理
	return 0;
}
