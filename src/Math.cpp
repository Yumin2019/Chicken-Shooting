
#include "Math.h"
#include "Object\CObj.h"


// 순서에 상관없이 넣어도 되도록 만든 함수.

bool Math::CollisionCheck(CObj * pSrc, CObj * pDest)
{
	// 플레이어와 총알 / 몬스터와 총알 / Skill Bullet Vs 총알 / 원 vs 원 원 vs 사각형 사각형 vs 사각형

	switch (pSrc->GetShape())
	{
	case ST_RECT:


		switch (pDest->GetShape())
		{
		case ST_RECT:
			return	RectToRect(pSrc, pDest);

		case ST_CIRCLE:
			return	CircleToRect(pDest, pSrc);

		}


	case ST_CIRCLE:


		switch (pDest->GetShape())
		{
		case ST_RECT:
			return  CircleToRect(pSrc, pDest);

		case ST_CIRCLE:
			return	CircleToCircle(pSrc, pDest);
		}

	}



	return false;
}

bool Math::CircleToCircle(CObj * pSrc, CObj * pDest)
{

	POSITION src = pSrc->GetCenter();

	float srcR = pSrc->GetSize().x / 2.f;

	POSITION dest = pDest->GetCenter();

	float destR = pDest->GetSize().x / 2.f;

	if (PointToPoint(src, dest) <= (destR + srcR))
		return true;

	return false;
}

bool Math::CircleToRect(CObj * pSrc, CObj * pDest)
{

	// src 원 dest 사각형 

	// 원의 중심
	POSITION Circle = pSrc->GetCenter();
	float l, t, r, b;

	// 사각형
	l = pDest->GetLeft();
	t = pDest->GetTop();
	r = pDest->GetRight();
	b = pDest->GetBottom();

	// 반지름
	float    CircleR = pSrc->GetSize().x / 2.f;


	// 원의 중심이 사각형의 왼쪽과 오른쪽 사이에 있거나
	// 위쪽과 아래쪽 사이에 있는 경우와 그러지 않은 경우로 나눈다

	if (Circle.y > pDest->GetTop() && Circle.y < pDest->GetBottom() ||
		Circle.x > pDest->GetLeft() && Circle.x < pDest->GetRight())
	{

		// 확장된 사각형을 구한다.
		l -= CircleR;
		t -= CircleR;
		r += CircleR;
		b += CircleR;

		// 확장된 사각형에 원의 중심이 있다면,
		// 충돌이다.

		if (Circle.x < l)
			return false;
		else if (Circle.x > r)
			return false;

		if (Circle.y < t)
			return false;
		else if (Circle.y > b)
			return false;

		return true;
	}

	POSITION RectPos[4];
	RectPos[0] = POSITION(l, t);
	RectPos[1] = POSITION(l, b);
	RectPos[2] = POSITION(r, t);
	RectPos[3] = POSITION(r, b);

	// 사각형의 4 꼭짓점이 
	// 원에 속하면 충돌

	for (size_t i = 0; i < 4; ++i)
	{
		if (PointToPoint(RectPos[i], Circle) <= CircleR)
			return true;
	}


	return false;
}

bool Math::RectToRect(CObj * pSrc, CObj * pDest)
{
	float srcl = pSrc->GetLeft();
	float srct = pSrc->GetTop();
	float srcr = pSrc->GetRight();
	float srcb = pSrc->GetBottom();


	float destl = pDest->GetLeft();
	float destt = pDest->GetTop();
	float destr = pDest->GetRight();
	float destb = pDest->GetBottom();


	if (srcl <= destr &&
		srcr >= destl &&
		srct <= destb &&
		srcb >= destt)
		return true;
	else
		return false;


	if (pSrc->GetLeft() <= pDest->GetRight() &&
		pSrc->GetRight() >= pDest->GetLeft() &&
		pSrc->GetTop() <= pDest->GetBottom() &&
		pSrc->GetBottom() >= pDest->GetTop())
		return true;
	else
		return false;
}

float Math::PointToPoint(_tagPosition tPos, _tagPosition mPos)
{
	return sqrt((tPos.x - mPos.x) * (tPos.x - mPos.x) + (tPos.y - mPos.y) * (tPos.y - mPos.y));

}

float Math::GetAngle(CObj * pSrc, CObj * pDest)
{
	// 목적지 - 이전위치를 뺸다.

	POSITION Dest = pDest->GetCenter();
	POSITION Src = pSrc->GetCenter();

	float width = Dest.x - Src.x;
	float height = Dest.y - Src.y;
	float dist = sqrt(width* width + height* height);

	float fAngle = acosf(width / dist);


	if (Dest.y < Src.y)
	{
		fAngle = PI * 2 - fAngle;
	}

	return  fAngle;
}

float Math::GetAngle(POSITION pSrc, POSITION pDest)
{
	float width = pDest.x - pSrc.x;
	float height = pDest.y - pSrc.y;

	float dist = sqrt(width* width + height* height);

	float fAngle = acosf(width / dist);


	if (pDest.y < pSrc.y)
	{
		fAngle = PI * 2 - fAngle;
	}

	return  fAngle;
}

bool Math::PointInScreen(_tagPosition tPos)
{
	if (tPos.x >= 0.f && tPos.x <= 1280.f &&
		tPos.y >= 0.f && tPos.y <= 720.f)
		return true;


	return false;
}

bool Math::PointInRect(POSITION tRectPos, _SIZE tRectSize, POSITION m_tPos)
{
	float l = tRectPos.x;
	float t = tRectPos.y;
	float r = tRectPos.x + tRectSize.x;
	float b = tRectPos.y + tRectSize.y;

	if(m_tPos.x <= r && m_tPos.x >= l &&
		m_tPos.y >= t && m_tPos.y <= b)
		return true;

	return false;
}


