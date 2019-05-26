#pragma once
#include "CMonster.h"

class CMinionA :
	public CMonster
{
private:
	CMinionA();
	CMinionA(const CMinionA& mon);
	~CMinionA();


private:
	friend class CCreateManager;

private:

	float m_fBulletTime2;
	float m_fBulletLimitTime2;

	float m_tTime;

public:

	void SetBulletTime2(float tSecond)
	{
		m_fBulletLimitTime2 = tSecond;
	}

	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Update(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual CMinionA* Clone();

};

