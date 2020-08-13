#pragma once

// デバッグウィンドウ
class DebugWindow
{
	public:

		enum Color
		{
			White = 0,
			Red,
			Blue,
			Yellow,
			Green,
		};

	private:
		static HWND child;
		static HINSTANCE hInst;

		static const int WindowW = 600;
		static const int WindowH = 640;

		struct TextData
		{
			public:
				int color;
				string text;

				TextData(Color color, string text)
				{
					this->color = color;
					this->text = text;
				}
		};
		static vector<TextData> textData;

		enum
		{
			BTN_CLEAR = 1001,
			BTN_SAVE,
		};

	public:
		// コンストラクタ
		DebugWindow()
		{

		}


		// デバッグウィンドウ開始
		static bool Start(HWND hWnd, HINSTANCE hInstance)
		{
			if (!IsDebug())
			{
				return false;
			}


			hInst = hInstance;
			

			WNDCLASSEX wc;
			LPCSTR szClassName = "DX_DEBUG_WND";

			wc.cbSize = sizeof(WNDCLASSEX);
			wc.style = CS_HREDRAW | CS_VREDRAW;
			wc.lpfnWndProc = (WNDPROC)DebugWindow::DbgProc;
			wc.cbClsExtra = 0;
			wc.cbWndExtra = 0;
			wc.hInstance = hInstance;
			wc.hIcon = (HICON)LoadImage(
			NULL,
			IDI_APPLICATION,
			IMAGE_ICON,
			0,
			0,
			LR_DEFAULTSIZE | LR_SHARED);
			wc.hCursor = (HCURSOR)LoadImage(
			NULL,
			IDC_ARROW,
			IMAGE_CURSOR,
			0,
			0,
			LR_DEFAULTSIZE | LR_SHARED);
			wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
			wc.lpszMenuName = NULL;
			wc.lpszClassName = szClassName;
			wc.hIconSm = (HICON)LoadImage(
			NULL,
			IDI_APPLICATION,
			IMAGE_ICON,
			0,
			0,
			LR_DEFAULTSIZE | LR_SHARED);

			RegisterClassEx(&wc);

			RECT rect = {0, 0, WindowW, WindowH};
			AdjustWindowRect(&rect, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, FALSE);
			
			///ウィンドウの生成
			child = CreateWindow(szClassName,
					"DEBUG WINDOW",	 // タイトルバーにこの名前が表示されます
					//WS_CHILD | WS_SYSMENU | WS_THICKFRAME | WS_CAPTION ,	// ウィンドウの種類
					WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,		// ウィンドウの種類
					CW_USEDEFAULT,	//Ｘ座標
					CW_USEDEFAULT,	//Ｙ座標
					WindowW,	//幅
					WindowH,	//高さ
					hWnd,	// 親ウィンドウのハンドル、親を作るときはNULL
					NULL,	// メニューハンドル、クラスメニューを使うときはNULL
					hInst,		// インスタンスハンドル
					NULL);

			if (!child)
			{
				return FALSE;
			}

			ShowWindow(child, SW_SHOW);
			UpdateWindow(child);

			Log("デバッグウィンドウの作成に成功しました。");

			return true;

		}

		// デバッグテキストの作成
		static void Log(string str, Color color)
		{
			if (!IsDebug())
			{
				return;
			}

			textData.push_back(TextData(color, str));

			InvalidateRect(child, NULL, FALSE);
		}

		static void Log(string str)
		{
			Log(str, Color::White);
		}


	private:
		static bool IsDebug()
		{
			#ifndef _DEBUG
				return false;
			#endif

			return true;
		}

