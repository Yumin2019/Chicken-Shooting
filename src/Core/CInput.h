#pragma once

#include "../Game.h"

typedef struct _tagInput
{
	string strTag;
	bool m_bDown;
	bool m_bPress;
	bool m_bUp;
	vector<DWORD> vecKey;

	_tagInput() :
		m_bDown(false),
		m_bPress(false),
		m_bUp(false)
	{}

}KEYINFO, *PKEYINFO;


class CInput
{
	DECLARE_SINGLE(CInput);


public:
	void Update(float fDeltaTime);
	bool Init();

private:
	PKEYINFO m_pKey;
	unordered_map<string, PKEYINFO> m_mapKey;
	

public:

	PKEYINFO FindKey(const string& strKey);
	bool KeyDown(const string& strKey);
	bool KeyUp(const string& strKey);
	bool KeyPress(const string& strKey);


	template<typename T>
	bool AddKey(const T& data)
	{

		if (!m_pKey)
			m_pKey = new KEYINFO;

		const char* m_Type = typeid(T).name();

		if (strcmp(m_Type, "int") == 0 ||
			strcmp(m_Type, "char") == 0)
		{
			m_pKey->vecKey.push_back((DWORD)data);
		}
		else
		{
			m_pKey->strTag = data;
			m_mapKey.insert(make_pair(m_pKey->strTag, m_pKey));
		}

		

		return true;
	}

	template<typename T, typename ... Types>
	bool AddKey(const T& data, const Types& ... arg)
	{

		if (!m_pKey)
			m_pKey = new KEYINFO;

		const char* m_Type = typeid(T).name();

		if (strcmp(m_Type, "int") == 0 ||
			strcmp(m_Type, "char") == 0)
		{
			m_pKey->vecKey.push_back((DWORD)data);
		}
		else
		{
			m_pKey->strTag = data;
			m_mapKey.insert(make_pair(m_pKey->strTag, m_pKey));
		}

		AddKey(arg...);

		if (m_pKey)
			m_pKey = NULL;

		return true;
	}

};

