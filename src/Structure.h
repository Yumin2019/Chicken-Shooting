#pragma once

typedef struct _tagCollision
{
	
	// ������ � ����.
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

	// �Ҹ�; 

	// + �������� ������ ����� ���µ� ���ų� ���� ���� ������ ������ ���ܼ�, ���⼭ �߸��� ���� ����.


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

