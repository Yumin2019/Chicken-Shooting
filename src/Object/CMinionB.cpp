#include "CMinionB.h"
#include "CSkillBullet.h"
#include "../Animation/CAnimation.h"
#include "../Core/CCreateManager.h"
#include "../Scene/CInGameScene.h"
#include "CPlayer.h"
#include "../Math.h"


CMinionB::CMinionB() :
	m_tTime(0.f)
{
}

CMinionB::CMinionB(const CMinionB & mon) :
	CMonster(mon)
{
	m_tTime = 0.f;
}


CMinionB::~CMinionB()
{
}

bool CMinionB::Init()
{

	switch (m_bXY)
	{
	case true:

		switch (m_eDir)
		{
		case SD_RIGHT:
			m_pAnimation->ChangeClip("MinionBRight");
			m_pAnimation->SetDefaultClip("MinionBRight");
			break;

		case SD_LEFT:

			m_pAnimation->ChangeClip("MinionBLeft");
			m_pAnimation->SetDefaultClip("MinionBLeft");
			break;
		}
		break;

	case false:


	
			switch (PLAYER->GetCenterX() >= GetCenterX())
			{
			case true:
				m_pAnimation->ChangeClip("MinionBRight");
				m_pAnimation->SetDefaultClip("MinionBRight");
				break;

			case false:

				m_pAnimation->ChangeClip("MinionBLeft");
				m_pAnimation->SetDefaultClip("MinionBLeft");
				break;
			}
		
		break;
	}

	return true;
}

void CMinionB::Input(float fDeltaTime)
{
	
}

void CMinionB::Collision(float fDeltaTime)
{
	CMonster::Collision(fDeltaTime);
}

void CMinionB::Update(float fDeltaTime)
{

	// 몬스터가 맞은 상태인 경우 
	if (GetAttacked())
	{
		switch (m_bXY)
		{
		case true: // x

			switch (m_eDir)
			{
			case SD_RIGHT:
				m_pAnimation->ChangeClip("MinionBRightAttacked");

				break;
			case SD_LEFT:
				m_pAnimation->ChangeClip("MinionBLeftAttacked");

				break;
			}

			break;

		case false: // y

			
				switch (PLAYER->GetCenterX() >= GetCenterX())
				{
				case true:
					m_pAnimation->ChangeClip("MinionBRightAttacked");

					break;
				case false:
					m_pAnimation->ChangeClip("MinionBLeftAttacked");

					break;
				}

			m_tTime = 0.f;


			break;

		}
	}


	CMonster::Update(fDeltaTime);



	// Minion Move


	switch (m_bXY)
	{
	case true:
		MoveX(fDeltaTime, m_eDir);


		if (GetLeft() < 0)
		{
			m_tPos.x = 0.f;
			m_eDir = SD_RIGHT;

			// 오른쪽모션으로 변경
			m_pAnimation->ChangeClip("MinionBRight");
			m_pAnimation->SetDefaultClip("MinionBRight");
		}


		if (GetRight() > FULL_WIDTH)
		{
			m_tPos.x = FULL_WIDTH - GetSize().x;
			m_eDir = SD_LEFT;


			m_pAnimation->ChangeClip("MinionBLeft");
			m_pAnimation->SetDefaultClip("MinionBLeft");
		}

		break;

	case false:

		m_tTime += fDeltaTime;


		if (m_tTime >= 0.5f)
		{
			m_tTime -= 0.5f;

			switch (PLAYER->GetCenterX() >= GetCenterX())
			{
			case true:
				m_pAnimation->ChangeClip("MinionBRight");
				m_pAnimation->SetDefaultClip("MinionBRight");
				break;

			case false:

				m_pAnimation->ChangeClip("MinionBLeft");
				m_pAnimation->SetDefaultClip("MinionBLeft");
				break;
			}
		}

		MoveY(fDeltaTime, m_eDir);

		if (GetTop() < 0.f)
		{
			m_tPos.y = 0.f;
			m_eDir = SD_RIGHT;

		}


		if (GetBottom() > FULL_HEIGHT)
		{
			m_tPos.y = FULL_HEIGHT - GetSize().y;
			m_eDir = SD_LEFT;

		}


		break;
	}



	m_fBulletTime += fDeltaTime;

	if (m_fBulletTime >= m_fBulletLimitTime) 
	{
			m_fBulletTime -= m_fBulletLimitTime;

			float fAngle = Math::GetAngle(this, (CObj*)PLAYER); 

			CSkillBullet *pBullet = FIND(CSkillBullet)->FindObj("MinionBBulletPrev")->Clone(); 

			pBullet->SetAngle(fAngle); 

			POSITION m_tPos = GetBulletPos(GetCenter(), GetAdjustDist(), fAngle, pBullet);


			pBullet->SetPos(m_tPos);


			pBullet->SetBulletType(BT_MONSTER);

			m_BulletList.push_back(pBullet);
		}


}

void CMinionB::Render(HDC hDC, float fDeltaTime)
{
	CMonster::Render(hDC, fDeltaTime);
}
CMinionB * CMinionB::Clone()
{
	return new CMinionB(*this);
}
