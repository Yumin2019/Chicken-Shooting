
#include "CMonster.h"


class CBoss :
	public CMonster
{
private:
	CBoss();
	CBoss(const CBoss& mon);
	~CBoss();


private:
	friend class CCreateManager;

private:

	enum BOSS_ATTACK
	{
		BA_NORMAL,
		BA_SPECIAL1,
		BA_SPECIAL2
	};

private:

	bool m_bDie;

	// 패시브
	float m_fPassiveTime; // 0 ~  
	float m_fPassiveLimitTime; // 20s

	// 기본 공격은 m_fBulletLimitTime을 이용한다.

	BOSS_ATTACK m_eAttack;

	int m_iCount;
	int m_iMaxCount;

	// 공격하는 시간에는 잠깐 멈추기 위한 변수

	bool m_bStop;
	float m_fStopTime;
	float m_fStopLimitTime;


	float m_fXYTime;
	float m_fXYLimitTime;

	// 공격 모션 
	bool m_bAttackMotion;

	// 공격 모션이 실행이 되었다면 true 
	// 이후에 공격 패턴 하나가 모두 끊났다면 false

	bool m_bTerm;
	float m_fTermTime;
	float m_fTermLimitTime;

private:
	void NormalAttack();

	void SpecialAttack1();

	void SpecialAttack2();

	void NormalAttackSet();

	void SpecialAttack1Set();

	void SpecialAttack2Set();

	void PassiveEffect();

public:

	void SetBossDie()
	{
		m_bDie = true;
	}

	void SetAttackMotion(bool attack)
	{
		m_bAttackMotion = attack;
	}

	bool GetAttackMotion() const
	{
		return m_bAttackMotion;
	}

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Update(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual CBoss* Clone();

	
};
