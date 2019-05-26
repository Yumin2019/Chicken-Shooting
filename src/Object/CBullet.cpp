#include "CPlayer.h"
#include "../Scene/CInGameScene.h"
#include "../Core/CSceneManager.h" 
#include "../Layer/CLayer.h"
#include "../Math.h"
#include "CBulletA.h"
#include "CItem.h"
#include "CMinionA.h"
#include "CMinionB.h"

bool CBullet::m_bBossXY;

CBullet::CBullet() :
	m_fDist(0.f),
	m_eBulletType(BT_NONE),
	m_iDamage(1),
	m_bEnable(true)
{
}

CBullet::CBullet(const CBullet & bullet) :
	CObj(bullet)
{
	m_fDist = 0.f;
	m_fLimitDist = bullet.m_fLimitDist;
	m_eBulletType = bullet.m_eBulletType;
	m_iDamage = bullet.m_iDamage;
	m_bEnable = bullet.m_bEnable;
}


CBullet::~CBullet()
{
}

bool CBullet::Init()
{
	
	return true;
}

void CBullet::Input(float fDeltaTime)
{
	
}

void CBullet::Collision(float fDeltaTime)
{
	if (!GetBulletEnable())
		return;



	list<class CObj*>& MonsterList = GET_SINGLE(CSceneManager)->GetMonsterList();

	list<class CObj*>::iterator iter;
	list<class CObj*>::iterator iterEnd;

	switch (m_eBulletType)
	{
		// �÷��̾��� Bullet�� ��쿡�� Monster�迭�� �ʿ��ϴ�.  �÷��̾��� bullet�� ������ ��ǥ�� ���Ͽ� �浹ó��.
	case BT_PLAYER_NORMAL: 

		
		iterEnd = MonsterList.end();

  		for (iter = MonsterList.begin(); iter != iterEnd; ++iter) 
		{
			if ((*iter) == PLAYER)
				continue;

			if (!(*iter)->GetLife()) // ����Ʈ�� �����ٰ� ���� ���͸� �ѱ��.
				continue;

			if (Math::CollisionCheck((*iter), this))
			{
				Die();	// Bullet Die


				// ���� �ǰ�
				((CMoveObj*)*iter)->AddHP(-GetDamage());
				((CMoveObj*)*iter)->SetAttacked(true);
				


				if (!(*iter)->GetLife()) // Die
				{
					list<class CBullet*> MonsterBulletList = ((CMoveObj*)*iter)->GetBulletList(); // ���Ϳ��� Bullet List�� ������.


					BulletMove(MonsterBulletList, GET_SINGLE(CSceneManager)->GetDeadMonsterBulletList());


					CBulletA* pBullet = (CBulletA*)CreateEffect("DieEffectBullet", (*iter)->GetPos());

					GET_SINGLE(CSceneManager)->GetSecondEffectList().push_back(pBullet);

					RandomItem((*iter)->GetPos()); 

				}

				return; // ���׸� �Ѿ��� ������ ��쿡�� return.
			}

		}

		break;



	case BT_PLAYER_BOMB: // ���� �Ѿ˰� �浹�ϸ� ���� �Ѿ��� ���־� �Ѵ�. + ���͵� ���־��Ѵ�.


		iterEnd = MonsterList.end();

		for (iter = MonsterList.begin(); iter != iterEnd; ++iter) 
		{
			
			if ((*iter) == PLAYER)
				continue;

			if (!(*iter)->GetLife())
				continue;



			// Monster Bullet Vs Skill Bullet

			list<class CBullet*> MonsterBulletList = ((CMoveObj*)*iter)->GetBulletList(); // ���Ϳ��� Bullet List�� ������.

			SkillToBullet(MonsterBulletList);
			


			// Monster Vs SkillBullet

			// ��� ���Ϳ� ��ź�� �浹�ߴ��� �˻�.

			if (Math::CollisionCheck((*iter), this))
			{


				if (!(*iter)->GetCollision().m_bStart && !(*iter)->GetCollision().m_bStay)
				{
					// ó�� �浹

					(*iter)->SetCollStart(true);
				}
				else if ((*iter)->GetCollision().m_bStart && !(*iter)->GetCollision().m_bStay)
				{
					// �浹 ��
					(*iter)->SetCollStart(false);
					(*iter)->SetCollStay(true);
				}




			}
			else // �浹���� ���� ���
			{
				if ((*iter)->GetCollision().m_bStart || (*iter)->GetCollision().m_bStay)
				{
					(*iter)->SetCollStart(false);
					(*iter)->SetCollStay(false);
					(*iter)->SetCollEnd(true);



					// �������� ���� �浹 ó��.

					((CMoveObj*)*iter)->AddHP(-GetDamage()); // Skill Damage = 15
					((CMoveObj*)*iter)->SetAttacked(true);



					if (!(*iter)->GetLife())
					{
						RandomItem((*iter)->GetPos()); 
						
						BulletMove(MonsterBulletList, GET_SINGLE(CSceneManager)->GetDeadMonsterBulletList());

						GET_SINGLE(CSceneManager)->GetSecondEffectList().push_back((CBulletA*)CreateEffect("DieEffectBullet", (*iter)->GetPos()));
					}

				}
				else if ((*iter)->GetCollision().m_bEnd)
					(*iter)->SetCollEnd(false);


			}


		

		}
				// Dead ���� Bullet List

				{ 
						list<CObj*>& DeadMonsterBulletList = GET_SINGLE(CSceneManager)->GetDeadMonsterBulletList();
						list<CObj*>::iterator m_iter;
						list<CObj*>::iterator m_iterEnd = DeadMonsterBulletList.end();


						for (m_iter = DeadMonsterBulletList.begin(); m_iter != m_iterEnd; ++m_iter) // *iter = *Bullet
						{

							if (!(*m_iter)->GetLife())
								continue;

							if (Math::CollisionCheck((*m_iter), this)) // �Ѿ˰� ��ų�� �浹
							{
								(*m_iter)->Die();
							}
				
						}


				}
		break;

	case BT_PLAYER_SHIELD:

		// �÷��̾� ����� ���� �Ѿ˰� �浹�ϸ� ���� �Ѿ��� ��� ���ش�.


				iterEnd = MonsterList.end();

				for (iter = MonsterList.begin(); iter != iterEnd; ++iter) 
				{
					if ((*iter) == PLAYER)
						continue;

					if (!(*iter)->GetLife())
						continue;

					// Monster Bullet Vs Shield

					list<class CBullet*> MonsterBulletList = ((CMoveObj*)*iter)->GetBulletList(); 

					SkillToBullet(MonsterBulletList); // ���� �Ѿ˰� ���带 ���Ͽ� �浹 ó��.

				}

				// ���� ������ Bullet List�� ó�����ش�.
				{ 
					list<CObj*>& DeadMonsterBulletList = GET_SINGLE(CSceneManager)->GetDeadMonsterBulletList();
					list<CObj*>::iterator m_iter;
					list<CObj*>::iterator m_iterEnd = DeadMonsterBulletList.end();


					for (m_iter = DeadMonsterBulletList.begin(); m_iter != m_iterEnd; ++m_iter) // *iter = *Bullet
					{

						if (!(*m_iter)->GetLife() || ((CBullet*)*m_iter)->GetBulletType() == BT_EFFECT)
							continue;

						if (Math::CollisionCheck((*m_iter), this)) // �Ѿ˰� ��ų�� �浹
						{
							(*m_iter)->Die();
						}

					}
				}


		break;

				case BT_ITEM:
				{

					if (Math::CollisionCheck(PLAYER, this))
					{
						// �� �������� �÷��̾�� ȿ���� ����Ѵ�.


						switch (((CItem*)this)->GetItemType())
						{
						case IT_SHIELD:

							PLAYER->AddShieldCount();
							break;

						case IT_BOMB:

							PLAYER->AddBombCount();
							break;

						case IT_PORTION:
							PLAYER->AddHP(25);

							break;

						case IT_BLUERUBY:
							// ���ǵ� ����
							PLAYER->AddSpeedLv();
							break;

						case IT_REDRUBY:
							PLAYER->AddAttackLv();
							break;


							// ���Ͱ� ������
						case IT_BLUEMARBLE:
							break;

						case IT_PINKMARBLE:
							break;

						}

						Die();

					}

				}

				break;

	case BT_BOSS_SPECIAL2_1:
	case BT_BOSS_SPECIAL1_1:
	case BT_BOSS_SPECIAL1PREV:
	case BT_PLAYER_ATTACKPREV:
	case BT_PLAYER_MISAIL:
		break; 

		// ������ Bullet�� ��쿡�� Player�� ������ �ʿ��ϴ�.
				case BT_MONSTER:
				{

						if (Math::CollisionCheck(PLAYER, this))
						{
							Die();				  // Bullet Die


							PLAYER->PlayerAttacked();

							PLAYER->AddHP(-GetDamage());

						}
				}
					break;


					////////////////////////////////// BOSS /////////////////////////////////////////

				case BT_BOSS_SPECIAL2_2: // �ָ���.
				case BT_BOSS_NORMAL:  // �⺻ ����
				{

						if (Math::CollisionCheck(PLAYER, this))
						{
							Die();				  // Bullet Die

							PLAYER->PlayerAttacked();

							PLAYER->AddHP(-GetDamage());
						}
				}
					break;


	}
}

