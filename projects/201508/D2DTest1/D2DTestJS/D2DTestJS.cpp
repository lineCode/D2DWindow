﻿/*
The MIT License (MIT)
Copyright (c) 2015 admin@sugarontop.net
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/


#include "stdafx.h"
#include "D2DTestJS.h"
#include "D2DWin.h"
#include "s1.h"
#include "JsrtWrapper.h"
#include <Commdlg.h>
#define MAX_LOADSTRING 100

// グローバル変数:
HINSTANCE hInst;                                // 現在のインターフェイス
WCHAR szTitle[MAX_LOADSTRING];                  // タイトル バーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING];            // メイン ウィンドウ クラス名

// このコード モジュールに含まれる関数の宣言を転送します:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	CoInitialize(0);
	_tsetlocale(0, _T("japanese"));
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    // グローバル文字列を初期化しています。
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_D2DTESTJS, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // アプリケーションの初期化を実行します:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_D2DTESTJS));

    MSG msg;

    // メイン メッセージ ループ:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスを登録します。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_D2DTESTJS));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_D2DTESTJS);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   関数: InitInstance(HINSTANCE, int)
//
//   目的: インスタンス ハンドルを保存して、メイン ウィンドウを作成します。
//
//   コメント:
//
//        この関数で、グローバル変数でインスタンス ハンドルを保存し、
//        メイン プログラム ウィンドウを作成および表示します。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // グローバル変数にインスタンス処理を格納します。

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

struct Global
{
	~Global()
	{
		DDDestroyWindow(g_handle);
	}
	D2DWin g_handle;
};

Global global;


D2Ctrl OutputControl;

using namespace V4;
namespace CHAKRA2 {
	void Initialize(JSRuntime& grun, JSContext& rcxt);
	bool JavaScriptRun(JSContext& r, LPCWSTR src);
	void Destroy(JSRuntime);

	extern wstring ErrorMsg;
};
JSContext gcxt;
JSRuntime grun;
void EportFunctions();
std::map<wstring, D2Ctrl> wmap;
std::map<wstring, D2Ctrls> wbase;

//
//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND  - アプリケーション メニューの処理
//  WM_PAINT    - メイン ウィンドウの描画
//  WM_DESTROY  - 中止メッセージを表示して戻る
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 選択されたメニューの解析:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: HDC を使用する描画コードをここに追加してください...
            EndPaint(hWnd, &ps);
        }
        break;
	case WM_CREATE:
	{
		auto DDDControlCreate = [hWnd](D2DWin w)
		{
			


			auto t = DDMkTopControls(w, V4::FRectF(0, 0, 0, 0), NONAME);

			auto tab = DDMkControls(t, V4::FRectF(0, 0, 0, 0), L"tab", NONAME);
			auto t0 = DDGetPage(tab, 0);

			wbase[L"page0"] = t0;


			V4::FRectFBoxModel rctxt(20,60, V4::FSizeF(500, 450));
			rctxt.Padding_.Set(3);

			rctxt.Padding_.r = 15; // scroll bar

			auto txm = DDMkTextbox(t0, rctxt, 1, NONAME);
			DDSetText(txm, L"var a=1+2; echo(a); //Script window. Input Javascript source. ..");

			std::map<std::wstring,VARIANT> prm;
			prm[L"forecolor"] = _variant_t((LONG)D2DRGBADWORD(0,0,200,255));
			prm[L"backcolor"] = _variant_t((LONG)D2DRGBADWORD(190, 190, 190, 155));
						

			DDSetParameter( txm, prm );


			auto otxm = DDMkTextbox(t0, V4::FRectF(150, 520, V4::FSizeF(500, 150)), 1, L"output");
			DDSetText(otxm, L"output");
			OutputControl = otxm;

			wmap[L"output"] = otxm;


			auto btnClear = DDMkButton(t0, FRectFBM(20, 600, V4::FSizeF(100, 30)), L"Clear"); 
			DDEvent0( D2EVENT0_MODE::CLICK, btnClear, [txm, otxm](D2EVENT0_MODE ev, D2Ctrl){
				DDSetText( txm, L"" );
				DDSetText(otxm, L"");
			});
			
			


			auto btnRun = DDMkButton(t0,FRectFBM(20,550,V4::FSizeF(100,30)), L"Run" );
			DDEvent0(D2EVENT0_MODE::CLICK, btnRun, [txm, otxm](D2EVENT0_MODE ev, D2Ctrl)
			{
				BSTR src = DDGetText(txm);

				try
				{ 

					if (!CHAKRA2::JavaScriptRun(gcxt, src))
					{
						
						DDSetText(otxm, CHAKRA2::ErrorMsg.c_str());
					}
				}
				catch( V4::FString er )
				{
					DDSetText( otxm, er );
				}

				::SysFreeString(src);

			});
					

			auto btnLoad = DDMkButton(t0, FRectFBM(20, 650, V4::FSizeF(100, 30)), L"Load");
			DDEvent0(D2EVENT0_MODE::CLICK, btnLoad, [hWnd,txm](D2EVENT0_MODE ev, D2Ctrl)
			{
				/*OPENFILENAME ofn;

				WCHAR cb[64]; lstrcpy( cb, L"js" );
				WCHAR cb2[MAX_PATH]; lstrcpy(cb2, L"*.js");
				WCHAR cb3[64]; lstrcpy(cb3, L"*.js");
				WCHAR cb4[MAX_PATH]; 
				WCHAR cb5[64]; lstrcpy(cb5, L"test");

				GetCurrentDirectory( MAX_PATH,cb4 );

				ZeroMemory(&ofn, sizeof(ofn));
				
				ofn.lStructSize = sizeof(ofn);
				ofn.hwndOwner = hWnd; 
				ofn.lpstrDefExt = cb;
				ofn.lpstrFile = cb2;
				ofn.nMaxFile = MAX_PATH;
				ofn.lpstrFilter = cb3;
				ofn.nFilterIndex = 0;
				ofn.lpstrInitialDir = cb4;
				ofn.lpstrTitle = cb5;
				ofn.Flags =  OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
				
				if ( ::GetOpenFileName(&ofn) )
				{
					wstring fnm = ofn.lpstrFile;
					
					BSTR src;
					DDOpenTextFile( fnm.c_str(), &src );

					DDSetText( txm, src );

					::SysFreeString(src);

				}*/

				BSTR src;
				if ( DDOpenTextFile( L"readme.txt", &src))
				{

					DDSetText(txm, src);

					::SysFreeString(src);
				}


			});
			
			
			CHAKRA2::Initialize(grun, gcxt);
			
			JSContext::Scope ct(gcxt);
			EportFunctions();
		};

		global.g_handle = DDMkWindow(hWnd, DDDControlCreate);

		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	break;
	case WM_SIZE:
	{
		FSizeF sz(lParam);
		
		DDResizeWindow( global.g_handle, sz.width, sz.height );

		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	break;

    case WM_DESTROY:
		CHAKRA2::Destroy(grun);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// バージョン情報ボックスのメッセージ ハンドラーです。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
