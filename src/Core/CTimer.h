#pragma once

#include "../Game.h"


class CTimer
{
	DECLARE_SINGLE(CTimer);

public:
	bool Init(HWND hWnd);
	void Update();

	float GetDeltaTime() const
	{
		return m_fDeltaTime;
	}

	
public:
	static float m_fFPS;

private:
	LARGE_INTEGER m_tTime, m_tSecond;
	float m_fDeltaTime;

	float m_fFPSTime;
	float m_iFrame;

	HWND m_hWnd;


};

