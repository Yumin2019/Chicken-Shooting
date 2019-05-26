#pragma once
#include "CSkillBullet.h"


class CItem :
	public CSkillBullet
{
public:
	CItem();
	CItem(CItem& Item);
	 ~CItem();

private:
	ITEM_TYPE m_ItemType;


public:
	ITEM_TYPE GetItemType() const
	{
		return m_ItemType;
	}

	void SetItem(ITEM_TYPE eType)
	{
		m_ItemType = eType;
	}

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Update(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual CItem* Clone();
};

