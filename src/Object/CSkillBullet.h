#pragma once
#include "CBullet.h"
class CSkillBullet :
	public CBullet
{
public:
	CSkillBullet();
	CSkillBullet(CSkillBullet& bullet);
	virtual ~CSkillBullet();


public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Update(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual CSkillBullet* Clone();


private:
	float m_fLifeLimitTime;
	float m_fLifeTime;



	// Skill Bullet Timer & Angle Timer
	float m_fAngleTime;
	float m_fAngleLimitTime;
	int	  m_iCount;


private:
	friend class CPlayer;

private:

	// 가장 가까운 적을 뽑아오는 함수.
	class CObj* FindAdjustMonster();

	void MisailCreate();
	void BulletChange();
	void CircleCreate();
	void BossSpecial1_2(); 
	void CreateFlower();

public:

	// Life Time
	void SetLifeLimitTime(float f)
	{
		m_fLifeLimitTime = f;
	}

	float GetLifeTime() const
	{
		return m_fLifeTime;
	}


	// AngleLimit Time
	void SetAngleLimitTime(float f)
	{
		m_fAngleLimitTime = f;
	}

	float GetAngleLimitTime() const
	{
		return m_fAngleLimitTime;
	}


	// Angle Time
	void AddAngleTime(float f)
	{
		m_fAngleTime += f;
	}

	float GetAngleTime() const
	{
		return m_fAngleTime;
	}

	
};

