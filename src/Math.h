#pragma once


class Math
{
	//Collision
public:

	// Collision �˻� �Լ�.
	static bool CollisionCheck(class CObj* pSrc, class CObj* pDest);

	// Circle vs Circle
	static bool CircleToCircle(class CObj* pSrc, class CObj* pDest);

	// Circle Vs Rect
	static bool CircleToRect(class CObj* pSrc, class CObj* pDest);

	// Rect Vs Rect
	static bool RectToRect(class CObj* pSrc, class CObj* pDest);

	// �� �� ������ �Ÿ�
	static float PointToPoint(struct _tagPosition tPos, struct _tagPosition mPos);

	// �� ��ü�� ��ġ������ �������� ��ũ�ڻ����� �̿��ؼ� ����(����)�� ��ȯ
	static float GetAngle(class CObj* pSrc, class CObj* pDest);

	// �� ��ü�� ��ġ ������ �̿��Ͽ� ���ϱ�
	static float GetAngle(struct _tagPosition pSrc, struct _tagPosition pDest);

	// �簢���� ��
	static bool PointInScreen(struct _tagPosition tPos);


	static bool PointInRect(_tagPosition tRectPos, _tagPosition tRectSize, _tagPosition m_tPos);

};

