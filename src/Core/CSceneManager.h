#pragma once

#include "../Game.h"

class CSceneManager
{
	DECLARE_SINGLE(CSceneManager);

private:
	class CScene* m_pScene;
	class CScene* m_pNextScene;

	HWND m_hWnd;


public:

	HWND GetHandle() const
	{
		return m_hWnd;
	}

	bool Init(HWND hWnd);
	void Input(float fDeltaTime);
	void Collision(float fDeltaTime);
	CHANGE_SCENE Update(float fDeltaTime);
	void Render(HDC hDC, float fDeltaTime);
	
private:
	CHANGE_SCENE ChangeScene();
	

public:
	list<class CObj*>& GetMonsterList();
	list<class CObj*>& GetDeadMonsterBulletList();
	list<class CObj*>& GetFirstEffectList();
	list<class CObj*>& GetSecondEffectList();
	list<class CObj*>& GetUIList();

	class CScene* GetCurrentScene() const
	{
		return m_pScene;
	}

	void InGameScene();
	void StartScene();
	void EndScene();

public:
	template <typename T>
	T* CreateScene(SCENE_CLIP eClip)
	{
		T* pScene = new T;

		if (!pScene->Init())
		{
			SAFE_DELETE(pScene);
			return NULL;
		}

		switch (eClip)
		{
		case SC_CURRENT:
			SAFE_DELETE(m_pScene);
			m_pScene = pScene;

			break;

		case SC_NEXT:
			SAFE_DELETE(m_pNextScene);
			m_pNextScene = pScene;
			break;
		}

		return pScene;
	}
	
};

