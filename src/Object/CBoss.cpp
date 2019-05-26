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


	// 셋팅

	switch (m_eDir)
	{
	case SD_LEFT:

		switch (m_bXY)
		{
		case true: // x축 왼쪽

			m_pAnimation->SetDefaultClip("BossLeft");

			break;

		case false: // y축 Up

			m_pAnimation->SetDefaultClip("BossUp");

			break;
		}


		break;

	case SD_RIGHT:

		switch (m_bXY)
		{
		case true: // x축 right

			m_pAnimation->SetDefaultClip("BossRight");

			break;

		case false: // y축 down

			m_pAnimation->SetDefaultClip("BossDown");

			break;
		}
	}

	// 패시브
	m_fPassiveTime = 0.f;
	m_fPassiveLimitTime = mon.m_fPassiveLimitTime;


	// 정지 시간
	m_fStopTime = 0.f;
	m_fStopLimitTime = mon.m_fStopLimitTime;

	m_bStop = false;


	// 이동에 대하여
	m_fXYTime = 0.f;
	m_fXYLimitTime = mon.m_fXYLimitTime;


	// 처음 공격 타입은 NORMAL
	NormalAttackSet();
	m_eAttack = BA_NORMAL;

	// 텀 시간.
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

	// 기본 공격 구현
	float fAngle = Math::GetAngle(this, (CObj*)PLAYER); 

	CSkillBullet* pBullet = FIND(CSkillBullet)->FindObj("BossNormalBullet")->Clone();

	pBullet->SetAngle(fAngle); // 플레이어를 향해

							   // 시작 위치 구하기
	POSITION m_tPos = GetBulletPos(GetCenter(), GetAdjustDist(), fAngle, pBullet);


	pBullet->SetPos(m_tPos); // Pos 

	pBullet->SetBulletType(BT_BOSS_NORMAL);
	
	m_BulletList.push_back(pBullet);


}

void CBoss::SpecialAttack1()
{

	// 3번을 쏘는데 중복은 있으면 안 된다.

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

		// define을 이용해서 광역기 및 공격을 한다.
	}


}

void CBoss::SpecialAttack2()
{


	// 0 ~ 11에서 중복있어도 상관없이 10번 ( 시간차 공격이라 한번씩 ) 할 예정
	// 여기서는 0 ~ 10을 하나 골라서, * 100을 하여 y축 좌표로 이용한다. FULL_HEIGHT
	
	// x 좌표는 중심에서 -600 + 600으로 고정.
	// 900 으로 잡고, 사진이 겹치는 일은 없도록 . 100단위로.

	// 7 ~ 11중에 하나 뽑아서 * 100.

	float x, y;

	y = rand() % 11; // 0 ~ 10

	x = rand() % 13 + 3; // 3 15

	POSITION tPos = POSITION(x * 100.f, y * 100.f);

	CBulletA* pEffect = FIND(CBulletA)->FindObj("Death")->Clone();

	pEffect->SetAnimation("DeathIdle"); // Idle로 셋팅 -> 2번 돌고(2초) -> time_return-> Die로 셋팅 -> 1번(1초) 사라짐.

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

	// 패시브 : 끈질긴 생명력 40초마다 체력 140 회복
	m_fPassiveLimitTime = 40.f;

	m_iHP = 300;
	m_iHPMax = 300;

	// Stop
	m_fStopLimitTime = 1.f;
	

	// 이동 변수
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

	// 패시브 타임은 언제나 센다.

	m_fPassiveTime += fDeltaTime;

	if (m_fPassiveTime >= m_fPassiveLimitTime)
	{
		m_fPassiveTime -= m_fPassiveLimitTime;

		AddHP(140);
		PassiveEffect();

		m_bStop = true;

		// 정지시간 1초 

	}




	// 몬스터가 맞은 상태인 경우 
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

	// 정지 상태라고 하더라도, 맞으면 클립을 바꿔야 한다.

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

			// true면 false 
			// false면 true 

			int idx = rand() % 2 + 1;


			switch (idx)
			{
					case 1:
						m_eDir = SD_LEFT;
				
						switch (m_bXY)
						{
						case true: // x축 왼쪽

							m_pAnimation->ChangeClip("BossLeft");
							m_pAnimation->SetDefaultClip("BossLeft");

							break;

						case false: // y축 Up

							m_pAnimation->ChangeClip("BossUp");
							m_pAnimation->SetDefaultClip("BossUp");

							break;
						}
				
				
						break;
					case 2: 
						m_eDir = SD_RIGHT; 
				
						switch (m_bXY)
						{
						case true: // x축 right

							m_pAnimation->ChangeClip("BossRight");
							m_pAnimation->SetDefaultClip("BossRight");

							break;

						case false: // y축 down

							m_pAnimation->ChangeClip("BossDown");
							m_pAnimation->SetDefaultClip("BossDown");

							break;
						}
				
				
						break;
					}




		}


			if (m_bXY) // X축
			{
				MoveX(fDeltaTime, m_eDir);

				if (GetLeft() < 0)
				{
					m_tPos.x = 0.f;
					m_eDir = SD_RIGHT;

					// 오른쪽모션으로 변경
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


			else // Y 축
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
		// 정지시간에는 시간을 재지 않는다.


			m_fBulletTime += fDeltaTime;

			if (!m_bAttackMotion)// 공격을 하지 않은 경우에만
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

			else // 공격 모션 그 이후
			{

					if (m_iCount == m_iMaxCount)
					{
						m_bAttackMotion = false; // attack 종료

						m_bTerm = true;
						m_fTermTime = 0.f;

												 // 현재 상태가 스페셜 공격이면 기본공격으로 
												 // 기본공격이면 스페셜 공격 중 1택.
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

						// 모든 공격 턴이 끝나면 그때부터, 시간재서 다시 공격 on
						// 대기 -> 공격시작 -> 공격끝 ->
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
	// Die == True일 때 Render로 애니메이션을 출력해야 한다.

	CMonster::Render(hDC, fDeltaTime);

}

CBoss * CBoss::Clone()
{
	return new CBoss(*this);
}
