#include "CTimer.h"

DEFINITION_SINGLE(CTimer);

float CTimer::m_fFPS;

CTimer::CTimer()
{
}


CTimer::~CTimer()
{
}

bool CTimer::Init(HWND hWnd)
{
	m_hWnd = hWnd;

	m_fDeltaTime = 0.f;
	QueryPerformanceFrequency(&m_tSecond);
	QueryPerformanceCounter(&m_tTime);

	m_fFPSTime = 0.f;
	m_fFPS = 0.f;
	m_iFrame = 0;

	return true;
}

void CTimer::Update()
{
	LARGE_INTEGER tTime;
	QueryPerformanceCounter(&tTime);

	m_fDeltaTime = (tTime.QuadPart - m_tTime.QuadPart) / (float)m_tSecond.QuadPart;

	m_tTime = tTime;


	m_fFPSTime += m_fDeltaTime;

	++m_iFrame;

	if (m_fFPSTime >= 1.f)
	{
		m_fFPS = m_iFrame / m_fFPSTime;
		m_fFPSTime = 0.f;
		m_iFrame = 0;

	}
}
