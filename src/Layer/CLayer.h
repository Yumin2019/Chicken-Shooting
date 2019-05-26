#pragma once

#include "../Game.h"

class CLayer
{
public:
	CLayer();
	~CLayer();

private:
	friend class CScene;

private:
	string m_strTag;
	int m_iZOrder;
	list<class CObj*> m_ObjList;


public:
	void SetTag(const string& strTag)
	{
		m_strTag = strTag;
	}

	string GetTag() const
	{
		return m_strTag;
	}

	void SetZOrder(int idx)
	{
		m_iZOrder = idx;
	}

	int GetZOrder() const
	{
		return m_iZOrder;
	}

	list<class CObj*>& GetList()
	{
		return m_ObjList;
	}

public:
	bool Init();
	void Input(float fDeltaTime);
	void Collision(float fDeltaTime);
	void Update(float fDeltaTime);
	void Render(HDC hDC, float fDeltaTime);

private:
	static bool LayerSort(CLayer* pL1, CLayer* pL2);

};

