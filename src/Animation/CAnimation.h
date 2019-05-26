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

	PANIMATIONCLIP m_pCurClip; // ���� ������ �ִ� ��.

	string m_strCurClip; // ���� ������ �ִ� Ŭ���� �̸�

	string m_strDefaultClip; // �����ٰ� ������ �ٽ� ���ư� Ŭ���� �̸�

	class CObj* m_pObj; // �Ҽ� Obj;

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

