#pragma once

#include "../Game.h"

class CObj
{
protected:
	CObj();
	CObj(const CObj& Obj);
	virtual ~CObj();

protected:
	friend class CLayer;

private:
	friend class CCreateManager;



protected:

	int					m_iRef; // 오브젝트에서 ref.
	bool				m_bLife;
	bool				m_bBlend; // Blend.
	float				m_fBlendTime;
	float				m_fBlendLimitTime;
	float				m_fSpeed;
	float				m_fAngle;

	_SIZE				m_tSize;
	POSITION			m_tPos;

	_SIZE	 m_tLeftTop;

	POSITION m_tRenderPos;
	_SIZE	 m_tRenderSize;

	SHAPE_TYPE			m_eShape;
	COLLISION			m_tCollision;


	class CTexture*		m_pTexture;
	class CAnimation*	m_pAnimation;


public:
	static bool m_bCollider;



public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Update(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual CObj* Clone();

	// 오브젝트 추가
	template<typename T>
	static T* CreateObj(class CLayer* pLayer)
	{
		T* pObj = new T;

		if (!pObj->Init())
		{
			SAFE_DELETE(pObj);
			return NULL;
		}

		pLayer->GetList().push_back(pObj);

		return pObj;
	}

	void SetRender(POSITION tLeftTop, _SIZE tRenderSize);

public:
	// 각도 증가 함수

	void AddAngle(float f)
	{
		m_fAngle += f;
	}

	// 스피드 증가 함수

	void AddSpeed(float f)
	{
		m_fSpeed += f;
	}


	// Move : 일정한 속력으로 방향성을 가지고.

	void MoveX(float fDeltaTime, STAND_DIRECTION eDir)
	{
		switch (eDir)
		{
		case SD_RIGHT:
			m_tPos.x += fDeltaTime * m_fSpeed;

			break;

		case SD_LEFT:
			m_tPos.x += fDeltaTime * m_fSpeed * -1.f;

			break;
		}
	}


	void MoveY(float fDeltaTime, STAND_DIRECTION eDir)
	{
		switch (eDir)
		{
		case SD_RIGHT:
			m_tPos.y += fDeltaTime * m_fSpeed;

			break;

		case SD_LEFT:
			m_tPos.y += fDeltaTime * m_fSpeed * -1.f;

			break;
		}
	}

	// Move : 일정한 속력을 가지고 각도를 이용해서

	void MoveAngle(float fDeltaTime)
	{
		m_tPos.x += fDeltaTime * m_fSpeed * cosf(m_fAngle);
		m_tPos.y += fDeltaTime * m_fSpeed * sinf(m_fAngle);
	}


	// Angle

	void SetAngle(float fAngle)
	{
		m_fAngle = fAngle;
	}

	float GetAngle() const
	{
		return m_fAngle;
	}

	// Speed

	float GetSpeed() const
	{
		return m_fSpeed;
	}

	void SetSpeed(float fSpeed)
	{
		m_fSpeed = fSpeed;
	}



	COLLISION GetCollision() const
	{
		return m_tCollision;
	}

	// Collision Set

	void SetCollStart(bool b)
	{
		m_tCollision.m_bStart = b;
	}

	void SetCollStay(bool b)
	{
		m_tCollision.m_bStay = b;
	}

	void SetCollEnd(bool b)
	{
		m_tCollision.m_bEnd = b;
	}

	// Size

	void SetSize(_SIZE& tPos)
	{
		m_tSize = tPos;
	}

	void SetSize(float x, float y)
	{
		m_tSize = _SIZE(x, y);
	}

	_SIZE GetSize()
	{
		return m_tSize;
	}


	// Pos

	void SetPos(POSITION& tPos)
	{
		m_tPos = tPos;
	}

	void SetPos(float x, float y)
	{
		m_tPos = POSITION(x, y);
	}

	POSITION GetPos()
	{
		return m_tPos;
	}


	// Shape

	SHAPE_TYPE GetShape() const
	{
		return m_eShape;
	}

	void SetShape(SHAPE_TYPE eShape)
	{
		m_eShape = eShape;
	}

	// l t r b

	float GetLeft() const
	{
		return m_tPos.x;
	}

	float GetRight() const
	{
		return GetLeft() + m_tSize.x;
	}

	float GetTop() const
	{
		return m_tPos.y;
	}

	float GetBottom() const
	{
		return GetTop() + m_tSize.y;
	}

	// Center

	float GetCenterX() const
	{
		return (GetLeft() + GetRight()) / 2.f;
	}

	float GetCenterY() const
	{
		return (GetTop() + GetBottom()) / 2.f;
	}

	POSITION GetCenter() const
	{
		return POSITION(GetCenterX(), GetCenterY());
	}



	// Blend
	void SetBlend(bool Blend)
	{
		m_bBlend = Blend;
	}

	float GetBlend() const
	{
		return m_bBlend;
	}

	void SetBlendLimitTime(float fTime)
	{
		m_fBlendLimitTime = fTime;
	}


	// map 프로토타입
	void SetTexture(class CTexture* pTexture)
	{
		m_pTexture = pTexture;
	}


	
public:

	 CObj* CreateMonster(const string& strTag, POSITION tPos);

	 CObj* CreateEffect(const string& strTag, POSITION tPos);

	 CObj* CreateItem(const string& strTag, POSITION tPos);
	
	 void CreateSpecialAttack2(float y);

	// 애니메이션 생성

	class CAnimation* CreateAnimation(const string& strTag);
	


public:

	class CAnimation* GetAnimation() const
	{
		return m_pAnimation;
	}

	class CTexture* GetTexture() const
	{
		return m_pTexture;
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



	// Life
	void Die()
	{
		m_bLife = false;
	}
	

	bool GetLife() const
	{
		return m_bLife;
	}


	// 우리는 비율을 가지고 계산을 하고 있기 때문에, lefttop만 잡아주면 된다.

	// Left Top 

	void SetLeftTop(float x, float y) // 비율.
	{
		m_tLeftTop = _SIZE(x, y);
	}

	void SetLeftTop(POSITION tPos)
	{
		m_tLeftTop = tPos;
	}

	// Render Pos

	POSITION GetRenderPos()
	{
		return m_tRenderPos;
	}


	// Render Size


	void SetRenderSize(_SIZE& tPos)
	{
		m_tRenderSize = tPos;
	}

	void SetRenderSize(float x, float y)
	{
		m_tRenderSize = _SIZE(x, y);
	}

	_SIZE GetRendderSize()
	{
		return m_tRenderSize;
	}



	// 바라보는 각도에 따라서 Angle을 지정해주는 함수이다.

	float GetDirAngle(STAND_DIRECTION eDir)
	{
		float fAngle = 0.f;
		switch (eDir)
		{
		case SD_RIGHT:
			break;

		case SD_LEFT:
			fAngle = PI;
			break;

		}

		return fAngle;
	}

	

	// Bullet의 시작위치를 구하는 함수
	// fAngle과 r, 시작 위치를 넣어주면 그 시작 위치로 부터 r * angle을 계산하여 총알의 시작 위치를 구해주는 함수이다.

	POSITION GetBulletPos(POSITION tMiddlePos, float fDist, float fAngle, class CBullet* pBullet);
	POSITION GetBulletPos(POSITION tMiddlePos, float fDist, float fAngle, float addx, float addy);

	// Shield의 시작 위치를 구하는 함수

	POSITION GetShieldPos(POSITION tPlayerCenter, _SIZE pBulletSize);


	// 기본값 셋팅 후에 복사까지하고서, 해야할 애니메이션 과정

	void SetAnimation();
	void SetAnimation(const string& strTag);
	

};