		static LRESULT CALLBACK DbgProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
		{
			PAINTSTRUCT ps;

			static HBITMAP hBitmap;	// BITMAPﾊﾝﾄﾞﾙ
			static HDC hdc_scr;

			static HFONT hFont;

			static HWND hBtn_save, hBtn_clear;
			static bool is_save_btn = true;

			switch(msg)
			{
				case WM_CREATE:
					{
						// HDCの作成
						HDC hdc = GetDC(hWnd);
						hBitmap = CreateCompatibleBitmap(hdc, WindowW, WindowH);
						hdc_scr = CreateCompatibleDC(hdc);
						SelectObject(hdc_scr, hBitmap);
						ReleaseDC(hWnd, hdc);   // HDCの解放

						hFont = CreateFont(16, 8, 0, 0, 
							0,
							false, false, false,
							DEFAULT_CHARSET,
							OUT_DEFAULT_PRECIS,
							CLIP_DEFAULT_PRECIS,
							ANTIALIASED_QUALITY,
							FIXED_PITCH | FF_DONTCARE,
							NULL);

						

						is_save_btn = true;

						hBtn_clear = CreateWindow("BUTTON", 
						"clear",
						WS_CHILD | WS_VISIBLE,
						100, 570, 100, 25,
						hWnd,
						(HMENU)BTN_CLEAR,
						hInst,
						NULL);

						hBtn_save = CreateWindow("BUTTON", 
						"save",
						WS_CHILD | WS_VISIBLE,
						400, 570, 100, 25,
						hWnd,
						(HMENU)BTN_SAVE,
						hInst,
						NULL);

						break;
					}


				case WM_PAINT:
					{
						SetBkMode(hdc_scr, TRANSPARENT);
	
						PatBlt(hdc_scr, 0, 0, WindowW, WindowH, BLACKNESS);	// 背景の描画
				
						for (int i = 0; ; i++)
						{
							int j = textData.size() - i - 1;
							int str_y = 545 - 20 * i;
							if (i >= (int)textData.size() || j < 0 || str_y < 0)
							{
								break;
							}

							COLORREF color;
							switch(textData.at(j).color)
							{
								case Color::White:
									color = RGB(255,255,255);
									break;
								case Color::Red:
									color = RGB(255,32,96);
									break;
								case Color::Blue:
									color = RGB(32,128,255);
									break;
								case Color::Yellow:
									color = RGB(255,255,0);
									break;
								case Color::Green:
									color = RGB(0,255,0);
									break;
								default:
									color = RGB(255,255,255);
									break;
							}
							SelectObject(hdc_scr, hFont);
							SetTextColor(hdc_scr, color);
							TextOut(hdc_scr, 10, str_y, textData.at(j).text.c_str(), lstrlen(textData.at(j).text.c_str()));
							SetTextColor(hdc_scr, RGB(0,0,0));
						}
				

						HDC hdc = BeginPaint(hWnd, &ps);
						BitBlt(hdc, 0, 0, WindowW, WindowH, hdc_scr, 0, 0, SRCCOPY);
			
						EndPaint(hWnd, &ps);
						break;
					}

				case WM_COMMAND:
					{
						switch(LOWORD(wp))
						{
							case BTN_CLEAR:
								{
									textData.clear();

									InvalidateRect(hWnd, NULL, FALSE);
									break;
								}
							case BTN_SAVE:
								{
									if(!is_save_btn)
									{
										break;
									}

									is_save_btn = false;

									if(MessageBox(NULL, "デバッグログを記録します…", "DEBUG WINDOW", MB_OK) == 1)
									{
										is_save_btn = true;
									}

									ofstream ofs("dx_debug_log.txt");

									if (!ofs)
									{
										// ファイルオープン失敗
										return -1;
									}

									for(int i = 0; i < (int)textData.size(); i++)
									{
										if(textData.at(i).text == "")
										{
											continue;
										}
										ofs << textData.at(i).text << endl;
									}

									ofs.close();

									break;
								}
						}
						break;
					}

				case WM_CLOSE:
					DestroyWindow(hBtn_save);
					DestroyWindow(hBtn_clear);
					DestroyWindow(hWnd);
					break;

				default:
					return (DefWindowProc(hWnd, msg, wp, lp));
			}

			return 0;
		}


};
