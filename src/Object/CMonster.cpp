#include "CMonster.h"
#include "CBullet.h"


CMonster::CMonster() :
	m_fBulletTime(0.f),
	m_bXY(true)
{


}

CMonster::CMonster(const CMonster & mon) :
	CMoveObj(mon)
{
	
	m_fBulletTime = 0.f;
	m_fBulletLimitTime = mon.m_fBulletLimitTime;

	m_bXY = true;
}


CMonster::~CMonster()
{

}

bool CMonster::Init()
{
	return false;
}

void CMonster::Input(float fDeltaTime)
{
}

void CMonster::Collision(float fDeltaTime)
{
	CMoveObj::Collision(fDeltaTime);
	
	// Minion's Bullet Collision

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


}

void CMonster::Update(float fDeltaTime)
{
	CMoveObj::Update(fDeltaTime);


	// Minion's Bullet Move

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

}

void CMonster::Render(HDC hDC, float fDeltaTime)
{
	
	CMoveObj::Render(hDC, fDeltaTime);

	// Minion's Bullet Render

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


}

CMonster * CMonster::Clone()
{
	return new CMonster(*this);
}
