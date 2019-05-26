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

	// UI�� Obj�� Render�� �θ��� �ʰ� ���� ó���Ѵ�.
	// RenderPos �� m_tPos�� ��ũ�� ��ǥ�� �������� �Ѵ�.


			if (!m_bBlend) // ���� OFF
			{
				TransparentBlt(hDC, m_tRenderPos.x , m_tRenderPos.y, m_tRenderSize.x, m_tRenderSize.y, m_pTexture->GetDC(),
					m_WhiteX, m_WhiteY, m_tRenderSize.x, m_tRenderSize.y, m_pTexture->GetColor());
				
			}
			else // ���� ON
			{

				BLENDFUNCTION bf;
				bf.BlendOp = 0;
				bf.BlendFlags = 0;
				bf.SourceConstantAlpha = 130;
				bf.AlphaFormat = 0;

				// �����ϰ� �Ͽ� backDC�� �ѱ�
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

