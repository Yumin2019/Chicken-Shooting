#include "CTexture.h"



CTexture::CTexture() :
	m_iRef(1),
	m_hMemDC(NULL),
	m_Color(RGB(255,0,255))
{
}


CTexture::~CTexture()
{
	// 기존에 잡고 있던 도구로 교체한다.
	SelectObject(m_hMemDC, m_hOldBitMap);

	// 비트맵을 삭제한다
	DeleteObject(m_hBitMap);

	// 메모리 DC를 삭제한다
	DeleteDC(m_hMemDC);
}

bool CTexture::Init(const wstring & strName)
{
	m_hMemDC = CreateCompatibleDC(GET_SINGLE(CCreateManager)->GetMemDC());

	m_hBitMap = (HBITMAP)LoadImage(GET_SINGLE(CCreateManager)->GetHInst(), strName.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	// 이미지를 메모리 DC에 장착하고 기존에 잡고 있던 도구를 받아둔다.

	m_hOldBitMap = (HBITMAP)SelectObject(m_hMemDC, m_hBitMap);

	GetObject(m_hBitMap, sizeof(m_tInfo), &m_tInfo);

	return true;
}

CTexture* CTexture::SetTexture(const wstring & strName, unsigned int r, unsigned int g, unsigned int b)
{
		wstring Text = L"Texture/" + strName;

		CTexture* m_pTexture = new CTexture;

		m_pTexture->Init(Text);

		m_pTexture->SetColor(r, g, b);

		return m_pTexture;
}


