// Direct2dTemplate.cpp : �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
// 

#include "stdafx.h"
#include "Direct2dTemplate.h"
#include "D2DWindow.h"
#include "D2DWindowControl.h"
#include "gdi32.h"
using namespace GDI32; 


#define LIGHTWINDOW

#define MAX_LOADSTRING 100

// �O���[�o���ϐ�:
HINSTANCE __hInst;								// ���݂̃C���^�[�t�F�C�X
TCHAR __szTitle[MAX_LOADSTRING];					// �^�C�g�� �o�[�̃e�L�X�g
TCHAR __szWindowClass[MAX_LOADSTRING];			// ���C�� �E�B���h�E �N���X��
#define IDLE_TIME (5*1000)
#define IDLE_TIMER_ID 99


using namespace V4;

D2DWindow window; 


//������////////////////////////////////////////////////
void CreateChild(V4::D2DWindow* parent);
//������///////////////////////////////////////////////




// ���̃R�[�h ���W���[���Ɋ܂܂��֐��̐錾��]�����܂�:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
void				ExitInstance();
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	MSG msg;
	HACCEL hAccelTable;

	CoInitialize(0);
	_tsetlocale ( 0, _T("japanese") ); 	 
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

	// �O���[�o������������������Ă��܂��B
	LoadString(hInstance, IDS_APP_TITLE, __szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_DIRECT2DTEMPLATE, __szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// �A�v���P�[�V�����̏����������s���܂�:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DIRECT2DTEMPLATE));

	// ���C�� ���b�Z�[�W ���[�v:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

//Exit:
	ExitInstance();
	return (int) msg.wParam;
}



//
//  �֐�: MyRegisterClass()
//
//  �ړI: �E�B���h�E �N���X��o�^���܂��B
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW |CS_DBLCLKS;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DIRECT2DTEMPLATE));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= NULL; 
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= __szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}


//
//   �֐�: InitInstance(HINSTANCE, int)
//
//   �ړI: �C���X�^���X �n���h����ۑ����āA���C�� �E�B���h�E���쐬���܂��B
//
//   �R�����g:
//
//        ���̊֐��ŁA�O���[�o���ϐ��ŃC���X�^���X �n���h����ۑ����A
//        ���C�� �v���O���� �E�B���h�E���쐬����ѕ\�����܂��B
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{


   __hInst = hInstance; // �O���[�o���ϐ��ɃC���X�^���X�������i�[���܂��B

   HWND hMainFrameWnd = CreateWindowW(__szWindowClass, __szTitle, WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

   if (!hMainFrameWnd )
   {
      return FALSE;
   }

   ShowWindow(hMainFrameWnd, nCmdShow);
   UpdateWindow(hMainFrameWnd);

   return TRUE;

}
void ExitInstance()
{
	CoUninitialize();
}

// �o�[�W�������{�b�N�X�̃��b�Z�[�W �n���h���[�ł��B
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



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// �I�����ꂽ���j���[�̉��:
		switch (wmId)
		{
			case IDM_ABOUT:
				DialogBox(__hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
				break;
			case IDM_EXIT:
				DestroyWindow(hWnd);
				break;
		
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_CREATE:
	{
		FRect rc;
		window.OnCreate = CreateChild;
		HWND hwnd = window.CreateD2DWindow( 0, hWnd, WS_CHILD|WS_VISIBLE, rc );

		/*
		auto IdleMessage = [](HWND hwnd, UINT msg, UINT_PTR id, DWORD time )
		{
			if ( IDLE_TIMER_ID == id )
				SendMessage(hwnd, WM_D2D_IDLE, 0, 0);
		};
		
		::SetTimer( hWnd, IDLE_TIMER_ID, IDLE_TIME, IdleMessage );*/
	}
	break;
	case WM_SIZE:
	{
		FSize sz(lParam);
		::MoveWindow( window.hWnd_, 0,0, sz.cx, sz.cy, TRUE );
	}
	break;
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
	break;
	case WM_ERASEBKGND :
	{			
		//InvalidateRect(window.hWnd_,NULL,FALSE); 
		return 1;
	}
	break;
	
	default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////


void CreateChild(V4::D2DWindow* parent)
{
	D2DTopControls* cs = new D2DTopControls();

	FRectF rc2;
	cs->CreateWindow(parent, NULL, rc2, VISIBLE, L"1 layer");


	FRectF rcbutton(10,10,FSizeF(200,60));
	D2DButton* btn = new D2DButton();
	btn->CreateWindow( parent, cs, rcbutton, VISIBLE|BORDER,L"btn1" );


	btn->OnClick_ = [cs](D2DButton* btn)
	{
		//::MessageBox( btn->parent_->hWnd_, L"Hello world", L"msg", MB_OK );

		D2DFrameWindowControl* x = new D2DFrameWindowControl();

		FRectF rc(100,100,FSizeF(500,300));

		x->CreateWindow( btn->parent_, cs, rc, VISIBLE|BORDER, L"NONAME" );


	};
}