#pragma once
#include "CObj.h"

class CBullet :
	public CObj
{
protected:
	CBullet();
	CBullet(const CBullet& bullet);
	virtual ~CBullet();


public:
	static bool m_bBossXY;


protected:

	bool m_bEnable;

	float m_fDist;
	float m_fLimitDist;
	BULLET_TYPE m_eBulletType;

	int m_iDamage;

public:

	bool GetBulletEnable() const
	{
		return m_bEnable;
	}

	void SetBulletEnable(bool enable)
	{
		m_bEnable = enable;
	}

	void AddDamage(int iDamage)
	{
		m_iDamage += iDamage;
	}

	void SetDamage(int iDamage)
	{
		m_iDamage = iDamage;
	}

	int GetDamage() const
	{
		return m_iDamage;
	}

	void SetLimitDist(float fDist)
	{
		m_fLimitDist = fDist;
	}

	void SetBulletType(BULLET_TYPE eType)
	{
		m_eBulletType = eType;
	}

	BULLET_TYPE GetBulletType() const
	{
		return m_eBulletType;
	}


public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Update(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual CBullet* Clone();


	void RandomItem(POSITION tPos);

	// Bullet List 2���� �־��ָ� ��� �Ѿ��� �ű�� �Լ�.
	void BulletMove(list<class CBullet*>& pSrc, list<class CObj*>& pDest); 


protected:
	void SkillToBullet(list<class CBullet*>& MonsterBulletList); // Skill Vs Monster's Bullet
	CObj* GetLiveMonster(list<CObj*>& MonsterList); // ����ִ� ���͸� ã�Ƽ� ��ȯ���ִ� �Լ�.
	void CreateMinionA();
	void CreateMinionB();
	// FlowerDie Effect����. Bullet��ġ����

public:
	void CreateFlowerDie();
};

