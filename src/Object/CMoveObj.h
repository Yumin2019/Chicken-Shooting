
#pragma once
#include "CObj.h"

class CMoveObj :
	public CObj
{
protected:
	CMoveObj();
	CMoveObj(const CMoveObj& move);
	virtual ~CMoveObj();


protected:
	friend class CBullet;



protected:
	list<class CBullet*> m_BulletList;
	STAND_DIRECTION m_eDir;
	float m_fAdjustDist;

	int m_iHP;
	int m_iHPMax;

	bool m_bAttacked;




protected:
	// Bullet Sort

	static bool BulletSort(class CBullet* bullet1, class CBullet* bullet2);


public:

	void AddHP(int iDamage);
	

	// Attacked

	bool GetAttacked() const
	{
		return m_bAttacked;
	}

	void SetAttacked(bool attack)
	{
		m_bAttacked = attack;
	}



	int GetHP() const
	{
		return m_iHP;
	}

	int GetHPMax() const
	{
		return m_iHPMax;
	}

	void SetHP(int ihp)
	{
		m_iHP = ihp;
		m_iHPMax = ihp;
	}


	// STAND_DIRECTION

	STAND_DIRECTION GetStandDir() const
	{
		return m_eDir;
	}

	void SetStandDir(STAND_DIRECTION eDir)
	{
		m_eDir = eDir;
	}

	// Bullet List를 가져오는 함수

	list<class CBullet*>& GetBulletList()
	{
		return m_BulletList;
	}

	// 몬스터로부터 총알이 생성되는 지점의 빗변 길이

	void SetAdjustDist(float fDist)
	{
		m_fAdjustDist = fDist;
	}


	float GetAdjustDist() const
	{
		return m_fAdjustDist;
	}


public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Update(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);

	virtual CMoveObj* Clone();
};












