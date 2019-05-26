#pragma once

typedef struct _tagCollision
{
	
	// 누구와 어떤 상태.
	//class CObj* pObj;

	bool m_bStart;
	bool m_bStay;
	bool m_bEnd;

	_tagCollision() :
		m_bStart(false),
		m_bStay(false),
		m_bEnd(false)
	{}



}COLLISION, *PCOLLISION;


typedef struct _tagAnimationClip
{
	ANIMATION_OPTION eOption;
	vector<class CTexture*> vecTexture;

	float fAnimationTime;
	float fAnimationLimitTime;
	float fAnimationFrameTime;

	int iFrameX;
	int iFrameY;

	int iStartX;
	int iStartY;


	int iLengthX;
	int iLengthY;

	float fWhiteX;
	float fWhiteY;

	float fFrameSizeX;
	float fFrameSizeY;

	float fOptionTime;
	float fOptionLimitTime;

}ANIMATIONCLIP, *PANIMATIONCLIP;

typedef struct _tagPosition
{
	float x, y;

	_tagPosition() :
		x(0.f),
		y(0.f)
	{}

	_tagPosition(float x, float y) :
		x(x),
		y(y)
	{}


	// ===== =

	void operator=(const _tagPosition& tPos)
	{
		x = tPos.x;
		y = tPos.y;
	}

	

	// ===== +
	_tagPosition operator+(float a) const
	{
		_tagPosition tPos;
		tPos.x = a + x;
		tPos.y = a + y;
		return tPos;
	}

	_tagPosition operator+(const _tagPosition& tPos) const
	{
		_tagPosition m_tPos;
		m_tPos.x = tPos.x + x;
		m_tPos.y = tPos.y + y;
		return m_tPos;
	}

	void operator+=(const _tagPosition& tPos)
	{
		x += tPos.x;
		y += tPos.y;
	}

	// 소름; 

	// + 기준으로 순서가 상관이 없는데 빼거나 곱할 때는 순서상 문제가 생겨서, 여기서 잘못된 값을 구함.


	// ===== -
	_tagPosition operator-(float a) const
	{
		_tagPosition tPos;
		tPos.x = x - a;
		tPos.y = y - a;
		return tPos;
	}

	_tagPosition operator-(const _tagPosition& tPos) const
	{
		_tagPosition m_tPos;
		m_tPos.x = x - tPos.x;
		m_tPos.y = y - tPos.y;
		return m_tPos;
	}

	void operator-=(const _tagPosition& tPos)
	{
		x -= tPos.x;
		y -= tPos.y;
	}


	// ===== *
	_tagPosition operator*(float a) const
	{
		_tagPosition tPos;
		tPos.x = x * a;
		tPos.y = y * a;
		return tPos;
	}

	_tagPosition operator*(const _tagPosition& tPos) const
	{
		_tagPosition m_tPos;
		m_tPos.x = x * tPos.x;
		m_tPos.y = y * tPos.y;
		return m_tPos;
	}

	void operator*=(const _tagPosition& tPos)
	{
		x *= tPos.x;
		y *= tPos.y;
	}

	
}POSITION, *PPOSITION, _SIZE, *_PSIZE;

typedef struct _tagResolution
{
	int iWidth;
	int iHeight;

}RESOLUTION, *PRESOLUTION;

