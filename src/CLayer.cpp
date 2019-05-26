#include "Layer\CLayer.h"
#include "Object\CObj.h"

CLayer::CLayer()
{
}


CLayer::~CLayer()
{
	Safe_Release_VecList(m_ObjList);
}


bool CLayer::Init()
{

	return true;
}

void CLayer::Input(float fDeltaTime)
{


	list<CObj*>::iterator iter;
	list<CObj*>::iterator iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd;)
	{
		if (!(*iter)->GetLife())
		{
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
			continue;
		}

		(*iter)->Input(fDeltaTime);
		++iter;
	}



}

void CLayer::Collision(float fDeltaTime) 
{

	list<CObj*>::iterator iter;
	list<CObj*>::iterator iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd; )
	{
		if (!(*iter)->GetLife())
		{
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
			continue;
		}

		(*iter)->Collision(fDeltaTime);
		++iter;
	}
	
}

void CLayer::Update(float fDeltaTime)
{
	list<CObj*>::iterator iter;
	list<CObj*>::iterator iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd; )
	{
		if (!(*iter)->GetLife())
		{
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
			continue;
		}

		(*iter)->Update(fDeltaTime);
		++iter;
	}

}

void CLayer::Render(HDC hDC, float fDeltaTime)
{
	list<CObj*>::iterator iter;
	list<CObj*>::iterator iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd; )
	{
		if (!(*iter)->GetLife())
		{
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
			continue;
		}

		(*iter)->Render(hDC, fDeltaTime);
		++iter;
	}

}

bool CLayer::LayerSort(CLayer * pL1, CLayer * pL2)
{
	return pL1->GetZOrder() < pL2->GetZOrder();
}
