#include "Texture\CTexture.h"
#include "Animation/CAnimation.h"
#include "Object/CSkillBullet.h"
#include "Object/CMoveObj.h"
#include "Core\CSceneManager.h"
#include "Scene\CInGameScene.h"

bool CObj::m_bCollider;



CObj::CObj() :
	m_iRef(1),
	m_bLife(true),
	m_bBlend(false),
	m_fBlendLimitTime(0.f),
	m_fBlendTime(0.f),
	m_pTexture(NULL),
	m_pAnimation(NULL),
	m_tSize(0.f, 0.f),
	m_eShape(ST_RECT),
	m_fSpeed(0.f),
	m_fAngle(0.f),
	m_tRenderPos(-200.f, -200.f),
	m_tRenderSize(0.f, 0.f)
{
}

CObj::CObj(const CObj & Obj)
{
	*this = Obj;

	m_fBlendTime = 0.f;

	if (Obj.m_pAnimation)
	{
		m_pAnimation = Obj.m_pAnimation->Clone();
	}
	else if (Obj.m_pTexture)
	{
		Obj.m_pTexture->AddRef();
	}
}


CObj::~CObj()
{

	if (m_pAnimation)
	{
		SAFE_RELEASE(m_pAnimation);
	}
	else if (m_pTexture)
		SAFE_RELEASE(m_pTexture);
}


bool CObj::Init()
{
	return false;
}

void CObj::Input(float fDeltaTime)
{
}

void CObj::Collision(float fDeltaTime)
{
	
}

void CObj::Update(float fDeltaTime)
{
	if (m_pAnimation)
	{
		m_pAnimation->Update(fDeltaTime);
	}

	// ��¿� ��ǥ�� �浹ü�� ��ǥ�� ���� �����Ѵ�.
	// �浹ü�� �������� ������ LeftTop�� ����Ͽ� RenderPos�� ���Ѵ�.
		m_tRenderPos = m_tPos + m_tLeftTop;

}

void CObj::Render(HDC hDC, float fDeltaTime)
{
	// ���� ��ǥ -> ��ũ�� ��ǥ
	POSITION tRenderPos;
	tRenderPos.x = -CInGameScene::l + m_tRenderPos.x;
	tRenderPos.y = -CInGameScene::t + m_tRenderPos.y;


		POSITION tImagePos;
		
		if (m_pAnimation)
		{
			PANIMATIONCLIP pClip = m_pAnimation->GetCurrentClip();

			// ����Ҷ� left + whitex top + whitey X�� ���� Y�� ����
			// FrameSize ����Ͽ�

			// ������X ������ * �ε��� + x����.
			tImagePos.x = pClip->iFrameX * pClip->fFrameSizeX + pClip->fWhiteX;

			// y�� ����
			tImagePos.y = pClip->iFrameY * pClip->fFrameSizeY + pClip->fWhiteY;

			// �̷��� ���� ���� left top.
		}

				// ���� ���� �߰�.

				if (!m_bBlend) // ���� OFF
				{
					TransparentBlt(hDC, tRenderPos.x, tRenderPos.y, m_tRenderSize.x, m_tRenderSize.y, m_pTexture->GetDC(),
						tImagePos.x, tImagePos.y, m_tRenderSize.x, m_tRenderSize.y, m_pTexture->GetColor());
				}
				else // ���� ON
				{
					// ������ �� ���� �߰� �޸� DC�� �ϳ� �� �ʿ��ϴ�.

					HDC m_Alpha = CreateCompatibleDC(hDC);
					HBITMAP m_AlphaBit = CreateCompatibleBitmap(hDC, m_pTexture->GetWidth(), m_pTexture->GetHeight());
					HBITMAP m_OldAlphaBit = (HBITMAP)SelectObject(m_Alpha, m_AlphaBit);

					
					BitBlt(m_Alpha, 0, 0, m_tRenderSize.x, m_tRenderSize.y, hDC, tRenderPos.x, tRenderPos.y, SRCCOPY);

					TransparentBlt(m_Alpha, 0, 0, m_tRenderSize.x, m_tRenderSize.y,
						m_pTexture->GetDC(), tImagePos.x, tImagePos.y, 
						m_tRenderSize.x, m_tRenderSize.y, m_pTexture->GetColor());

					BLENDFUNCTION bf;
					bf.BlendOp = 0;
					bf.BlendFlags = 0;
					bf.SourceConstantAlpha = 130;
					bf.AlphaFormat = 0;

					// ��� ������ �A ����, �����ϰ� �Ͽ� backDC�� �ѱ�
					AlphaBlend(hDC, tRenderPos.x, tRenderPos.y, m_tRenderSize.x, m_tRenderSize.y,
						m_Alpha, 0, 0, m_tRenderSize.x, m_tRenderSize.y, bf);

					// ������ ���� ���� ����
					SelectObject(m_Alpha, m_OldAlphaBit);
					DeleteObject(m_AlphaBit);
					DeleteDC(m_Alpha);
				}


				if (CObj::m_bCollider)
				{
					switch (m_eShape)
					{
					case ST_RECT:
						Rectangle(hDC, -CInGameScene::l + m_tPos.x, -CInGameScene::t + m_tPos.y,
							-CInGameScene::l + m_tPos.x + m_tSize.x, -CInGameScene::t + m_tPos.y + m_tSize.y);

						break;

					case ST_CIRCLE:
						Ellipse(hDC, -CInGameScene::l + m_tPos.x, -CInGameScene::t + m_tPos.y, 
							-CInGameScene::l + m_tPos.x + m_tSize.x, -CInGameScene::t + m_tPos.y + m_tSize.y);

						break;
					}

				}

	
}

