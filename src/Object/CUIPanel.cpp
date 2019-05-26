#include "CUIPanel.h"
#include "CPlayer.h"
#include "CBoss.h"
#include "../Scene/CInGameScene.h"
#include "../Core/CSceneManager.h"

CUIPanel::CUIPanel() :
	m_ePanelType(PT_NONE)
{
}

CUIPanel::CUIPanel(const CUIPanel & ui) :
	CUI(ui)
{
}


CUIPanel::~CUIPanel()
{
}

void CUIPanel::HPBarUpdate()
{
	float m_HPPercent = (float)PLAYER->GetHP() / (float)PLAYER->GetHPMax();

	m_HPPercent = m_HPPercent <= 0.f ? 0.f : m_HPPercent;

	SetRenderSize(496.f * m_HPPercent, m_tRenderSize.y);
}

void CUIPanel::BossHPBarUpdate()
{
	float m_HPPercent = (float)BOSS->GetHP() / (float)BOSS->GetHPMax();

	m_HPPercent = m_HPPercent <= 0.f ? 0.f : m_HPPercent;

	SetRenderSize(496.f * m_HPPercent, m_tRenderSize.y);
}

void CUIPanel::AttackLvUpdate()
{

	float m_AttackPer = (float)PLAYER->GetAttackLv() / (float)ATTACK_LV_MAX;

	SetRenderSize(50.f * m_AttackPer, m_tRenderSize.y);

}

void CUIPanel::SpeedLvUpdate()
{

	float m_SpeedPer = (float)PLAYER->GetSpeedLv() / (float)ATTACK_LV_MAX;

	SetRenderSize(45.f * m_SpeedPer, m_tRenderSize.y);

}

void CUIPanel::CoolDownA()
{

	float m_CoolDownPer = (PLAYER->GetBallCreateLimitTime() * (float)PLAYER->GetBallCount() + PLAYER->GetBallCreateTime());

	m_CoolDownPer /= (PLAYER->GetBallCreateLimitTime() * (float)PLAYER->GetBallLimitCount());

	m_CoolDownPer = m_CoolDownPer > 1.f ? 1.f : m_CoolDownPer;

	SetRenderSize(47.f * m_CoolDownPer, 47.f);

}

void CUIPanel::CoolDownS()
{

	float m_CoolDownPer = PLAYER->GetShieldTime() / PLAYER->GetShieldLimitTime();

	SetRenderSize(47.f * m_CoolDownPer, 47.f);
}

void CUIPanel::CoolDownD()
{

	float m_CoolDownPer = PLAYER->GetMisailTime() / PLAYER->GetMisailLimitTime();

	SetRenderSize(47.f * m_CoolDownPer, 47.f);
}

void CUIPanel::CoolDownR()
{

	float m_CoolDownPer = PLAYER->GetBombTime() / PLAYER->GetBombLimitTime();

	SetRenderSize(47.f * m_CoolDownPer, 47.f);
}

bool CUIPanel::Init()
{

	return true;
}

void CUIPanel::Input(float fDeltaTime)
{
}

void CUIPanel::Collision(float fDeltaTime)
{
	CUI::Collision(fDeltaTime);

}

void CUIPanel::Update(float fDeltaTime)
{
	CUI::Update(fDeltaTime);

	// 자신의 flag에 따라서 처리

	// OBJECT는 죽더라도, UI는 남는다는 점을 이용하여 장면을 전환한다.

	switch (m_ePanelType)
	{
	case PT_PLAYER_HPBAR:

		HPBarUpdate();

		if(!CInGameScene::m_bPlayer)
		{
			// 플레이어가 사망.
			GAMETIME += fDeltaTime;

			if (GAMETIME >= 3.f)
				GET_SINGLE(CSceneManager)->EndScene();
		}
		break;
		
	case PT_BOSS_HPBAR:

		BossHPBarUpdate();

		if (!CInGameScene::m_bBoss)
		{
			// 보스가 사망
			GAMETIME += fDeltaTime;

			if (GAMETIME >= 3.f)
				GET_SINGLE(CSceneManager)->EndScene();
		}

		break;
		
	case PT_ATTACK_LV:

		AttackLvUpdate();

		break;

	case PT_SPEED_LV:

		SpeedLvUpdate();

		break;

		// Image
	case PT_IMAGE:
		break;



	case PT_SKILL_COOLDOWN_A:

		 CoolDownA();

		
		break;

	case PT_SKILL_COOLDOWN_S:

		CoolDownS();

		break;

	case PT_SKILL_COOLDOWN_D:

		CoolDownD();


		break;

	case PT_SKILL_COOLDOWN_R:

		CoolDownR();


		break;
	}
}

void CUIPanel::Render(HDC hDC, float fDeltaTime)
{
	CUI::Render(hDC, fDeltaTime);
	

}

CUIPanel * CUIPanel::Clone()
{
	return new CUIPanel(*this);
}
