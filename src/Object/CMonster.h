#pragma once
#include "CMoveObj.h"

class CMonster :
	public CMoveObj
{
protected:
	CMonster();
	CMonster(const CMonster& mon);
	virtual ~CMonster();

protected:

	// true = x
	// false = y
	// X Y ���� �̵��ϴ��� �ν��� bool

	bool m_bXY; 

	float m_fBulletTime;
	float m_fBulletLimitTime;



public:

	// �Ѿ� �߻� �ð� ���� n�ʿ� 1��
	void SetBulletTime(float tSecond)
	{
		m_fBulletLimitTime = tSecond;
	}

	bool GetXY() const
	{
		return m_bXY;
	}

	void SetXY(bool b)
	{
		m_bXY = b;
	}

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Update(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);

	virtual CMonster* Clone();


};

