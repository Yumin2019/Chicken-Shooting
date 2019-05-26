#include "CPlayer.h"
#include "CSkillBullet.h"
#include "CBulletA.h"
#include "../Core/CCreateManager.h"
#include "../Core/CInput.h"
#include "../Animation/CAnimation.h"
#include "../Core/CSceneManager.h"
#include "../Scene/CInGameScene.h"


CPlayer::CPlayer()
{
}


CPlayer::~CPlayer()
{
	Safe_Release_VecList(m_MisailBullet);
	
}

bool CPlayer::Init()
{

	SetPos(500.f, 500.f);
	

	SetLeftTop(-15.f, -25.f);
	SetRenderSize(64.f, 64.f);

	SetSize(35.f, 29.f);


	SetSpeed(400.f);
	SetAdjustDist(80.f);
	SetShape(ST_RECT);
	SetHP(100);
	m_eDir = SD_NONE;
	m_bShield = false;
	m_bBomb = false;


	m_bMisail = false;

	m_fStopTime = 0.f;

	m_iSpeedLv = 0;
	m_iAttackLv = 0;

	m_iShield = 4;
	m_iBomb = 6; 

	m_fMisailTime = 0.f;
	m_fMisailLimitTime = 6.f; // 0.5씩 감소.

	m_fShieldTime = 0.f;
	m_fBombTime = 0.f;

	m_fShieldLimitTime = 6.f;
	m_fBombLimitTime = 1.6f;

	m_iBallCount = 0;
	m_iBallLimitCount = 3; // + 1

	m_fBallCreateTime = 0.f;
	m_fBallCreateLimitTime = 0.6f; // 0.1


	m_pAnimation = CreateAnimation("Player");

	m_pAnimation->AddClip(L"Player/PlayerLeft.bmp", "PlayerLeft", AO_LOOP,
		0.3f, 0, 0, 3, 1, 0.f, 0.f, 0.f, 64.f, 64.f, 255, 0, 255);


	m_pAnimation->AddClip(L"Player/PlayerRight.bmp", "PlayerRight", AO_LOOP,
		0.3f, 0, 0, 3, 1, 0.f, 0.f, 0.f, 64.f, 64.f, 255, 0, 255);


	m_pAnimation->AddClip(L"Player/PlayerUp.bmp", "PlayerUp", AO_LOOP,
		0.3f, 0, 0, 3, 1, 0.f, 0.f, 0.f, 64.f, 64.f, 255, 0, 255);


	m_pAnimation->AddClip(L"Player/PlayerDown.bmp", "PlayerDown", AO_LOOP,
		0.3f, 0, 0, 3, 1, 0.f, 0.f, 0.f, 64.f, 64.f, 255, 0, 255);



	m_pAnimation->AddClip(L"Player/PlayerIdle.bmp", "PlayerIdle", AO_LOOP,
		3.f, 0, 0, 6, 1, 0.f, 0.f, 0.f, 64.f, 64.f, 255, 0, 255);


	m_pAnimation->SetDefaultClip("PlayerIdle");

	// 시작할 때는 Default 클립으로

	m_pAnimation->SetObj(this);

	m_pAnimation->ChangeClip(m_pAnimation->GetDefaultClip());



	return true;
}

void CPlayer::Input(float fDeltaTime)
{


				if (KEYPRESS("MoveLeft"))
				{
					
						MoveX(fDeltaTime, SD_LEFT);


					m_eDir = SD_LEFT;

					if (GetLeft() < 0.f)
						m_tPos.x = 0.f;

					m_pAnimation->ChangeClip("PlayerLeft");
					m_fStopTime = 0.f;
					
				}

				else if (KEYPRESS("MoveRight"))
				{

					
						MoveX(fDeltaTime, SD_RIGHT);


					m_eDir = SD_RIGHT;

					if (GetRight() > FULL_WIDTH)
						m_tPos.x = FULL_WIDTH - GetSize().x;

					m_pAnimation->ChangeClip("PlayerRight");
					m_fStopTime = 0.f;


				}

				else if (KEYPRESS("MoveBack"))
				{
					
						MoveY(fDeltaTime, SD_LEFT);


					if (GetTop() < 0)
						m_tPos.y = 0.f;

					m_pAnimation->ChangeClip("PlayerUp");
					m_fStopTime = 0.f;


				}

				else if (KEYPRESS("MoveFront"))
				{
					
						MoveY(fDeltaTime, SD_RIGHT);


					if (GetBottom() > FULL_HEIGHT)
						m_tPos.y = FULL_HEIGHT - GetSize().y;

					m_pAnimation->ChangeClip("PlayerDown");
					m_fStopTime = 0.f;

				}


				

				if (KEYPRESS("Fire"))  // A
				{
					m_fBallCreateTime += fDeltaTime;

					if (m_fBallCreateTime >= m_fBallCreateLimitTime && m_iBallCount < m_iBallLimitCount)
					{
						m_fBallCreateTime -= m_fBallCreateTime;

						BallCreate();
					}
				}


				if (KEYUP("Fire"))  // A
				{
					// 강제로 출발시킴.
					SetAttackEnd();
					Fire();
				}


				if (KEYDOWN("Shield") && !m_bShield && m_iShield)  // S
				{
					m_bShield = true;
					Shield();

				}

				if (KEYDOWN("Bomb") && !m_bBomb && m_iBomb) // R
				{
					m_bBomb = true;
					Bomb();

				}


				if (KEYDOWN("Misail") && !m_bMisail)  // D 
				{
					m_bMisail = true;
					Misail();
				}
				
				//  Cheat Key

				if (KEYDOWN("Cheat"))
				{
					m_iBomb = 100;
					m_iShield = 100;
					m_iHP = 99999;
					m_iHPMax = 99999;
					m_fSpeed = 600.f;
				
					m_fBallCreateLimitTime -= 0.1f;
					++m_iBallLimitCount;
				}

			
				
			
			
}

