#include "CUIButton.h"



CUIButton::CUIButton() :
	m_eBtn(BS_NONE),
	m_eButtonWhat(BW_NONE)
{
}

CUIButton::CUIButton(const CUIButton & ui) :
	CUI(ui)
{
	m_eBtn = BS_NONE;
}


CUIButton::~CUIButton()
{
}

bool CUIButton::Init()
{
	return true;
}

void CUIButton::Input(float fDeltaTime)
{
	CUI::Input(fDeltaTime);
}

void CUIButton::Collision(float fDeltaTime)
{
	CUI::Collision(fDeltaTime);

	
}

void CUIButton::Update(float fDeltaTime)
{
	CUI::Update(fDeltaTime);

}

void CUIButton::Render(HDC hDC, float fDeltaTime)
{
	CUI::Render(hDC, fDeltaTime);

}

CUIButton * CUIButton::Clone()
{
	return new CUIButton(*this);
}
