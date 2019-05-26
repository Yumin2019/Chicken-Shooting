#include "CScene.h"
#include "../Layer/CLayer.h"
#include "../Texture/CTexture.h"
#include "../Scene/CInGameScene.h"


CScene::CScene() :
	m_pScreen(NULL),
	m_eSceneClip(SC_NONE)
{
}


CScene::~CScene()
{
	Safe_Delete_VecList(m_LayerList);
	SAFE_RELEASE(m_pScreen);
}

void CScene::Input(float fDeltaTime)
{
	list<CLayer*>::iterator iter;
	list<CLayer*>::iterator iterEnd = m_LayerList.end();

	for (iter = m_LayerList.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->Input(fDeltaTime);
	}
}

void CScene::Collision(float fDeltaTime)
{
	list<CLayer*>::iterator iter;
	list<CLayer*>::iterator iterEnd = m_LayerList.end();

	for (iter = m_LayerList.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->Collision(fDeltaTime);
	}
}

void CScene::Update(float fDeltaTime)
{
	list<CLayer*>::iterator iter;
	list<CLayer*>::iterator iterEnd = m_LayerList.end();

	for (iter = m_LayerList.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->Update(fDeltaTime);
	}
}

void CScene::Render(HDC hDC, float fDeltaTime)
{
	// 배경화면을 출력한다.

	switch (m_eInfo)
	{
	case SI_MAIN:
	case SI_END:

		BitBlt(hDC, 0, 0, m_pScreen->GetWidth(), m_pScreen->GetHeight(), m_pScreen->GetDC(), 0, 0, SRCCOPY);

		break;
	case SI_INGAME:
		BitBlt(hDC, -CInGameScene::l, -CInGameScene::t, m_pScreen->GetWidth(), m_pScreen->GetHeight(), m_pScreen->GetDC(), 0, 0, SRCCOPY);

		break;
	}

	list<CLayer*>::iterator iter;
	list<CLayer*>::iterator iterEnd = m_LayerList.end();

	for (iter = m_LayerList.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->Render(hDC, fDeltaTime);
	}
}

CLayer * CScene::FindLayer(const string & strTag)
{
	list<CLayer*>::iterator iter;
	list<CLayer*>::iterator iterEnd = m_LayerList.end();

	for (iter = m_LayerList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->GetTag() == strTag)
			return (*iter);
	}

	return NULL;
}

CLayer * CScene::CreateLayer(const string & strTag, int ZOrder)
{
	CLayer* pLayer = new CLayer;

	pLayer->SetTag(strTag);

	pLayer->SetZOrder(ZOrder);

	m_LayerList.push_back(pLayer);

	if (m_LayerList.size() >= 2)
	{
		m_LayerList.sort(CLayer::LayerSort);
		// sort
	}

	return pLayer;
}