void CPlayer::Collision(float fDeltaTime)
{
	CMoveObj::Collision(fDeltaTime);

	// Player's Bullet Collision

	list<CBullet*>::iterator iter;
	list<CBullet*>::iterator iterEnd = m_BulletList.end();

	for (iter = m_BulletList.begin(); iter != iterEnd;)
	{
		if (!(*iter)->GetLife())
		{
			SAFE_RELEASE((*iter));
			iter = m_BulletList.erase(iter);
			iterEnd = m_BulletList.end();
			continue;
		}

		(*iter)->Collision(fDeltaTime);
		++iter;

	}


	// Player's Misail Collision

	list<CBullet*>::iterator m_iter;
	list<CBullet*>::iterator m_iterEnd = m_MisailBullet.end();


	for (m_iter = m_MisailBullet.begin(); m_iter != m_iterEnd;)
	{
		if (!(*m_iter)->GetLife())
		{
			SAFE_RELEASE((*m_iter));
			m_iter = m_MisailBullet.erase(m_iter);
			m_iterEnd = m_MisailBullet.end();
			continue;
		}

		(*m_iter)->Collision(fDeltaTime);


		++m_iter;

	}

}

void CPlayer::Update(float fDeltaTime)
{
			if (m_bMisail)
			{
				m_fMisailTime += fDeltaTime;

				if (m_fMisailTime >= m_fMisailLimitTime)
				{
					m_bMisail = false;
					m_fMisailTime = 0.f;
				}
			}

			if (m_bShield)
			{
				m_fShieldTime += fDeltaTime;

				if (m_fShieldTime >= m_fShieldLimitTime)
				{
					m_bShield = false;
					m_fShieldTime = 0.f;
				}
			}

			if (m_bBomb)
			{
				m_fBombTime += fDeltaTime;

				if (m_fBombTime >= m_fBombLimitTime)
				{
					m_bBomb = false;
					m_fBombTime = 0.f;
				}
			}


		m_fStopTime += fDeltaTime;

		if (m_fStopTime >= 0.3f)
		{
			m_pAnimation->ChangeClip(m_pAnimation->GetDefaultClip());
		}

	

	
	CMoveObj::Update(fDeltaTime);

	// Player's Bullet Move

	list<CBullet*>::iterator iter;
	list<CBullet*>::iterator iterEnd = m_BulletList.end();

	for (iter = m_BulletList.begin(); iter != iterEnd;)
	{
		if (!(*iter)->GetLife())
		{
			SAFE_RELEASE((*iter));
			iter = m_BulletList.erase(iter);
			iterEnd = m_BulletList.end();
			continue;
		}

		(*iter)->Update(fDeltaTime);
		++iter;

	}

	// Player's Misail Move

	list<CBullet*>::iterator m_iter;
	list<CBullet*>::iterator m_iterEnd = m_MisailBullet.end();


	for (m_iter = m_MisailBullet.begin(); m_iter != m_iterEnd;)
	{
		if (!(*m_iter)->GetLife())
		{
			SAFE_RELEASE((*m_iter));
			m_iter = m_MisailBullet.erase(m_iter);
			m_iterEnd = m_MisailBullet.end();
			continue;
		}

		(*m_iter)->Update(fDeltaTime);
		++m_iter;

	}
}