void CBullet::Update(float fDeltaTime)
{
	CObj::Update(fDeltaTime);


}

void CBullet::Render(HDC hDC, float fDeltaTime)
{

	CObj::Render(hDC, fDeltaTime);
	
}

CBullet * CBullet::Clone()
{
	return new CBullet(*this);
}

void CBullet::SkillToBullet(list<class CBullet*>& MonsterBulletList) // SKillToBullet�Լ��� Skill Bullet�� �������� �����ϴ� �Լ��̴�. 
{
	list<class CBullet*>::iterator iter;
	list<class CBullet*>::iterator iterEnd = MonsterBulletList.end();

	for (iter = MonsterBulletList.begin(); iter != iterEnd; ) 
	{

		if (!(*iter)->GetLife()) // �̹� �׾� �ִ� ���
		{
			++iter;
			continue;
		}

		// ��� �ִ� ��쿡�� �浹 ó���� �Ѵ�. Bullet�� Bullet��.
		if (Math::CollisionCheck((*iter), this))
		{
			// �浹�� ��쿡�� Monster Bullet�� ���δ�.
			(*iter)->Die();
		}
		else
			++iter;
		
	}
}

CObj * CBullet::GetLiveMonster(list<CObj*>& MonsterList)
{
	list<CObj*>::iterator iter;
	list<CObj*>::iterator iterEnd = MonsterList.end();

	for (iter = MonsterList.begin(); iter != iterEnd;)
	{
		if ((*iter)->GetLife())
			return (*iter);

		else
			++iter;
	}


	return NULL;
}

