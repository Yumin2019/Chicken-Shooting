#pragma once

#include "../Game.h"

class CCore
{
	DECLARE_SINGLE(CCore);

private:
	HINSTANCE	 m_hInst;
	HWND		 m_hWnd;
	HDC			 m_hDC;
	HDC			 m_hBackDC;
	HBITMAP		 m_hBackBitmap;
	HFONT Font, OldFont;

	static bool	 m_bLoop;

public:
	bool Init(HINSTANCE hInst);
	int Run();
	void Logic();

	
	void DestroyGame()
	{
		DestroyWindow(m_hWnd);
	}

private:
	ATOM MyRegisterClass();
	BOOL InitInstance();
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void Input(float fDeltaTime);
	void Collision(float fDeltaTime);
	int Update(float fDeltaTime);
	void Render(float fDeltaTime);
	void FontCreate(HDC hDC);
};

