#include "CBulletA.h"
#include "../Scene/CInGameScene.h"
#include "CPlayer.h"

CBulletA::CBulletA() :
	m_fTime(0.f),
	m_fLimitTime(1.f)
{
}

CBulletA::CBulletA(CBulletA & bullet) :
	CBullet(bullet)
{
	m_fTime = 0.f;
	m_fLimitTime = bullet.m_fLimitTime;
}


CBulletA::~CBulletA()
{
}

void CBulletA::SetEffect()
{

	SetSize(0.f, 0.f);

	SetSpeed(0.f);

	SetLimitDist(100.f);

	SetDamage(0);

	SetShape(ST_CIRCLE);

	SetBulletType(BT_EFFECT);
}

bool CBulletA::Init()
{
	return true;
}

void CBulletA::Input(float fDeltaTime)
{
}

void CBulletA::Collision(float fDeltaTime)
{
	// Bullet Collision

	CBullet::Collision(fDeltaTime);

}

void CBulletA::Update(float fDeltaTime)
{
	if (!GetBulletEnable())
		return;


	switch(GetBulletType())
	{
			case BT_EFFECT: // 기본적인 effect 움직이지 않는다. 
				break;


			case BT_BOSS_SPECIAL2_1:
				break;

			case BT_EFFECT_PLAYER:

				// 플레이어의 Center를 기준으로, Render사이즈를 기준으로 잡는다.

				SetPos(PLAYER->GetCenter());

				break;


			default:

				MoveAngle(fDeltaTime);

				m_fDist += m_fSpeed * fDeltaTime;
	
				if (m_fDist >= m_fLimitDist)
					Die();

				break;

	}

	CBullet::Update(fDeltaTime);


}

void CBulletA::Render(HDC hDC, float fDeltaTime)
{
	CBullet::Render(hDC, fDeltaTime);

}

CBulletA * CBulletA::Clone()
{
	return new CBulletA(*this);
}
