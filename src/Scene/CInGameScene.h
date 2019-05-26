#pragma once

#include "CScene.h"

class CInGameScene	:
	public CScene
{
private:
	CInGameScene();
	~CInGameScene();


	enum DIR
	{
		LEFT,
		TOP,
		RIGHT,
		BOTTOM
	};

private:
	friend class CSceneManager;


public:
	static class CPlayer* m_pPlayer;
	static class CBoss* m_pBoss;

	// 보스 등장 여부
	static bool m_bBoss;

	// 플레이어 등장 여부
	static bool m_bPlayer;

	// Game 종료에 사용할 변수.
	static float m_fGameTime;

	// Camera
	static float l;
	static float t;
	

private:
	WAVE_TYPE m_eWave; // 1 wave 2 wave 
	int m_iWaveLv; // 1~5 minion 6 Boss


	// 처음 몬스터가 출몰한 이후에 다음 몬스터가 출몰하기 까지의 시간.
	float m_fMonsterTime; 
	float m_fMonsterLimitTime;

	// Term Time
	float m_fTermTime;
	float m_fTermLimitTime;

	bool m_bWaveTerm; // WaveTerm을 생각해줘야 하는 때

private:

	void WaveSet();

	void Lv1();
	void Lv2();
	void Lv3();
	void Lv4();
	void Lv5();
	void Lv6();

	// 여기서 Pos란 l t r b 1 ~ 5
	void MinionACreate(POSITION tPos, bool XY, DIR eStart);
	void MinionBCreate(POSITION tPos, bool XY, DIR eStart);

	// Pos와 스타트 지점을 넣어주면 조정해서 위치를 반환
	POSITION MovePos(DIR eStart, POSITION tPos);

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Update(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
};

