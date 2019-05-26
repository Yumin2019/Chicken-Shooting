#include "Core\CSceneManager.h"
#include "Scene\CInGameScene.h"
#include "Scene/CStartScene.h"
#include "Scene/CEndScene.h"
#include "Layer/CLayer.h"

DEFINITION_SINGLE(CSceneManager);

CSceneManager::CSceneManager() :
	m_pScene(NULL),
	m_pNextScene(NULL)
{
}


CSceneManager::~CSceneManager()
{
	SAFE_DELETE(m_pScene);
	SAFE_DELETE(m_pNextScene);
}

bool CSceneManager::Init(HWND hWnd)
{

	m_hWnd = hWnd;

	CreateScene<CStartScene>(SC_CURRENT);


	

	return true;
}

void CSceneManager::Input(float fDeltaTime)
{
	m_pScene->Input(fDeltaTime);
}

void CSceneManager::Collision(float fDeltaTime)
{
	m_pScene->Collision(fDeltaTime);

}

CHANGE_SCENE CSceneManager::Update(float fDeltaTime)
{
	m_pScene->Update(fDeltaTime);

	return ChangeScene();

}

void CSceneManager::Render(HDC hDC, float fDeltaTime)
{
	m_pScene->Render(hDC, fDeltaTime);


}



CHANGE_SCENE CSceneManager::ChangeScene()
{
	if (m_pNextScene)
	{
		SAFE_DELETE(m_pScene);
		m_pScene = m_pNextScene;
		m_pNextScene = NULL;

		return CS_CHANGE;
	}

	return CS_NONE;
}

list<CObj*>& CSceneManager::GetMonsterList()
{
	return m_pScene->FindLayer("MonsterLayer")->GetList();
}

list<CObj*>& CSceneManager::GetDeadMonsterBulletList()
{
	return m_pScene->FindLayer("DeadMonsterBulletLayer")->GetList();
}

list<CObj*>& CSceneManager::GetFirstEffectList()
{
	return m_pScene->FindLayer("FirstEffectLayer")->GetList();

}

list<CObj*>& CSceneManager::GetSecondEffectList()
{
	return m_pScene->FindLayer("SecondEffectLayer")->GetList();

}

list<class CObj*>& CSceneManager::GetUIList()
{
	return m_pScene->FindLayer("UILayer")->GetList();

}

void CSceneManager::InGameScene()
{
	CreateScene<CInGameScene>(SC_NEXT);

}

void CSceneManager::StartScene()
{
	CreateScene<CStartScene>(SC_NEXT);

}

void CSceneManager::EndScene()
{
	CreateScene<CEndScene>(SC_NEXT);
}

