#pragma once
#include "CUI.h"



class CUIButton :
	public CUI
{
public:
	CUIButton();
	CUIButton(const CUIButton& ui);
	~CUIButton();

private:
	BUTTON_STATE m_eBtn;
	BUTTON_WHAT  m_eButtonWhat;


public:

	BUTTON_WHAT GetButtonWhat() const
	{
		return m_eButtonWhat;
	}

	void SetButtonWhat(BUTTON_WHAT eType)
	{
		m_eButtonWhat = eType;
	}


	void SetButtonState(BUTTON_STATE eState)
	{
		m_eBtn = eState;
	}

	BUTTON_STATE GetButtonState() const
	{
		return m_eBtn;
	}

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Update(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual CUIButton* Clone();
};

