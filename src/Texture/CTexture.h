#pragma once

#include "../Core/CCreateManager.h"

class CTexture
{
public:
	CTexture();
	~CTexture();

private:
	HDC m_hMemDC;

	HBITMAP m_hBitMap;
	HBITMAP m_hOldBitMap;

	BITMAP m_tInfo;

	int m_iRef;

	COLORREF m_Color;


public:

	// Color

	void SetColor(unsigned int r, unsigned int g, unsigned int b)
	{
		m_Color = RGB(r, g, b);
	}

	void SetColor(COLORREF color)
	{
		m_Color = color;
	}

	COLORREF GetColor() const
	{
		return m_Color;
	}

	HDC GetDC() const
	{
		return m_hMemDC;
	}

	// Texture�� �ʺ�� ���� ������

	int GetWidth() const
	{
		return m_tInfo.bmWidth;
	}


	int GetHeight() const
	{
		return m_tInfo.bmHeight;
	}

	// ������
	static CTexture* SetTexture(const wstring & strName, unsigned int r, unsigned int g, unsigned int b);

	// wstring - ��� ����.
	bool Init(const wstring& strName);

public:

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

