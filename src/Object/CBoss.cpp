#include "CBoss.h"
#include "CSkillBullet.h"
#include "../Core/CCreateManager.h"
#include "../Math.h"
#include "../Animation/CAnimation.h"
#include "CBulletA.h"
#include "../Core/CSceneManager.h"
#include "../Scene/CInGameScene.h"


CBoss::CBoss() :
	m_fPassiveTime(0.f),
	m_iCount(0),
	m_fStopTime(0.f),
	m_bStop(false),
	m_fXYTime(0.f),
	m_bAttackMotion(false),
	m_eAttack(BA_NORMAL),
	m_bTerm(false),
	m_fTermTime(0.f),
	m_bDie(false)
{


}

CBoss::CBoss(const CBoss & mon) :
	CMonster(mon)
{
	m_bDie = false;

	SetPos(FULL_WIDTH/2.f, FULL_HEIGHT/2.f);

	
	int idx = rand() % 2;

	switch (idx)
	{
	case 0: m_eDir = SD_RIGHT; break;
	case 1: m_eDir = SD_LEFT; break;
	}

	idx = rand() % 2;

	switch (idx)
	{
	case 0: m_bXY = true; break;
	case 1: m_bXY = false; break;
	}


	// ����

	switch (m_eDir)
	{
	case SD_LEFT:

		switch (m_bXY)
		{
		case true: // x�� ����

			m_pAnimation->SetDefaultClip("BossLeft");

			break;

		case false: // y�� Up

			m_pAnimation->SetDefaultClip("BossUp");

			break;
		}


		break;

	case SD_RIGHT:

		switch (m_bXY)
		{
		case true: // x�� right

			m_pAnimation->SetDefaultClip("BossRight");

			break;

		case false: // y�� down

			m_pAnimation->SetDefaultClip("BossDown");

			break;
		}
	}

	// �нú�
	m_fPassiveTime = 0.f;
	m_fPassiveLimitTime = mon.m_fPassiveLimitTime;


	// ���� �ð�
	m_fStopTime = 0.f;
	m_fStopLimitTime = mon.m_fStopLimitTime;

	m_bStop = false;


	// �̵��� ���Ͽ�
	m_fXYTime = 0.f;
	m_fXYLimitTime = mon.m_fXYLimitTime;


	// ó�� ���� Ÿ���� NORMAL
	NormalAttackSet();
	m_eAttack = BA_NORMAL;

	// �� �ð�.
	m_fTermTime = 0.f;
	m_fTermLimitTime = 0.f;
	m_bTerm = false;

	m_iCount = 0;

	m_bAttackMotion = false;

}


CBoss::~CBoss()
{
}

void CBoss::NormalAttack()
{

	// �⺻ ���� ����
	float fAngle = Math::GetAngle(this, (CObj*)PLAYER); 

	CSkillBullet* pBullet = FIND(CSkillBullet)->FindObj("BossNormalBullet")->Clone();

	pBullet->SetAngle(fAngle); // �÷��̾ ����

							   // ���� ��ġ ���ϱ�
	POSITION m_tPos = GetBulletPos(GetCenter(), GetAdjustDist(), fAngle, pBullet);


	pBullet->SetPos(m_tPos); // Pos 

	pBullet->SetBulletType(BT_BOSS_NORMAL);
	
	m_BulletList.push_back(pBullet);


}

void CBoss::SpecialAttack1()
{

	// 3���� ��µ� �ߺ��� ������ �� �ȴ�.

	bool bStart[4] = {};

	CBullet::m_bBossXY = !CBullet::m_bBossXY;

	for (int i = 0; i < 3; ++i)
	{

		CSkillBullet* pBullet = FIND(CSkillBullet)->FindObj("BossSpecialAttack1_1")->Clone();

		POSITION tSrc;

		int idx;

		while (true)
		{
			idx = rand() % 4;

			if (!bStart[idx])
			{
				bStart[idx] = true;
				break;
			}
		}

		switch (idx)
		{
		case 0: tSrc = SRC_1; break;
		case 1: tSrc = SRC_2; break;
		case 2: tSrc = SRC_3; break;
		case 3: tSrc = SRC_4; break;
		}

		float fAngle = Math::GetAngle(tSrc, DEST);

		pBullet->SetAngle(fAngle);

		pBullet->SetPos(tSrc);

		pBullet->SetBulletType(BT_BOSS_SPECIAL1_1);

		m_BulletList.push_back(pBullet);

		// define�� �̿��ؼ� ������ �� ������ �Ѵ�.
	}


}