void CBullet::CreateMinionA()
{
	CMinionA* pMinion = (CMinionA*)CreateMonster("MinionA", GetPos());


	switch (m_bBossXY)
	{
	case true:
		pMinion->SetXY(true);

		if (GetCenterX() <= FULL_WIDTH / 2.f)
			pMinion->SetStandDir(SD_RIGHT);
		else
			pMinion->SetStandDir(SD_LEFT);

		break;

	case false:
		pMinion->SetXY(false);

		if (GetCenterY() >= FULL_HEIGHT / 2.f)
			pMinion->SetStandDir(SD_LEFT);
		else
			pMinion->SetStandDir(SD_RIGHT);

		break;
	}

	pMinion->Init();

	GET_SINGLE(CSceneManager)->GetSecondEffectList().push_back((CBulletA*)CreateEffect("FlowerDie", GetPos()));
		

	GET_SINGLE(CSceneManager)->GetMonsterList().push_front(pMinion);
}

void CBullet::CreateMinionB()
{
	CMinionA* pMinion = (CMinionA*)CreateMonster("MinionB", GetPos());

	switch (m_bBossXY)
	{
	case true:
		pMinion->SetXY(true);

		if (GetCenterX() <= FULL_WIDTH / 2.f)
			pMinion->SetStandDir(SD_RIGHT);
		else
			pMinion->SetStandDir(SD_LEFT);

		break;

	case false:
		pMinion->SetXY(false);

		if (GetCenterY() >= FULL_HEIGHT / 2.f)
			pMinion->SetStandDir(SD_LEFT);
		else
			pMinion->SetStandDir(SD_RIGHT);

		break;
	}


	pMinion->Init();

	GET_SINGLE(CSceneManager)->GetSecondEffectList().push_back((CBulletA*)CreateEffect("FlowerDie", GetPos()));

	GET_SINGLE(CSceneManager)->GetMonsterList().push_front(pMinion);
}

void CBullet::CreateFlowerDie()
{
	GET_SINGLE(CSceneManager)->GetSecondEffectList().push_back((CBulletA*)CreateEffect("FlowerDie", GetPos()));

}

void CBullet::BulletMove(list<class CBullet*>& pSrc, list<class CObj*>& pDest)
{

	list<class CBullet*>::iterator iter;
	list<class CBullet*>::iterator iterEnd = pSrc.end();

	for (iter = pSrc.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->GetLife()) // ����ִ� ��쿡��.
		{
			(*iter)->AddRef(); // ������ BulletList���� Release ���� 

			pDest.push_back((*iter));
		}

	}
}

void CBullet::RandomItem(POSITION tPos)
{

		int idx = rand() % 100 + 1; // 1 ~ 100

		if (idx <= 75) 
		{
			// ������ ����.
			idx = rand() % 100 + 1;

			CItem* pItem;

			if(idx <= 30)
				pItem = (CItem*)CreateItem("PortionItem", tPos);

			else if(idx <= 50)
				pItem = (CItem*)CreateItem("DamageItem", tPos);

			else if(idx <= 70)
				pItem = (CItem*)CreateItem("SpeedItem", tPos);

			else if(idx <= 85)
				pItem = (CItem*)CreateItem("BombItem", tPos);

			else
				pItem = (CItem*)CreateItem("ShieldItem", tPos);

			GET_SINGLE(CSceneManager)->GetFirstEffectList().push_back(pItem);
		}


}
