#pragma once
#include "CMonster.h"


class CMinionB :
	public CMonster
{

private:
	CMinionB();
	CMinionB(const CMinionB& mon);
	virtual ~CMinionB();

private:
	float m_tTime;


private:
	friend class CCreateManager;

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Update(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);

	virtual CMinionB* Clone();


};

