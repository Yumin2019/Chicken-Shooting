#include "CSkillBullet.h"
#include "CPlayer.h"
#include "../Core/CSceneManager.h" 
#include "../Core/CCreateManager.h"
#include "../Math.h"
#include "../Object/CItem.h"
#include "../Scene/CInGameScene.h"


CSkillBullet::CSkillBullet() :
	m_fLifeTime(0.f),
	m_fAngleTime(0.f),
	m_fAngleLimitTime(0.01f),
	m_iCount(0)				
{
}

CSkillBullet::CSkillBullet(CSkillBullet & bullet) :
	CBullet(bullet)
{ 
	m_fLifeLimitTime = bullet.m_fLifeLimitTime;

	m_fAngleLimitTime = bullet.m_fAngleLimitTime;

	m_fAngleTime = 0.f;

	m_fLifeTime = 0.f;

	m_iCount = 0;
}


CSkillBullet::~CSkillBullet()
{
}

bool CSkillBullet::Init()
{
	return true;
}

void CSkillBullet::Input(float fDeltaTime)
{
}

void CSkillBullet::Collision(float fDeltaTime)
{
	// Bullet Collision

	CBullet::Collision(fDeltaTime);
}

void CSkillBullet::Update(float fDeltaTime)
{


		if (GetBulletType() == BT_ITEM)
		{
			m_fBlendTime += fDeltaTime;

			if (m_fBlendTime >= m_fBlendLimitTime) 
			{
				m_fBlendTime -= m_fBlendLimitTime;

				++m_iCount;

				switch (m_bBlend)
				{
				case true:
					m_bBlend = false;
					break;
				case false:
					m_bBlend = true;
					break;
				}


				if (m_iCount <= 4)
					m_fBlendLimitTime = 0.5f;
				else if (m_iCount <= 9)
					m_fBlendLimitTime = 0.2f; // 3초 남음
				else
					m_fBlendLimitTime = 0.17f;

				// 점점 빨라지는 ... 효과 !
			}

	}
	


	m_fLifeTime += fDeltaTime;

	if (m_fLifeTime >= m_fLifeLimitTime)
	{

		switch(GetBulletType())
		{
		case BT_PLAYER_MISAIL:
			
			MisailCreate();

			break;


		case BT_PLAYER_ATTACKPREV:

			BulletChange();

			break;
			
		case BT_PLAYER_SHIELD: 


			break;


		case BT_PLAYER_BOMB:
		{
			CBullet::Collision(fDeltaTime);
			CBullet::Collision(fDeltaTime);

		}
			break;

		case BT_MONSTER: // MinionB

			break;

		case BT_BOSS_NORMAL:

			CreateFlower();

			break;

		case BT_ITEM:
			
			switch (((CItem*)this)->GetItemType())
			{
				case IT_BLUEMARBLE:
					CreateMinionA();

					break;

				case IT_PINKMARBLE:
					CreateMinionB();

					break;
					// Marble아이템은 살아있었다면, 미니언을 생성
			}



			break;

		
		}


		Die();

		return;
	}
	


	switch (GetBulletType())
	{
	case BT_PLAYER_SHIELD:

		// 쉴드는 플레이어의 좌표를 따라간다.

		SetPos(GetShieldPos(PLAYER->GetCenter(), GetSize()));

		break;

	case BT_PLAYER_MISAIL: // 미사일 불릿의 1차. EggBread

		m_fAngleTime += fDeltaTime;


		if (m_fAngleTime >= m_fAngleLimitTime) // 0.01s에 1.2 도  
		{
			m_fAngleTime -= m_fAngleLimitTime;

			AddAngle(PI / 180.f * 1.2f);

			SetPos(GetBulletPos(PLAYER->GetCenter(), 85.f, m_fAngle, this));

			// 중심을 기준으로 이 자리에서 각도만 조금씩 바꿔서 처리한다.

		}


		break;





	case BT_PLAYER_ATTACKPREV: // ATTACKprev Egg
	{
		m_fAngleTime += fDeltaTime;


		if (m_fAngleTime >= m_fAngleLimitTime) // 0.01s에 1.2도 3초에 1바퀴
		{
			m_fAngleTime -= m_fAngleLimitTime;


			AddAngle(PI / 180.f * 1.2f);

			SetPos(GetBulletPos(PLAYER->GetCenter(), 40.f, m_fAngle, this));

		}


	}
	break;



	case BT_PLAYER_NORMAL: // 미사일 불릿의 2차. fried egg
			{
				// 가장 가까운 적을 기준으로 따라간다.
				CObj* pMonster = FindAdjustMonster();

				if (pMonster)
				{
					// 몬스터를 기준으로 각도를 얻는다.
					float fAngle = Math::GetAngle(this, pMonster); 

					SetAngle(fAngle);
				}
		
				// 아무도 없으면 처음에 주어진 각도에 따라 움직인다.
			}

			MoveAngle(fDeltaTime);

				break;

		case BT_MONSTER: // MinionB
		{
				float fAngle = Math::GetAngle(this, PLAYER);

				m_fAngleTime += fDeltaTime;

				SetAngle(fAngle);


					if (m_fAngleTime >= m_fAngleLimitTime)
					{
						m_fAngleTime -= m_fAngleLimitTime;

						CircleCreate();
	
					}


			MoveAngle(fDeltaTime);


		}

		break;

		case BT_BOSS_SPECIAL1PREV: // 플레이어를 계속 따라다닌다.
		case BT_BOSS_NORMAL:  
		{

			float fAngle = Math::GetAngle(this, PLAYER);

			SetAngle(fAngle);

			MoveAngle(fDeltaTime);

			// 플레이어의 각도를 잡고 계속 따라간다. 시간 지나면 죽는다.
			
		}
			break;

		case BT_BOSS_SPECIAL1_1:

			m_fAngleTime += fDeltaTime;


			if (m_iCount < 3 && m_fAngleTime >= m_fAngleLimitTime) 
			{
				m_fAngleTime -= m_fAngleLimitTime;

				int idx = rand() % 2 + 1;

				CSkillBullet* pMarble;

				switch (idx)
				{
					case 1: pMarble = (CSkillBullet*)CreateItem("BlueMarble", GetPos()); break;
					case 2: pMarble = (CSkillBullet*)CreateItem("PinkMarble", GetPos()); break;
				}


				GET_SINGLE(CSceneManager)->GetFirstEffectList().push_back(pMarble);

				++m_iCount;
			}



			MoveAngle(fDeltaTime);


			break;

	

			// ONLY MOVE ANGLE 
		case BT_PLAYER_BOMB:
		case BT_BOSS_SPECIAL2_2:

			MoveAngle(fDeltaTime);

			break;

	}

	CBullet::Update(fDeltaTime);

}

