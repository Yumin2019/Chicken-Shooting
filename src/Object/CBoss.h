
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

	// �нú�
	float m_fPassiveTime; // 0 ~  
	float m_fPassiveLimitTime; // 20s

	// �⺻ ������ m_fBulletLimitTime�� �̿��Ѵ�.

	BOSS_ATTACK m_eAttack;

	int m_iCount;
	int m_iMaxCount;

	// �����ϴ� �ð����� ��� ���߱� ���� ����

	bool m_bStop;
	float m_fStopTime;
	float m_fStopLimitTime;


	float m_fXYTime;
	float m_fXYLimitTime;

	// ���� ��� 
	bool m_bAttackMotion;

	// ���� ����� ������ �Ǿ��ٸ� true 
	// ���Ŀ� ���� ���� �ϳ��� ��� �����ٸ� false

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