CObj * CObj::Clone()
{
	return new CObj(*this);
}

void CObj::SetRender(POSITION tLeftTop, _SIZE tRenderSize)
{
	SetRenderSize(tRenderSize);
	SetLeftTop(tLeftTop);
}

CObj * CObj::CreateMonster(const string & strTag, POSITION tPos)
{
	CObj* pMonster = FIND(CObj)->FindObj(strTag)->Clone();

	if (!pMonster)
	{
		SAFE_RELEASE(pMonster);
		return NULL;

	}
	
	pMonster->SetAnimation();

	pMonster->SetPos(tPos);

	int idx = rand() % 2;

	switch (idx)
	{
	case 0: ((CMoveObj*)pMonster)->SetStandDir(SD_RIGHT); break;
	case 1: ((CMoveObj*)pMonster)->SetStandDir(SD_LEFT); break;
	}

	return pMonster;
}

CObj * CObj::CreateEffect(const string & strTag, POSITION tPos)
{

	CObj* pEffect = FIND(CObj)->FindObj(strTag)->Clone();

	if (!pEffect)
	{
		SAFE_RELEASE(pEffect);
		return NULL;

	}

	pEffect->SetAnimation();

	pEffect->SetPos(tPos);

	return pEffect;
}

CObj * CObj::CreateItem(const string & strTag, POSITION tPos)
{
	CObj* pItem = FIND(CObj)->FindObj(strTag)->Clone();

	pItem->SetPos(tPos);

	return pItem;
}

void CObj::CreateSpecialAttack2(float y)
{
	CSkillBullet* pBullet = FIND(CSkillBullet)->FindObj("Energy")->Clone();

	pBullet->SetPos(0.f, y);

	pBullet->SetBulletType(BT_BOSS_SPECIAL2_2);

	pBullet->SetAnimation();

	GET_SINGLE(CSceneManager)->GetDeadMonsterBulletList().push_back(pBullet);
}

CAnimation * CObj::CreateAnimation(const string & strTag)
{
	SAFE_RELEASE(m_pAnimation);

	m_pAnimation = new CAnimation;

	m_pAnimation->SetTag(strTag);

	return m_pAnimation;
}


POSITION CObj::GetBulletPos(POSITION tMiddlePos, float fDist, float fAngle, CBullet * pBullet)
{
	POSITION m_tPos;

		m_tPos.x = tMiddlePos.x + fDist * cosf(fAngle) - (pBullet->GetSize().x / 2.f);
		m_tPos.y = tMiddlePos.y + fDist * sinf(fAngle) - (pBullet->GetSize().y / 2.f);
	
	return m_tPos;
}

POSITION CObj::GetBulletPos(POSITION tMiddlePos, float fDist, float fAngle, float addx, float addy)
{
	POSITION m_tPos;

	m_tPos.x = tMiddlePos.x + fDist * cosf(fAngle) + addx;
	m_tPos.y = tMiddlePos.y + fDist * sinf(fAngle) + addy;

	return m_tPos;
}

POSITION CObj::GetShieldPos(POSITION tPlayerCenter, _SIZE pBulletSize)
{
	POSITION tPos;

	tPos.x = tPlayerCenter.x - (pBulletSize.x / 2.f);
	tPos.y = tPlayerCenter.y - (pBulletSize.y / 2.f);

	return tPos;
}

void CObj::SetAnimation()
{

	if (!m_pAnimation)
		return;

	m_pAnimation->SetObj(this);
	m_pAnimation->ChangeClip(m_pAnimation->GetDefaultClip());
}

void CObj::SetAnimation(const string & strTag)
{

	if (!m_pAnimation)
		return;

	m_pAnimation->SetObj(this);
	m_pAnimation->ChangeClip(strTag);
}

