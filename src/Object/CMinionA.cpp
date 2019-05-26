#include "CMinionA.h"
#include "CBulletA.h"
#include "../Core/CCreateManager.h"
#include "../Animation/CAnimation.h"
#include "../Math.h"
#include "../Scene/CInGameScene.h"
#include "CPlayer.h"



CMinionA::CMinionA() :
	m_fBulletTime2(0.f),
	m_tTime(0.f)
{
}

CMinionA::CMinionA(const CMinionA & mon) :
	CMonster(mon)
{

	m_fBulletLimitTime2 = mon.m_fBulletLimitTime2;
	m_fBulletTime2 = 0.f;

	m_tTime = 0.f;
}


CMinionA::~CMinionA()
{
	// Monster���� bulletlist�Ҹ�
}

bool CMinionA::Init()
{

	switch (m_bXY)
	{
	case true:

		switch (m_eDir)
		{
		case SD_RIGHT:
			m_pAnimation->ChangeClip("MinionARight");
			m_pAnimation->SetDefaultClip("MinionARight");
			break;

		case SD_LEFT:

			m_pAnimation->ChangeClip("MinionALeft");
			m_pAnimation->SetDefaultClip("MinionALeft");
			break;
		}
		break;

	case false:

		switch (PLAYER->GetCenterX() >= GetCenterX())
		{
		case true:
			m_pAnimation->ChangeClip("MinionARight");
			m_pAnimation->SetDefaultClip("MinionARight");
			break;

		case false:

			m_pAnimation->ChangeClip("MinionALeft");
			m_pAnimation->SetDefaultClip("MinionALeft");
			break;
		}

		break;
	}



	return true;
}

void CMinionA::Input(float fDeltaTime)
{
	CMonster::Input(fDeltaTime);


}

void CMinionA::Collision(float fDeltaTime)
{
	CMonster::Collision(fDeltaTime);
	
}

void CMinionA::Update(float fDeltaTime)
{
	


	// ���Ͱ� ���� ������ ��� 
	if (GetAttacked())
	{

		// m_bXY = true
		// �̴Ͼ��� ��쿡�� �⺻ ����� 2���� �־ 
		// �������� �����ϴ� ��쿡 �ٲ㼭 ������ �ؾ� �Ѵ�.


		switch (m_bXY)
		{
			case true: // x

				switch (m_eDir)
				{
				case SD_RIGHT:
					m_pAnimation->ChangeClip("MinionARightAttacked");

					break;
				case SD_LEFT:
					m_pAnimation->ChangeClip("MinionALeftAttacked");

					break;
				}

			break;

		case false: // y


				switch (PLAYER->GetCenterX() >= GetCenterX())
				{
				case true:
					m_pAnimation->ChangeClip("MinionARightAttacked");

					break;
				case false:
					m_pAnimation->ChangeClip("MinionALeftAttacked");

					break;
				}

				m_tTime = 0.f;


				break;

		}
	}


	CMonster::Update(fDeltaTime);



	// Minion Move


	switch(m_bXY)
	{
	case true:
		MoveX(fDeltaTime, m_eDir);


		if (GetLeft() < 0)
		{
			m_tPos.x = 0.f;
			m_eDir = SD_RIGHT;

			// �����ʸ������ ����
			m_pAnimation->ChangeClip("MinionARight");
			m_pAnimation->SetDefaultClip("MinionARight");
		}


		if (GetRight() > FULL_WIDTH)
		{
			m_tPos.x = FULL_WIDTH - GetSize().x;
			m_eDir = SD_LEFT;


			m_pAnimation->ChangeClip("MinionALeft");
			m_pAnimation->SetDefaultClip("MinionALeft");
		}

		break;

	case false:

		m_tTime += fDeltaTime;

		// 0.5�� �������� ��Ȳ�� ���� Change (���� ���̸� �� �ٲٰ�����.)


		if (m_tTime >= 0.5f)
		{
			m_tTime -= 0.5f;

			switch (PLAYER->GetCenterX() >= GetCenterX())
			{
			case true:
				m_pAnimation->ChangeClip("MinionARight");
				m_pAnimation->SetDefaultClip("MinionARight");
				break;

			case false:

				m_pAnimation->ChangeClip("MinionALeft");
				m_pAnimation->SetDefaultClip("MinionALeft");
				break;
			}
		}

		MoveY(fDeltaTime, m_eDir);

		if (GetTop() < 0.f)
		{
			m_tPos.y = 0.f;
			m_eDir = SD_RIGHT;

		}


		if (GetBottom() > FULL_HEIGHT)
		{
			m_tPos.y = FULL_HEIGHT - GetSize().y;
			m_eDir = SD_LEFT;

		}



		break;
	}
	


	m_fBulletTime += fDeltaTime;
	m_fBulletTime2 += fDeltaTime;

	if (m_fBulletTime >= m_fBulletLimitTime) // 1St Attack
	{
		m_fBulletTime -= m_fBulletLimitTime;

		// �÷��̾��� ��ġ�� �������� ��´�.

		float fAngleSize = PI / 18.f * 1.8f; 

		// fAngleSize�� ���� ��ġ�̴�. 

		for (int i = 0; i < 5; ++i)
		{


			float fAngle = Math::GetAngle(this, PLAYER); 

			CBulletA* pBullet = FIND(CBulletA)->FindObj("MinionABullet")->Clone();

			fAngle += i * fAngleSize - fAngleSize * 2.f;;

			pBullet->SetAngle(fAngle); 

			// ���� ��ġ ���ϱ�
			POSITION m_tPos = GetBulletPos(GetCenter(), GetAdjustDist(), fAngle, pBullet);

			pBullet->SetPos(m_tPos); // Pos 

			pBullet->SetBulletType(BT_MONSTER);

			m_BulletList.push_back(pBullet);
		}
	}

	//////////////////////////////////////// 

	if (m_fBulletTime2 >= m_fBulletLimitTime2) // 2St Attack  
	{
		m_fBulletTime2 -= m_fBulletLimitTime2;

		float fAngleSize = PI / 18.f * 0.9f; 
											

		for (int i = 0; i < 5; ++i)
		{

			float fAngle = Math::GetAngle(this, PLAYER); // �����̴� �÷��̾ ��������.

			CBulletA* pBullet = FIND(CBulletA)->FindObj("MinionABullet")->Clone(); 


			   // 5�߾� ��µ�. ���� 2�� �Ʒ� 2�� ��� 1��.

			   // 2�� ��ŭ + �� ��ġ���� �����Ͽ� ������ �ȴ�.


			fAngle += i * fAngleSize - fAngleSize * 2.f;;

			pBullet->SetAngle(fAngle);


			// ���� ��ġ ���ϱ�

			POSITION m_tPos = GetBulletPos(GetCenter(), GetAdjustDist(), fAngle, pBullet);


			pBullet->SetPos(m_tPos); // Pos 

			pBullet->SetBulletType(BT_MONSTER);

			m_BulletList.push_back(pBullet);
		}
	}

	
	
}

void CMinionA::Render(HDC hDC, float fDeltaTime)
{
	CMonster::Render(hDC, fDeltaTime);

}

CMinionA * CMinionA::Clone()
{
	return new CMinionA(*this);
}
