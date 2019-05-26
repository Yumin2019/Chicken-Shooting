#pragma once

#include "../Game.h"

class CCreateManager
{
	DECLARE_SINGLE(CCreateManager);

private:
	HINSTANCE m_hInst;
	HDC		  m_hDC;

public:
	bool Init(HINSTANCE hInst, HDC hDC);

	HDC GetMemDC() const
	{
		return m_hDC;
	}

	HINSTANCE GetHInst() const
	{
		return m_hInst;
	}

	/*
		Create 매니저에서 오브젝트 찾아오고, 추가하고 잡다한 기능을 모아서 한다.
	*/
	class CObj* FindObj(const string& strKey);

private:
	unordered_map<string, class CObj*> m_mapObj;

	void MinionACreate();
	void MinionBCreate();
	void BossCreate();
	
	void BossAttackBullet();

	class CTexture* CreateTexture(const wstring& strFileName, unsigned int r, unsigned int g, unsigned int b, class CObj* pObj);


	class CItem* CreateItem(_SIZE tSize, float fSpeed, float fLimitDist, SHAPE_TYPE eShape,
		int iDamage, float fLifeLimitTime, ITEM_TYPE eItem);
	

	class CSkillBullet* CreateSkillBullet(_SIZE tSize, float fSpeed, float fLimitDist, SHAPE_TYPE eShape, 
		int iDamage, float fLifeLimitTime);


	class CBulletA* CreateBulletA(_SIZE tSize, float fSpeed, float fLimitDist, SHAPE_TYPE eShape, int iDamage);

	void SetRender(POSITION tLeftTop, _SIZE tRenderSize, class CObj* pObj);

};