void CPlayer::Render(HDC hDC, float fDeltaTime)
{
	

	// Player's Bullet Render

	list<CBullet*>::iterator iter;
	list<CBullet*>::iterator iterEnd = m_BulletList.end();

	for (iter = m_BulletList.begin(); iter != iterEnd;)
	{

		if (!(*iter)->GetLife())
		{
			SAFE_RELEASE((*iter));
			iter = m_BulletList.erase(iter);
			iterEnd = m_BulletList.end();
			continue;
		}

		(*iter)->Render(hDC, fDeltaTime);
		++iter;

	}



	// Player's Misail Render

	list<CBullet*>::iterator m_iter;
	list<CBullet*>::iterator m_iterEnd = m_MisailBullet.end();


	for (m_iter = m_MisailBullet.begin(); m_iter != m_iterEnd;)
	{
		if (!(*m_iter)->GetLife())
		{
			SAFE_RELEASE((*m_iter));
			m_iter = m_MisailBullet.erase(m_iter);
			m_iterEnd = m_MisailBullet.end();
			continue;
		}

		(*m_iter)->Render(hDC, fDeltaTime);


		++m_iter;

	}


	// Player Render

	CMoveObj::Render(hDC, fDeltaTime);


}



void CPlayer::BallCreate()
{

	float fAngle = 60.f * m_fBallCreateLimitTime + 120.f;  

	fAngle *= m_iBallCount;

	// 각도를 라디안으로 표현한다.

	fAngle *= PI / 180.f;

	CSkillBullet* pBullet = FIND(CSkillBullet)->FindObj("AttackPrev")->Clone();

	pBullet->SetAngle(fAngle);

	pBullet->SetPos(GetBulletPos(GetCenter(), 40.f, fAngle, pBullet));

	pBullet->SetBulletType(BT_PLAYER_ATTACKPREV);

	m_BulletList.push_back(pBullet);

	++m_iBallCount;

}

void CPlayer::PlayerAttacked()
{
	// 피격 이펙트를 추가한다.

	CBulletA* pBullet = (CBulletA*)CreateEffect("PlayerAttacked", GetCenter());

	GET_SINGLE(CSceneManager)->GetSecondEffectList().push_back(pBullet);
}


list<class CBullet*>& CPlayer::GetMisailBulletList()
{
	return m_MisailBullet;
}


void CPlayer::Shield()
{
	// Shield Bullet 생성. 

	CSkillBullet* pBullet = FIND(CSkillBullet)->FindObj("Shield")->Clone(); 

	pBullet->SetAngle(0.f);

	pBullet->SetPos(GetShieldPos(GetCenter(), pBullet->GetSize()));

	pBullet->SetBulletType(BT_PLAYER_SHIELD); 

	list<CObj*>& m_Bullet = GET_SINGLE(CSceneManager)->GetFirstEffectList();   // 바닥에

	m_Bullet.push_back(pBullet);

	CBulletA* pEffect = (CBulletA*)CreateEffect("ShieldEffect", GetCenter());

	m_Bullet.push_back(pEffect); // 장판 위에 Effect

	--m_iShield;
}

void CPlayer::Misail()
{

		CSkillBullet* pBullet = FIND(CSkillBullet)->FindObj("MisailPrev")->Clone(); 

		pBullet->SetAnimation();

		float fAngle = GetDirAngle();

		pBullet->SetAngle(fAngle);


		pBullet->SetPos(GetBulletPos(GetCenter(), 80.f, fAngle, pBullet));

		pBullet->SetBulletType(BT_PLAYER_MISAIL); 

		m_BulletList.push_back(pBullet);
	
}

void CPlayer::Fire()
{
	// 현재 불릿리스트에서 attackperv를 찾는다.

	list<CBullet*>::iterator iter;
	list<CBullet*>::iterator iterEnd = m_BulletList.end();

	for (iter = m_BulletList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->GetBulletType() == BT_PLAYER_ATTACKPREV)
		{
			((CSkillBullet*)(*iter))->BulletChange();
			((CSkillBullet*)(*iter))->Die();
		}
	}
}

void CPlayer::Bomb()
{
	float fAngle = GetDirAngle();

	CSkillBullet* pBullet = FIND(CSkillBullet)->FindObj("Bomb")->Clone();

	pBullet->SetAngle(fAngle);

	pBullet->SetAnimation();

	switch (m_eDir) // 바라보는 방향대로.
	{
	case SD_RIGHT:
		pBullet->SetPos(GetBulletPos(POSITION(GetCenterX(), 0.f), 3.f, fAngle, 0.f, 0.f));

		break;
	case SD_LEFT:
		pBullet->SetPos(GetBulletPos(POSITION(GetCenterX(), 0.f), 3.f, fAngle, -45.f, 0.f));

		break;
	}
	pBullet->SetBulletType(BT_PLAYER_BOMB);


	GET_SINGLE(CSceneManager)->GetSecondEffectList().push_back(pBullet);

	--m_iBomb;

}