void CBoss::SpecialAttack2()
{


	// 0 ~ 11���� �ߺ��־ ������� 10�� ( �ð��� �����̶� �ѹ��� ) �� ����
	// ���⼭�� 0 ~ 10�� �ϳ� ���, * 100�� �Ͽ� y�� ��ǥ�� �̿��Ѵ�. FULL_HEIGHT
	
	// x ��ǥ�� �߽ɿ��� -600 + 600���� ����.
	// 900 ���� ���, ������ ��ġ�� ���� ������ . 100������.

	// 7 ~ 11�߿� �ϳ� �̾Ƽ� * 100.

	float x, y;

	y = rand() % 11; // 0 ~ 10

	x = rand() % 13 + 3; // 3 15

	POSITION tPos = POSITION(x * 100.f, y * 100.f);

	CBulletA* pEffect = FIND(CBulletA)->FindObj("Death")->Clone();

	pEffect->SetAnimation("DeathIdle"); // Idle�� ���� -> 2�� ����(2��) -> time_return-> Die�� ���� -> 1��(1��) �����.

	pEffect->SetPos(tPos);

	GET_SINGLE(CSceneManager)->GetSecondEffectList().push_back(pEffect);

	pEffect->CreateFlowerDie();

}

void CBoss::NormalAttackSet()
{
	m_fBulletLimitTime = 3.5f;
	m_iMaxCount = 10;
	m_fTermLimitTime = 2.5f;
}

void CBoss::SpecialAttack1Set()
{
	m_fBulletLimitTime = 6.f; 
	m_iMaxCount = 1;
	m_fTermLimitTime = 10.f;

}

void CBoss::SpecialAttack2Set()
{
	m_fBulletLimitTime = 0.5f;
	m_iMaxCount = 65;
	m_fTermLimitTime = 5.5f;
}

void CBoss::PassiveEffect()
{
	list<CObj*>& BulletList = GET_SINGLE(CSceneManager)->GetSecondEffectList();

	CBulletA* pBullet = FIND(CBulletA)->FindObj("BossPassive")->Clone();

	pBullet->SetAngle(-PI / 2.f);

	pBullet->SetPos(GetLeft() - 40.f, GetBottom());

	pBullet->SetBulletType(BT_BOSS_SPECIAL1_1); 

	BulletList.push_back(pBullet);


	CBulletA* pEffect = FIND(CBulletA)->FindObj("BossPassive")->Clone();

	pEffect->SetAngle(-PI / 2.f);

	pEffect->SetPos(GetRight() + 40.f, GetBottom());

	pEffect->SetBulletType(BT_BOSS_SPECIAL1_1);

	BulletList.push_back(pEffect);


}

bool CBoss::Init()
{

	SetSpeed(350.f);
	SetAdjustDist(50.f);
	SetShape(ST_RECT);

	// �нú� : ������ ����� 40�ʸ��� ü�� 140 ȸ��
	m_fPassiveLimitTime = 40.f;

	m_iHP = 300;
	m_iHPMax = 300;

	// Stop
	m_fStopLimitTime = 1.f;
	

	// �̵� ����
	m_fXYLimitTime = 7.f;
	
	// TermTime
	m_fTermLimitTime = 0.f;


	return true;
}

void CBoss::Input(float fDeltaTime)
{

}

void CBoss::Collision(float fDeltaTime)
{

	CMonster::Collision(fDeltaTime);

}

