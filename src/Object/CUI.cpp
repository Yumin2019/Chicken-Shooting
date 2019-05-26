#include "CUI.h"
#include "../Texture/CTexture.h"


CUI::CUI() :
	m_WhiteX(0.f),
	m_WhiteY(0.f)
{
}

CUI::CUI(const CUI & ui) :
	CObj(ui)
{
}


CUI::~CUI()
{
}


void CUI::Input(float fDeltaTime)
{
}

void CUI::Collision(float fDeltaTime)
{
}

void CUI::Update(float fDeltaTime)
{
	CObj::Update(fDeltaTime);
}

void CUI::Render(HDC hDC, float fDeltaTime)
{

	// UI는 Obj의 Render를 부르지 않고 따로 처리한다.
	// RenderPos 및 m_tPos는 스크린 좌표를 기준으로 한다.


			if (!m_bBlend) // 블렌딩 OFF
			{
				TransparentBlt(hDC, m_tRenderPos.x , m_tRenderPos.y, m_tRenderSize.x, m_tRenderSize.y, m_pTexture->GetDC(),
					m_WhiteX, m_WhiteY, m_tRenderSize.x, m_tRenderSize.y, m_pTexture->GetColor());
				
			}
			else // 블렌딩 ON
			{

				BLENDFUNCTION bf;
				bf.BlendOp = 0;
				bf.BlendFlags = 0;
				bf.SourceConstantAlpha = 130;
				bf.AlphaFormat = 0;

				// 투명하게 하여 backDC에 넘김
				AlphaBlend(hDC, m_tRenderPos.x, m_tRenderPos.y, m_tRenderSize.x, m_tRenderSize.y,
					m_pTexture->GetDC(), 0, 0, m_tRenderSize.x, m_tRenderSize.y, bf);

			}
		

			if (CObj::m_bCollider)
			{
						switch (m_eShape)
						{
								case ST_RECT:
									Rectangle(hDC, m_tPos.x, m_tPos.y, m_tPos.x + m_tSize.x, +m_tPos.y + m_tSize.y);

									break;

								case ST_CIRCLE:
									Ellipse(hDC, m_tPos.x, m_tPos.y, m_tPos.x + m_tSize.x, +m_tPos.y + m_tSize.y);

									break;
						}
			}

}

