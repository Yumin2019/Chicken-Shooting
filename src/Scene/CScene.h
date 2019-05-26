#pragma once

#include "../Game.h"

class CScene
{
protected:
	CScene();
	virtual ~CScene();


private:
	friend class CSceneManager;


protected:
	list<class CLayer*> m_LayerList;
	class CTexture* m_pScreen;
	SCENE_INFORM	m_eInfo;
	SCENE_CLIP      m_eSceneClip;


	

public:
	virtual bool Init() = 0;

	SCENE_CLIP GetSceneClip() const
	{
		return m_eSceneClip;
	}

	void SetSceneClip(SCENE_CLIP eClip)
	{
		m_eSceneClip = eClip;
	}


public:

	virtual void Input(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Update(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);

	class CLayer* FindLayer(const string& strTag);

	class CLayer* CreateLayer(const string& strTag, int ZOrder);

	
};

