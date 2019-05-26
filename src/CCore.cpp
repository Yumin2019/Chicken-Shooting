#include "Core\CCore.h"
#include "Core/CTimer.h"
#include "Core\CSceneManager.h"
#include "Core/CCreateManager.h"
#include "Core/CInput.h"
#include "Object\CObj.h"


DEFINITION_SINGLE(CCore);
bool CCore::m_bLoop;

CCore::CCore()
{
	//_CrtSetBreakAlloc(648);

}


CCore::~CCore()
{
	DESTROY_SINGLE(CTimer);
	DESTROY_SINGLE(CSceneManager);
	DESTROY_SINGLE(CCreateManager);
	DESTROY_SINGLE(CInput);

	SelectObject(m_hBackDC, OldFont);
	DeleteObject(Font);

	DeleteDC(m_hBackDC);
	ReleaseDC(m_hWnd, m_hDC);
	DeleteObject(m_hBackBitmap);


	
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); 

}

bool CCore::Init(HINSTANCE hInst)
{
	m_hInst = hInst;

	m_bLoop = true;
	
	MyRegisterClass();

	InitInstance();


	// 화면용 DC
	m_hDC = GetDC(m_hWnd);

	// 화면용 DC와 호환이 되는 메모리 DC 생성
	m_hBackDC = CreateCompatibleDC(m_hDC);

	// 메모리 DC에 정확한 정보를 생성
	m_hBackBitmap = CreateCompatibleBitmap(m_hDC, RS_WIDTH, RS_HEIGHT);

	// 생성한 정보를 가지고 BackDC에서 선택
	SelectObject(m_hBackDC, m_hBackBitmap);

	// 폰트 생성
	FontCreate(m_hBackDC);
	

	// 시간 관리자 초기화
	if (!GET_SINGLE(CTimer)->Init(m_hWnd))
		return false;

	// 입력 관리자 초기화
	if (!GET_SINGLE(CInput)->Init())
		return false;

	// 생성 관리자 초기화
	if (!GET_SINGLE(CCreateManager)->Init(m_hInst, m_hBackDC))
		return false;

	// 장면 관리자 초기화
	if (!GET_SINGLE(CSceneManager)->Init(m_hWnd))
		return false;

	return true;
}

int CCore::Run()
{

	MSG msg;

	// 기본 메시지 루프입니다.
	while (m_bLoop)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Logic();
		}
	}

	return (int)msg.wParam;
}

void CCore::Logic()
{
	// Timer 업데이트
	GET_SINGLE(CTimer)->Update();

	float fDeltaTime = GET_SINGLE(CTimer)->GetDeltaTime();

	Input(fDeltaTime);
	
	Collision(fDeltaTime);

	if (Update(fDeltaTime) == CS_CHANGE)
		return;

	Render(fDeltaTime);

}

ATOM CCore::MyRegisterClass()
{

	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = CCore::WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_hInst;
	wcex.hIcon = LoadIcon(m_hInst, MAKEINTRESOURCE(RUBY));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL; 
	wcex.lpszClassName = L"Chicken Shooting";
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(RUBY));

	return RegisterClassExW(&wcex);
}

BOOL CCore::InitInstance()
{
	 m_hWnd = CreateWindowW(L"Chicken Shooting", L"Chicken Shooting", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, m_hInst, nullptr);

	if (!m_hWnd)
	{
		return FALSE;
	}

	RECT rc = { 0, 0, RS_WIDTH, RS_HEIGHT };

	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, rc.right - rc.left,
		rc.bottom - rc.top, SWP_NOZORDER);

	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	return TRUE;
}

LRESULT CCore::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

			case WM_PAINT:
			{
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hWnd, &ps);
				EndPaint(hWnd, &ps);
			}
			break;
			case WM_DESTROY:
				m_bLoop = false;
				PostQuitMessage(0);
				break;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}

	return 0;
}

void CCore::Input(float fDeltaTime)
{

	if(KEYDOWN("Collider"))
		CObj::m_bCollider = !CObj::m_bCollider;

	GET_SINGLE(CInput)->Update(fDeltaTime);
	GET_SINGLE(CSceneManager)->Input(fDeltaTime);
}

void CCore::Collision(float fDeltaTime)
{
	GET_SINGLE(CSceneManager)->Collision(fDeltaTime);

}

int CCore::Update(float fDeltaTime)
{
	CHANGE_SCENE sc;
	sc = GET_SINGLE(CSceneManager)->Update(fDeltaTime);

	return sc;
}

void CCore::Render(float fDeltaTime)
{
	
	GET_SINGLE(CSceneManager)->Render(m_hBackDC, fDeltaTime);

	 //BackDC에 적은 것을 m_hDC에 옮긴다.

	BitBlt(m_hDC, 0, 0, RS_WIDTH, RS_HEIGHT, m_hBackDC,  
		 0, 0, SRCCOPY );

	if (CObj::m_bCollider)
	{
		TCHAR strFPS[20];
		swprintf_s(strFPS, L"FPS : %.f", CTimer::m_fFPS);

		TextOut(m_hDC, RS_WIDTH - 100.f, RS_HEIGHT - 50.f, strFPS, lstrlen(strFPS));
	}
}

void CCore::FontCreate(HDC hDC)
{
	LOGFONT lf;

	lf.lfHeight = 42;
	lf.lfWidth = 0;
	lf.lfEscapement = 0;
	lf.lfOrientation = 0;
	lf.lfWeight = FW_NORMAL;
	lf.lfItalic = 0;
	lf.lfUnderline = 0;
	lf.lfStrikeOut = 0;
	lf.lfCharSet = ANSI_CHARSET;
	lf.lfOutPrecision = 0;
	lf.lfClipPrecision = 0;
	lf.lfQuality = 0;
	lf.lfPitchAndFamily = 0;
	lstrcpy(lf.lfFaceName, L"GameFont");

	Font = CreateFontIndirect(&lf);

	OldFont = (HFONT)SelectObject(hDC, Font);

	SetBkMode(hDC, TRANSPARENT); // 배경 투명
}

