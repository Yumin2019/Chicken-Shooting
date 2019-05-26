#pragma once


class Math
{
	//Collision
public:

	// Collision 검사 함수.
	static bool CollisionCheck(class CObj* pSrc, class CObj* pDest);

	// Circle vs Circle
	static bool CircleToCircle(class CObj* pSrc, class CObj* pDest);

	// Circle Vs Rect
	static bool CircleToRect(class CObj* pSrc, class CObj* pDest);

	// Rect Vs Rect
	static bool RectToRect(class CObj* pSrc, class CObj* pDest);

	// 두 점 사이의 거리
	static float PointToPoint(struct _tagPosition tPos, struct _tagPosition mPos);

	// 두 물체의 위치정보를 바탕으로 아크코사인을 이용해서 각도(라디안)을 반환
	static float GetAngle(class CObj* pSrc, class CObj* pDest);

	// 두 물체의 위치 정보를 이용하여 구하기
	static float GetAngle(struct _tagPosition pSrc, struct _tagPosition pDest);

	// 사각형의 점
	static bool PointInScreen(struct _tagPosition tPos);


	static bool PointInRect(_tagPosition tRectPos, _tagPosition tRectSize, _tagPosition m_tPos);

};

