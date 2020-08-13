#include "DxLib.h"

#include <string>
#include <vector>
#include <fstream>

using namespace std;

#include "DebugWindow.h"

vector<DebugWindow::TextData> DebugWindow::textData;
HWND DebugWindow::child = nullptr;
HINSTANCE DebugWindow::hInst = nullptr;


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

	int select = 0;
	int nowSelect = -1;

	bool isPushUpKey = false;
	bool isPushDownKey = false;

	int pushWaitCount = 0;

	vector<string> characters;
	characters.push_back("霊夢");
	characters.push_back("魔理沙");
	characters.push_back("咲夜");
	characters.push_back("妖夢");
	characters.push_back("早苗");


	// dat.dxaファイルから画像読み込み
	vector<int> imgs;
	for (int i = 0; i < 5; i++)
	{
		string file = "dat/" + characters[i] + ".png";

		int handle = LoadGraph(file.c_str());


		string log = file;
		if (handle != -1)
		{

			log = file + "の読み込みに成功";
		}
		else
		{
			log = file + "の読み込みに失敗";
		}

		DebugWindow::Log(log);

		imgs.push_back(handle);
	}
	

	// メインループ
	for (; ProcessMessage()==0 && ClearDrawScreen()==0; ScreenFlip())
	{
		// 上下キー
		pushWaitCount--;
		if (pushWaitCount <= 0)
		{
			if (CheckHitKey(KEY_INPUT_DOWN))
			{
				select++;
				if (select > 4)
				{
					select = 0;
				}

				pushWaitCount = 20;
			}

			if (CheckHitKey(KEY_INPUT_UP))
			{
				select--;
				if (select < 0)
				{
					select = 4;
				}

				pushWaitCount = 20;
			}
		}

		// キャラクター名を表示
		for (int i = 0; i < 5; i++)
		{
			int color = (i == select) ? GetColor(255, 255, 0) : GetColor(255, 255, 255);
			DrawString(20, 50 + 50 * i, characters[i].c_str(), color);
		}


		// キャラクターが選択された
		if (nowSelect != select)
		{
			if (CheckHitKey(KEY_INPUT_SPACE))
			{
				nowSelect = select;


				DebugWindow::Color color;

				switch (select)
				{
					case 0:
						color = DebugWindow::Color::Red;
						break;

					case 1:
						color = DebugWindow::Color::Yellow;
						break;

					case 2:
						color = DebugWindow::Color::White;
						break;

					case 3:
						color = DebugWindow::Color::Green;
						break;

					case 4:
						color = DebugWindow::Color::Blue;
						break;
				}

				string log = characters[select] + " が選択されました。";

				// 別ウィンドウに表示させる
				DebugWindow::Log(log, color);
			}
		}

		// キャラクターが選択されていたら、画像表示
		if (nowSelect != -1)
		{
			DrawGraph(200, 10, imgs[nowSelect], true);
		}

	}

	DxLib_End(); // DXライブラリ終了処理
	return 0;
}
