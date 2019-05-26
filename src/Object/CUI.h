#pragma once
#include "CObj.h"


class CUI :
	public CObj
{
public:
	CUI();
	CUI(const CUI& ui);
	virtual ~CUI() = 0;



private:
	float m_WhiteX;
	float m_WhiteY;

	// Object�� ������ǥ -> ȭ����ǥ
	// UI�� ȭ����ǥ�� �ٷ� ���

public:


	void SetWhite(float x, float y)
	{
		m_WhiteX = x;
		m_WhiteY = y;
	}

	virtual bool Init() = 0;
	virtual void Input(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Update(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual CUI* Clone() = 0;
};

