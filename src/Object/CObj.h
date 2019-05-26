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

	int					m_iRef; // ������Ʈ���� ref.
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

	// ������Ʈ �߰�
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
	// ���� ���� �Լ�

	void AddAngle(float f)
	{
		m_fAngle += f;
	}

	// ���ǵ� ���� �Լ�

	void AddSpeed(float f)
	{
		m_fSpeed += f;
	}


	// Move : ������ �ӷ����� ���⼺�� ������.

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

	// Move : ������ �ӷ��� ������ ������ �̿��ؼ�

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


	// map ������Ÿ��
	void SetTexture(class CTexture* pTexture)
	{
		m_pTexture = pTexture;
	}


	
public:

	 CObj* CreateMonster(const string& strTag, POSITION tPos);

	 CObj* CreateEffect(const string& strTag, POSITION tPos);

	 CObj* CreateItem(const string& strTag, POSITION tPos);
	
	 void CreateSpecialAttack2(float y);

	// �ִϸ��̼� ����

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


	// �츮�� ������ ������ ����� �ϰ� �ֱ� ������, lefttop�� ����ָ� �ȴ�.

	// Left Top 

	void SetLeftTop(float x, float y) // ����.
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



	// �ٶ󺸴� ������ ���� Angle�� �������ִ� �Լ��̴�.

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

	

	// Bullet�� ������ġ�� ���ϴ� �Լ�
	// fAngle�� r, ���� ��ġ�� �־��ָ� �� ���� ��ġ�� ���� r * angle�� ����Ͽ� �Ѿ��� ���� ��ġ�� �����ִ� �Լ��̴�.

	POSITION GetBulletPos(POSITION tMiddlePos, float fDist, float fAngle, class CBullet* pBullet);
	POSITION GetBulletPos(POSITION tMiddlePos, float fDist, float fAngle, float addx, float addy);

	// Shield�� ���� ��ġ�� ���ϴ� �Լ�

	POSITION GetShieldPos(POSITION tPlayerCenter, _SIZE pBulletSize);


	// �⺻�� ���� �Ŀ� ��������ϰ�, �ؾ��� �ִϸ��̼� ����

	void SetAnimation();
	void SetAnimation(const string& strTag);
	

};

