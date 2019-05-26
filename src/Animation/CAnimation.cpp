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

	// 1 frame �ð� fAnimationFrameTime

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

		// ���� ����� �����Ѿ� �Ѵ�.

		++m_pCurClip->iFrameX;

		if (m_pCurClip->iFrameX - m_pCurClip->iStartX == m_pCurClip->iLengthX)
		{
			// 0 ���� �����Ͽ� ����.
			m_pCurClip->iFrameX = m_pCurClip->iStartX;
			++m_pCurClip->iFrameY;

			if (m_pCurClip->iFrameY - m_pCurClip->iStartY == m_pCurClip->iLengthY)
			{
				m_pCurClip->iFrameY = m_pCurClip->iStartY;

				switch (m_pCurClip->eOption)
				{
				case AO_ONCE_RETURN:
					ChangeClip(m_strDefaultClip);
					

					// Object�� ������� (MoveObj) �ǰݻ��� false;

					((CMoveObj*)m_pObj)->SetAttacked(false);

					break;

				case AO_ONCE_DESTROY:

					// Die���Ŀ� (�ð� ������ �ٽ� ���� ���ɼ��� ����)

					m_pObj->Die(); // ��ƼŬ

					switch (((CBullet*)m_pObj)->GetBulletType())
					{
					case BT_BOSS_SPECIAL2_1: // �ذ�

						// �ױ����� Bullet�� ��ȯ�Ѵ�.

						// �뷱�� ����. ���尡 ������ BOSS�� ���� ���� �Ұ���.

						// �ذ��� �ڸ��� 10%�� Ȯ���� ���� ����.

						if((rand() % 100 + 1) <= 10)
						GET_SINGLE(CSceneManager)->GetFirstEffectList().push_back(m_pObj->CreateItem("ShieldItem", m_pObj->GetPos()));

						m_pObj->CreateSpecialAttack2(m_pObj->GetPos().y);
						
						break;
					}

					
					return;


					// Option Time�� TIME �ʿ��� ���.


					// Time�� ������

					// LOOP�� ���.


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


		// �ٽ� �̿��� ���� ����Ͽ� �̸� �����Ѵ�.
	}


	m_pCurClip = FindClip(strTag);

	// ã�� Clip�� ������Ʈ�� �����Ѵ�.
	
	m_pObj->SetTexture(m_pCurClip->vecTexture[0]); // ��Ʋ��
   
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
	// �ִϸ��̼� Ŭ���� ����� �Լ� (���� ���� �뵵)


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


	// �ؽ�ó

	pClip->vecTexture.push_back(CTexture::SetTexture(strTag, r, g, b));

	m_mapClip.insert(make_pair(What, pClip));

	// �⺻ Ŭ���� ���� / ���� Ŭ���� ������ �����Ѵ�.


	return true;
}



