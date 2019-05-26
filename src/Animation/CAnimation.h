#pragma once

#include "../Game.h"

class CAnimation
{
private:
	CAnimation();
	CAnimation(const CAnimation& anim);
	~CAnimation();

private:
	friend class CObj;


public:
	bool Init();
	void Update(float fTime);

	CAnimation* Clone();

private:
	unordered_map<string, PANIMATIONCLIP> m_mapClip;

	PANIMATIONCLIP m_pCurClip; // 지금 돌리고 있는 것.

	string m_strCurClip; // 현재 돌리고 있는 클립의 이름

	string m_strDefaultClip; // 돌리다가 끝나면 다시 돌아갈 클립의 이름

	class CObj* m_pObj; // 소속 Obj;

	int m_iRef;

	string m_strTag; // Who

private:
	PANIMATIONCLIP FindClip(const string& strTag);


public:
	void SetObj(CObj* pObj);
	void ChangeClip(const string& strTag);
	void SetDefaultClip(const string& strTag);
	void SetCurrentClip(const string& strTag);
	

	bool AddClip(const wstring& strTag, const string& What, ANIMATION_OPTION eOption,
		float fAnimationLimitTime,  int iStartX, int iStartY,
		int iLengthX, int iLengthY, float fOptionLimitTime, 
		float fWhiteX, float fWhiteY,
		float fFrameSizeX, float fFrameSizeY,
		unsigned int r, unsigned int g, unsigned int b);



public:
	// Tag

	void SetTag(const string& strTag)
	{
		m_strTag = strTag;
	}

	PANIMATIONCLIP GetCurrentClip() const
	{
		return m_pCurClip;
	}


	string GetDefaultClip() const
	{
		return m_strDefaultClip;
	}


	// Ref
	void AddRef()
	{
		++m_iRef;
	}

	int Release()
	{
		--m_iRef;
		if (m_iRef == 0)
		{
			delete this;
			return 0;
		}

		return m_iRef;
	}

};

