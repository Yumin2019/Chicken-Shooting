#pragma once
#include "CUI.h"
class CUIPanel :
	public CUI
{
public:
	CUIPanel();
	CUIPanel(const CUIPanel& ui);
	~CUIPanel();

private:
	PANEL_TYPE m_ePanelType;



public:
	void SetPanelType(PANEL_TYPE eType)
	{
		m_ePanelType = eType;
	}

	PANEL_TYPE GetPenelType() const
	{
		return m_ePanelType;
	}


private:
	void HPBarUpdate();
	void BossHPBarUpdate();
	
	void AttackLvUpdate();
	void SpeedLvUpdate();

	void CoolDownA();
	void CoolDownS();
	void CoolDownD();
	void CoolDownR();


public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Update(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual CUIPanel* Clone();
};