void CBoss::Update(float fDeltaTime)
{

	if (m_bDie)
	{
		CMonster::Update(fDeltaTime);
		return;
	}

	// �нú� Ÿ���� ������ ����.

	m_fPassiveTime += fDeltaTime;

	if (m_fPassiveTime >= m_fPassiveLimitTime)
	{
		m_fPassiveTime -= m_fPassiveLimitTime;

		AddHP(140);
		PassiveEffect();

		m_bStop = true;

		// �����ð� 1�� 

	}




	// ���Ͱ� ���� ������ ��� 
	if (GetAttacked())
	{

		switch (m_eDir)
		{
		case SD_RIGHT:
			m_pAnimation->ChangeClip("BossRightAttacked");

			break;
		case SD_LEFT:
			m_pAnimation->ChangeClip("BossLeftAttacked");

			break;
		}
	}

	// ���� ���¶�� �ϴ���, ������ Ŭ���� �ٲ�� �Ѵ�.

	if (m_bStop)
	{
		m_fStopTime += fDeltaTime;

		if (m_fStopTime >= m_fStopLimitTime)
		{
			m_fStopTime = 0.f;
			m_bStop = false;
		}
	}
	else
	{

		m_fXYTime += fDeltaTime;

		if (m_fXYTime >= m_fXYLimitTime)
		{
			m_fXYTime -= m_fXYLimitTime;


			m_bXY = !m_bXY;

			// true�� false 
			// false�� true 

			int idx = rand() % 2 + 1;


			switch (idx)
			{
					case 1:
						m_eDir = SD_LEFT;
				
						switch (m_bXY)
						{
						case true: // x�� ����

							m_pAnimation->ChangeClip("BossLeft");
							m_pAnimation->SetDefaultClip("BossLeft");

							break;

						case false: // y�� Up

							m_pAnimation->ChangeClip("BossUp");
							m_pAnimation->SetDefaultClip("BossUp");

							break;
						}
				
				
						break;
					case 2: 
						m_eDir = SD_RIGHT; 
				
						switch (m_bXY)
						{
						case true: // x�� right

							m_pAnimation->ChangeClip("BossRight");
							m_pAnimation->SetDefaultClip("BossRight");

							break;

						case false: // y�� down

							m_pAnimation->ChangeClip("BossDown");
							m_pAnimation->SetDefaultClip("BossDown");

							break;
						}
				
				
						break;
					}




		}


			if (m_bXY) // X��
			{
				MoveX(fDeltaTime, m_eDir);

				if (GetLeft() < 0)
				{
					m_tPos.x = 0.f;
					m_eDir = SD_RIGHT;

					// �����ʸ������ ����
					m_pAnimation->ChangeClip("BossRight");
					m_pAnimation->SetDefaultClip("BossRight");
				}


				if (GetRight() > FULL_WIDTH)
				{
					m_tPos.x = FULL_WIDTH - GetSize().x;
					m_eDir = SD_LEFT;


					m_pAnimation->ChangeClip("BossLeft");
					m_pAnimation->SetDefaultClip("BossLeft");
				}
			}


			else // Y ��
			{

				MoveY(fDeltaTime, m_eDir);

				if (GetTop() < 0.f)
				{
					m_tPos.y = 0.f;
					m_eDir = SD_RIGHT;

					m_pAnimation->ChangeClip("BossDown");
					m_pAnimation->SetDefaultClip("BossDown");

				}


				if (GetBottom() > FULL_HEIGHT)
				{
					m_tPos.y = FULL_HEIGHT - GetSize().y;
					m_eDir = SD_LEFT;


					m_pAnimation->ChangeClip("BossUp");
					m_pAnimation->SetDefaultClip("BossUp");

				}


			}
		// �����ð����� �ð��� ���� �ʴ´�.


			m_fBulletTime += fDeltaTime;

			if (!m_bAttackMotion)// ������ ���� ���� ��쿡��
			{

				if (m_bTerm)
				{
					m_fTermTime += fDeltaTime;

					if (m_fTermTime >= m_fTermLimitTime)
					{
						m_fTermTime -= m_fTermLimitTime;

						m_bTerm = false;
						m_fBulletTime = 0.f;
					}

				}

				else if (m_fBulletTime >= m_fBulletLimitTime)
				{
					m_fBulletTime -= m_fBulletLimitTime;


					switch (m_eDir)
					{
					case SD_RIGHT:
						m_pAnimation->ChangeClip("BossRightAttack");

						break;
					case SD_LEFT:
						m_pAnimation->ChangeClip("BossLeftAttack");

						break;
					}

					

					switch (m_eAttack)
					{
					case BA_NORMAL:
						NormalAttackSet();
						NormalAttack();
						break;
					case BA_SPECIAL1:
						SpecialAttack1Set();
						SpecialAttack1();
						break;
					case BA_SPECIAL2:
						SpecialAttack2Set();
						SpecialAttack2();
						break;
					}

					m_iCount = 1;

					m_bStop = true;

					m_bAttackMotion = true;


				}
			}

			else // ���� ��� �� ����
			{

					if (m_iCount == m_iMaxCount)
					{
						m_bAttackMotion = false; // attack ����

						m_bTerm = true;
						m_fTermTime = 0.f;

												 // ���� ���°� ����� �����̸� �⺻�������� 
												 // �⺻�����̸� ����� ���� �� 1��.
						switch (m_eAttack)
						{
						case BA_NORMAL:
						{
							int idx = rand() % 2;
							m_eAttack = idx ? BA_SPECIAL1 : BA_SPECIAL2;
						}

						break;

						case BA_SPECIAL1:
						case BA_SPECIAL2:

							m_eAttack = BA_NORMAL;


							break;
						}

						// ��� ���� ���� ������ �׶�����, �ð��缭 �ٽ� ���� on
						// ��� -> ���ݽ��� -> ���ݳ� ->
					}


				if (m_fBulletTime >= m_fBulletLimitTime && m_iCount < m_iMaxCount && m_bAttackMotion)
				{
					m_fBulletTime -= m_fBulletLimitTime;


					switch (m_eAttack)
					{
					case BA_NORMAL:
						NormalAttack();
						break;
					case BA_SPECIAL1:
						SpecialAttack1();
						break;
					case BA_SPECIAL2:
						SpecialAttack2();
						break;
					}

					++m_iCount;

					
				}
			

			}

	}


	CMonster::Update(fDeltaTime);

}

void CBoss::Render(HDC hDC, float fDeltaTime)
{
	// Die == True�� �� Render�� �ִϸ��̼��� ����ؾ� �Ѵ�.

	CMonster::Render(hDC, fDeltaTime);

}

CBoss * CBoss::Clone()
{
	return new CBoss(*this);
}
