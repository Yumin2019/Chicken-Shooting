#include "CAnimation.h"
#include "../Texture/CTexture.h"
#include "../Object/CMoveObj.h"
#include "../Object/CBullet.h"
#include "../Core/CSceneManager.h"

CAnimation::CAnimation() :
	m_pCurClip(NULL),
	m_iRef(1),
	m_pObj(NULL)
{
}

CAnimation::CAnimation(const CAnimation & anim)
{
	*this = anim;

	m_mapClip.clear();

	unordered_map<string, PANIMATIONCLIP>::const_iterator iter;
	unordered_map<string, PANIMATIONCLIP>::const_iterator iterEnd = anim.m_mapClip.end();

	for (iter = anim.m_mapClip.begin(); iter != iterEnd; ++iter)
	{
		PANIMATIONCLIP pClip = new ANIMATIONCLIP;

		*pClip = *iter->second; 

		for (size_t i = 0; i < iter->second->vecTexture.size(); ++i)
		{
			pClip->vecTexture[i]->AddRef();
		}

		m_mapClip.insert(make_pair(iter->first, pClip));

	}

	m_pCurClip = NULL;
	m_strCurClip = "";
	
}


CAnimation::~CAnimation()
{

	unordered_map<string, PANIMATIONCLIP>::iterator iter;
	unordered_map<string, PANIMATIONCLIP>::iterator iterEnd = m_mapClip.end();

	for (iter = m_mapClip.begin(); iter != iterEnd; ++iter)
	{
		for (size_t i = 0; i < iter->second->vecTexture.size(); ++i)
		{
			SAFE_RELEASE(iter->second->vecTexture[i]);
		}
	
		SAFE_DELETE(iter->second);
	}

	m_mapClip.clear();
}

bool CAnimation::Init()
{
	return true;
}

void CAnimation::Update(float fTime)
{
	m_pCurClip->fAnimationTime += fTime;

	// 1 frame 시간 fAnimationFrameTime

	switch (m_pCurClip->eOption)
	{
	case AO_TIME_RETURN:
		
		m_pCurClip->fOptionTime += fTime;

		if (m_pCurClip->fOptionTime >= m_pCurClip->fOptionLimitTime)
		{
			ChangeClip(GetDefaultClip());
		}
		break;

	case AO_TIME_DESTROY:

		m_pCurClip->fOptionTime += fTime;

		if (m_pCurClip->fOptionTime >= m_pCurClip->fOptionLimitTime)
		{
			m_pObj->Die();
		}
		break;
	}



	while (m_pCurClip->fAnimationTime >= m_pCurClip->fAnimationFrameTime)
	{
		m_pCurClip->fAnimationTime -= m_pCurClip->fAnimationFrameTime;

		// 다음 장면을 가리켜야 한다.

		++m_pCurClip->iFrameX;

		if (m_pCurClip->iFrameX - m_pCurClip->iStartX == m_pCurClip->iLengthX)
		{
			// 0 부터 시작하여 센다.
			m_pCurClip->iFrameX = m_pCurClip->iStartX;
			++m_pCurClip->iFrameY;

			if (m_pCurClip->iFrameY - m_pCurClip->iStartY == m_pCurClip->iLengthY)
			{
				m_pCurClip->iFrameY = m_pCurClip->iStartY;

				switch (m_pCurClip->eOption)
				{
				case AO_ONCE_RETURN:
					ChangeClip(m_strDefaultClip);
					

					// Object에 상관없이 (MoveObj) 피격상태 false;

					((CMoveObj*)m_pObj)->SetAttacked(false);

					break;

				case AO_ONCE_DESTROY:

					// Die이후에 (시간 때문에 다시 들어올 가능성이 있음)

					m_pObj->Die(); // 파티클

					switch (((CBullet*)m_pObj)->GetBulletType())
					{
					case BT_BOSS_SPECIAL2_1: // 해골

						// 죽기전에 Bullet을 소환한다.

						// 밸런스 조정. 쉴드가 없으면 BOSS를 깨는 것이 불가능.

						// 해골의 자리에 10%의 확률로 방패 생성.

						if((rand() % 100 + 1) <= 10)
						GET_SINGLE(CSceneManager)->GetFirstEffectList().push_back(m_pObj->CreateItem("ShieldItem", m_pObj->GetPos()));

						m_pObj->CreateSpecialAttack2(m_pObj->GetPos().y);
						
						break;
					}

					
					return;


					// Option Time은 TIME 쪽에서 사용.


					// Time은 위에서

					// LOOP는 계속.


				}
			}
		}
		

	}
}

CAnimation * CAnimation::Clone()
{
	return new CAnimation(*this);
}

PANIMATIONCLIP CAnimation::FindClip(const string & strTag)
{
	unordered_map<string, PANIMATIONCLIP>::iterator iter = m_mapClip.find(strTag);

	if (iter == m_mapClip.end())
		return NULL;

	return iter->second;
}

void CAnimation::SetObj(CObj * pObj)
{
	m_pObj = pObj;
}

void CAnimation::ChangeClip(const string & strTag)
{

	if (strTag == m_strCurClip)
		return;

	m_strCurClip = strTag;

	if (m_pCurClip)
	{
		m_pCurClip->iFrameX = m_pCurClip->iStartX;
		m_pCurClip->iFrameY = m_pCurClip->iStartY;
		m_pCurClip->fAnimationTime = 0.f;
		m_pCurClip->fOptionTime = 0.f;


		// 다시 이용할 것을 대비하여 미리 셋팅한다.
	}


	m_pCurClip = FindClip(strTag);

	// 찾은 Clip을 오브젝트가 셋팅한다.
	
	m_pObj->SetTexture(m_pCurClip->vecTexture[0]); // 아틀라스
   
}

void CAnimation::SetDefaultClip(const string & strTag)
{
	m_strDefaultClip = strTag;
}

void CAnimation::SetCurrentClip(const string & strTag)
{
	m_strCurClip = strTag;
}


bool CAnimation::AddClip(const wstring & strTag, const string& What, ANIMATION_OPTION eOption,
	float fAnimationLimitTime, 
	int iStartX, int iStartY, 
	int iLengthX, int iLengthY,
	float fOptionLimitTime,
	float fWhiteX, float fWhiteY,
	float fFrameSizeX, float fFrameSizeY,
	unsigned int r, unsigned int g, unsigned int b)
{
	// 애니메이션 클립을 만드는 함수 (정보 셋팅 용도)


	PANIMATIONCLIP pClip = new ANIMATIONCLIP;

	pClip->eOption = eOption;
	pClip->fAnimationLimitTime = fAnimationLimitTime;
	pClip->iStartX = iStartX;
	pClip->iStartY = iStartY;
	pClip->iLengthX = iLengthX;
	pClip->iLengthY = iLengthY;
	pClip->fOptionLimitTime = fOptionLimitTime;
	pClip->fAnimationFrameTime = fAnimationLimitTime / (iLengthX* iLengthY);



	pClip->fAnimationTime = 0.f;
	pClip->iFrameX = iStartX; // startx
	pClip->iFrameY = iStartY; // starty
	pClip->fOptionTime = 0.f;


	pClip->fWhiteX = fWhiteX;
	pClip->fWhiteY = fWhiteY;
	pClip->fFrameSizeX = fFrameSizeX;
	pClip->fFrameSizeY = fFrameSizeY;


	// 텍스처

	pClip->vecTexture.push_back(CTexture::SetTexture(strTag, r, g, b));

	m_mapClip.insert(make_pair(What, pClip));

	// 기본 클립의 설정 / 현재 클립의 설정은 따로한다.


	return true;
}



