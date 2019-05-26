#pragma once
#include "CBullet.h"


class CBulletA :
	public CBullet
{
public:
	CBulletA();
	CBulletA(CBulletA& bullet);
	~CBulletA();


private:
	// BulletA Àü¿ë Timer

	float m_fTime;
	float m_fLimitTime;

public:
	void SetEffect();

	void SetLimitTime(float f)
	{
		m_fLimitTime = f;
	}


	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Update(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual CBulletA* Clone();
};

