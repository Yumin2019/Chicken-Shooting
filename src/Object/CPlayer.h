#pragma once
#include "CMoveObj.h"

class CPlayer :
	public CMoveObj
{
private:
	CPlayer();
	~CPlayer();

private:
	friend class CInGameScene;

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Update(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);


private:

	// 개수

	int m_iShield;
	int m_iBomb;


	// Ball Count

	int m_iBallCount;
	int m_iBallLimitCount;

	// Ball Create Time & LimitTime

	float m_fBallCreateTime;  
	float m_fBallCreateLimitTime; // 0.6



	// MISAIL BULLET

	list<class CBullet*> m_MisailBullet;
	bool m_bMisail;

	// Misail Use : 미사일은 재사용 대기시간을 가지고 체크한다.

	float m_fMisailTime;
	float m_fMisailLimitTime;



	// StopTime
	float m_fStopTime; // 0.5초 동안 움직이지 않으면 idle모션

	
	// Shiel Use

	bool m_bShield;
	float m_fShieldTime;
	float m_fShieldLimitTime;

	// Bomb use

	bool m_bBomb;
	float m_fBombTime;
	float m_fBombLimitTime;
	

	// Bomb과 Shield는 1번씩 사용하도록 설정. ANimation


	/* 레벨의 개념 */

	// 스피드 : 속도 증가

	// 공격 : 공격 횟수(BallLimitCount++, BallCreateLimitTime 감소) / 미사일 LimitTime 감소 

	int m_iAttackLv;
	int m_iSpeedLv; // fir = 1 max = define 6

public:

	void SetAttackEnd()
	{
		m_iBallCount = 0;
		m_fBallCreateTime = 0.f;
	}

	float GetBombTime() const
	{
		return m_fBombTime;
	}

	float GetBombLimitTime() const
	{
		return m_fBombLimitTime;
	}

	float GetShieldTime() const
	{
		return m_fShieldTime;
	}

	float GetShieldLimitTime() const
	{
		return m_fShieldLimitTime;
	}


	// Ball Creat Time
	float GetBallCreateTime() const
	{
		return m_fBallCreateTime;
	}

	float GetBallCreateLimitTime() const
	{
		return m_fBallCreateLimitTime;
	}
	
	// BallCout

	

	int GetBallCount() const
	{
		return m_iBallCount;
	}

	int GetBallLimitCount() const
	{
		return m_iBallLimitCount;
	}
	
	// Shield Bomb remains

	int GetShieldCount() const
	{
		return m_iShield;
	}

	int GetBombCount() const
	{
		return m_iBomb;
	}

	void AddBombCount()
	{
		++m_iBomb;
	}

	void AddShieldCount()
	{
		++m_iShield;
	}


	// Misail Time
	float GetMisailTime() const
	{
		return m_fMisailTime;
	}

	float GetMisailLimitTime() const
	{
		return m_fMisailLimitTime;
	}

	// Attack , Speed Lv
	int GetAttackLv() const
	{
		return m_iAttackLv;
	}

	int GetSpeedLv() const
	{
		return m_iSpeedLv;
	}

	void AddAttackLv()
	{
		if (m_iAttackLv < ATTACK_LV_MAX)
		{
			++m_iAttackLv;
			m_fMisailLimitTime -= 0.5f; // 3.5f
			m_fBallCreateLimitTime -= 0.1f; // 0.1f
			++m_iBallLimitCount; // 8
		}
	}

	void AddSpeedLv()
	{
		if (m_iSpeedLv < SPEED_LV_MAX)
		{
			++m_iSpeedLv;
			m_fSpeed += 20.f; // max 100 550
		}
	}

public:
	list<class CBullet*>& GetMisailBulletList();


public:
	void PlayerAttacked();

	void SetBomb(bool bomb)
	{
		m_bBomb = bomb;
	}

	bool GetBomb() const
	{
		return m_bBomb;
	}

	void SetShield(bool shield)
	{
		m_bShield = shield;
	}

	bool GetShield() const
	{
		return m_bShield;
	}

private:

	void BallCreate();
	void Shield();
	void Misail();
	void Fire();
	void Bomb();



private:

	// 현재 바라보는 각도에 따라서 각도를 구해서 반환해준다.

	float GetDirAngle()
	{
		float fAngle = 0.f; // 0 = 0도

		switch (m_eDir)
		{
		case SD_RIGHT:
			break;

		case SD_LEFT:
			fAngle += PI;
			break;

		}
		
		return fAngle;
	}
};

