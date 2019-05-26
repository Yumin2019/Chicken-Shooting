#include "CItem.h"



CItem::CItem() :
	m_ItemType(IT_NONE)
{
}

CItem::CItem(CItem & Item) :
	CSkillBullet(Item)
{
	m_ItemType = Item.m_ItemType;
}


CItem::~CItem()
{
}

bool CItem::Init()
{
	return true;
}

void CItem::Input(float fDeltaTime)
{
	CSkillBullet::Input(fDeltaTime);
}

void CItem::Collision(float fDeltaTime)
{
	CSkillBullet::Collision(fDeltaTime);

}

void CItem::Update(float fDeltaTime)
{
	CSkillBullet::Update(fDeltaTime);

}

void CItem::Render(HDC hDC, float fDeltaTime)
{
	CSkillBullet::Render(hDC, fDeltaTime);

}

CItem * CItem::Clone()
{
	return new CItem(*this);
}
