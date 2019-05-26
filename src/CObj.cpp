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

	// 출력용 좌표와 충돌체의 좌표가 따로 존재한다.
	// 충돌체를 기준으로 설정한 LeftTop을 계산하여 RenderPos를 구한다.
		m_tRenderPos = m_tPos + m_tLeftTop;

}

void CObj::Render(HDC hDC, float fDeltaTime)
{
	// 월드 좌표 -> 스크린 좌표
	POSITION tRenderPos;
	tRenderPos.x = -CInGameScene::l + m_tRenderPos.x;
	tRenderPos.y = -CInGameScene::t + m_tRenderPos.y;


		POSITION tImagePos;
		
		if (m_pAnimation)
		{
			PANIMATIONCLIP pClip = m_pAnimation->GetCurrentClip();

			// 출력할때 left + whitex top + whitey X의 간격 Y의 간격
			// FrameSize 고려하여

			// 프레임X 사이즈 * 인덱스 + x여백.
			tImagePos.x = pClip->iFrameX * pClip->fFrameSizeX + pClip->fWhiteX;

			// y도 동일
			tImagePos.y = pClip->iFrameY * pClip->fFrameSizeY + pClip->fWhiteY;

			// 이렇게 구한 값이 left top.
		}

				// 블렌드 변수 추가.

				if (!m_bBlend) // 블렌딩 OFF
				{
					TransparentBlt(hDC, tRenderPos.x, tRenderPos.y, m_tRenderSize.x, m_tRenderSize.y, m_pTexture->GetDC(),
						tImagePos.x, tImagePos.y, m_tRenderSize.x, m_tRenderSize.y, m_pTexture->GetColor());
				}
				else // 블렌딩 ON
				{
					// 블렌딩을 할 때는 추가 메모리 DC가 하나 더 필요하다.

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

					// 배경 색깔을 뺸 것을, 투명하게 하여 backDC에 넘김
					AlphaBlend(hDC, tRenderPos.x, tRenderPos.y, m_tRenderSize.x, m_tRenderSize.y,
						m_Alpha, 0, 0, m_tRenderSize.x, m_tRenderSize.y, bf);

					// 이전에 쓰던 것을 장착
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

