#include "Object\CMoveObj.h"
#include "Object/CBullet.h"
#include "Scene\CInGameScene.h"
#include "Animation\CAnimation.h"
#include "Object\CBoss.h"

CMoveObj::CMoveObj() :
	m_eDir(SD_NONE),
	m_bAttacked(false)
{
}

CMoveObj::CMoveObj(const CMoveObj & move) :
	CObj(move)
{
	m_eDir = move.m_eDir;
	m_fAdjustDist = move.m_fAdjustDist;

	m_iHP = move.m_iHP;
	m_iHPMax = m_iHP;

}


CMoveObj::~CMoveObj()
{
	Safe_Release_VecList(m_BulletList);
}

bool CMoveObj::BulletSort(CBullet * bullet1, CBullet * bullet2)
{
	return bullet1->GetBulletType() < bullet2->GetBulletType();
}

void CMoveObj::AddHP(int iDamage)
{

	m_iHP += iDamage;

	// 보스의 상태에 따른 처리

	if (m_iHP > m_iHPMax)
		m_iHP = m_iHPMax;

	if (m_iHP <= 0)
	{
		Die();

		if ((CMoveObj*)BOSS == this)
		{
			// Layer에서 없애주는 것을 막는 역할

			BOSS->GetAnimation()->ChangeClip("BossDie");
			BOSS->SetBossDie(); // Move & Attack 막기.
			CInGameScene::m_bBoss = false;
			m_bLife = true;

		}
		else if ((CMoveObj*)PLAYER == this)
		{
			CInGameScene::m_bPlayer = false;
		}
	}
}

bool CMoveObj::Init()
{
	return false;
}

void CMoveObj::Input(float fDeltaTime)
{
}

void CMoveObj::Collision(float fDeltaTime)
{
	CObj::Collision(fDeltaTime);
}

void CMoveObj::Update(float fDeltaTime)
{
	CObj::Update(fDeltaTime);

}

void CMoveObj::Render(HDC hDC, float fDeltaTime)
{
	CObj::Render(hDC, fDeltaTime);

}

CMoveObj * CMoveObj::Clone()
{
	return new CMoveObj(*this);
}
