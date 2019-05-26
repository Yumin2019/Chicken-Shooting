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

	// ���� ���� ����
	static bool m_bBoss;

	// �÷��̾� ���� ����
	static bool m_bPlayer;

	// Game ���ῡ ����� ����.
	static float m_fGameTime;

	// Camera
	static float l;
	static float t;
	

private:
	WAVE_TYPE m_eWave; // 1 wave 2 wave 
	int m_iWaveLv; // 1~5 minion 6 Boss


	// ó�� ���Ͱ� ����� ���Ŀ� ���� ���Ͱ� ����ϱ� ������ �ð�.
	float m_fMonsterTime; 
	float m_fMonsterLimitTime;

	// Term Time
	float m_fTermTime;
	float m_fTermLimitTime;

	bool m_bWaveTerm; // WaveTerm�� ��������� �ϴ� ��

private:

	void WaveSet();

	void Lv1();
	void Lv2();
	void Lv3();
	void Lv4();
	void Lv5();
	void Lv6();

	// ���⼭ Pos�� l t r b 1 ~ 5
	void MinionACreate(POSITION tPos, bool XY, DIR eStart);
	void MinionBCreate(POSITION tPos, bool XY, DIR eStart);

	// Pos�� ��ŸƮ ������ �־��ָ� �����ؼ� ��ġ�� ��ȯ
	POSITION MovePos(DIR eStart, POSITION tPos);

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Update(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
};