void CSkillBullet::Render(HDC hDC, float fDeltaTime)
{
	CBullet::Render(hDC, fDeltaTime);
	
}

CSkillBullet * CSkillBullet::Clone()
{
	return new CSkillBullet(*this);
}

CObj* CSkillBullet::FindAdjustMonster()
{
	list<class CObj*> MonsterList = GET_SINGLE(CSceneManager)->GetMonsterList();

	list<class CObj*>::iterator iter;
	list<class CObj*>::iterator iterEnd = MonsterList.end();

	CObj* pMonster = NULL;

	float fDist = FULL_WIDTH * 2;

	for (iter = MonsterList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter) == PLAYER)
			continue;

		if (!(*iter)->GetLife())
			continue;

		// 미사일의 센터와 몬스터의 중심사이의 거리를 구해서 가장 짧은 것을 잡는다.

		float m_fDist = Math::PointToPoint((*iter)->GetCenter(), GetCenter());

		if (fDist > m_fDist)
		{
			pMonster = (*iter);
			fDist = m_fDist;
		}
	}
	return pMonster; // 가장 거리가 짧은 것을 Target으로 잡는다.
}

void CSkillBullet::MisailCreate()
{

	float fAngle = 0.f;


	list<CBullet*>& PlayerMisailList = PLAYER->GetMisailBulletList();

	for (int i = 0; i < 4; ++i)
	{
		CSkillBullet* pBullet = FIND(CSkillBullet)->FindObj("Misail")->Clone();

		pBullet->SetAngle(fAngle);


		pBullet->SetPos(GetBulletPos(GetCenter(), 25.f, fAngle, pBullet));

		pBullet->SetBulletType(BT_PLAYER_NORMAL);

		PlayerMisailList.push_back(pBullet);

		fAngle += PI / 2.f;


	}
}

void CSkillBullet::BulletChange()
{

	list<CBullet*>& PlayerBulletList = PLAYER->GetMisailBulletList(); // 데입할 List


	CSkillBullet* pBullet = FIND(CSkillBullet)->FindObj("Attack")->Clone();

	pBullet->SetAngle(GetAngle()); // 기존 Bullet의 Angle

	pBullet->SetPos(GetPos()); // pos는 그대로.

	pBullet->SetBulletType(BT_PLAYER_NORMAL); 

	PlayerBulletList.push_back(pBullet); // 중간에 size가 변하는 것을 생각하지 않도록. 다른 List

}

void CSkillBullet::CircleCreate()
{
	// 사방에 뿌릴 총알을 만든다.
	
	// 8개45도.
	
	float fAngle = 0.f;

	for (size_t i = 0; i < 8; ++i)
	{
		CBullet* pBullet = FIND(CBullet)->FindObj("MinionBBullet")->Clone();

		pBullet->SetAngle(fAngle); 

		pBullet->SetPos(GetPos()); // 자신의 위치에서 생성

		pBullet->SetBulletType(BT_MONSTER);

		GET_SINGLE(CSceneManager)->GetDeadMonsterBulletList().push_back(pBullet);

		fAngle += PI / 4.f;
	}
}

void CSkillBullet::BossSpecial1_2()
{

	float fAngle = PI / 4.f; // 45도부터 시작하여 90도 4번.

	for (size_t i = 0; i < 4; ++i)
	{
		CBullet* pBullet = FIND(CBullet)->FindObj("BossBulletName")->Clone();

		pBullet->SetAngle(fAngle);

		pBullet->SetPos(GetPos()); // 자신의 위치에서 생성

		pBullet->SetBulletType(BT_MONSTER);

		GET_SINGLE(CSceneManager)->GetDeadMonsterBulletList().push_back(pBullet);


		fAngle += PI / 4.f;
	}
}

void CSkillBullet::CreateFlower()
{

	float fAngle = 0.f;

	for (size_t i = 0; i < 8; ++i)
	{
		CBullet* pBullet = FIND(CBullet)->FindObj("BossNormalBullet2")->Clone();

		pBullet->SetAngle(fAngle);

		pBullet->SetPos(GetPos()); 

		pBullet->SetBulletType(BT_MONSTER);

		GET_SINGLE(CSceneManager)->GetDeadMonsterBulletList().push_back(pBullet);


		fAngle += PI / 4.f;
	}
}


