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
		// 플레이어의 Bullet의 경우에는 Monster배열이 필요하다.  플레이어의 bullet과 몬스터의 좌표를 비교하여 충돌처리.
	case BT_PLAYER_NORMAL: 

		
		iterEnd = MonsterList.end();

  		for (iter = MonsterList.begin(); iter != iterEnd; ++iter) 
		{
			if ((*iter) == PLAYER)
				continue;

			if (!(*iter)->GetLife()) // 리스트를 돌리다가 죽은 몬스터면 넘긴다.
				continue;

			if (Math::CollisionCheck((*iter), this))
			{
				Die();	// Bullet Die


				// 몬스터 피격
				((CMoveObj*)*iter)->AddHP(-GetDamage());
				((CMoveObj*)*iter)->SetAttacked(true);
				


				if (!(*iter)->GetLife()) // Die
				{
					list<class CBullet*> MonsterBulletList = ((CMoveObj*)*iter)->GetBulletList(); // 몬스터에서 Bullet List를 가져옴.


					BulletMove(MonsterBulletList, GET_SINGLE(CSceneManager)->GetDeadMonsterBulletList());


					CBulletA* pBullet = (CBulletA*)CreateEffect("DieEffectBullet", (*iter)->GetPos());

					GET_SINGLE(CSceneManager)->GetSecondEffectList().push_back(pBullet);

					RandomItem((*iter)->GetPos()); 

				}

				return; // 조그만 총알이 명중한 경우에는 return.
			}

		}

		break;



	case BT_PLAYER_BOMB: // 몬스터 총알과 충돌하면 몬스터 총알을 없애야 한다. + 몬스터도 없애야한다.


		iterEnd = MonsterList.end();

		for (iter = MonsterList.begin(); iter != iterEnd; ++iter) 
		{
			
			if ((*iter) == PLAYER)
				continue;

			if (!(*iter)->GetLife())
				continue;



			// Monster Bullet Vs Skill Bullet

			list<class CBullet*> MonsterBulletList = ((CMoveObj*)*iter)->GetBulletList(); // 몬스터에서 Bullet List를 가져옴.

			SkillToBullet(MonsterBulletList);
			


			// Monster Vs SkillBullet

			// 모든 몬스터와 폭탄과 충돌했는지 검사.

			if (Math::CollisionCheck((*iter), this))
			{


				if (!(*iter)->GetCollision().m_bStart && !(*iter)->GetCollision().m_bStay)
				{
					// 처음 충돌

					(*iter)->SetCollStart(true);
				}
				else if ((*iter)->GetCollision().m_bStart && !(*iter)->GetCollision().m_bStay)
				{
					// 충돌 중
					(*iter)->SetCollStart(false);
					(*iter)->SetCollStay(true);
				}




			}
			else // 충돌하지 않은 경우
			{
				if ((*iter)->GetCollision().m_bStart || (*iter)->GetCollision().m_bStay)
				{
					(*iter)->SetCollStart(false);
					(*iter)->SetCollStay(false);
					(*iter)->SetCollEnd(true);



					// 떨어지자 마자 충돌 처리.

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
				// Dead 몬스터 Bullet List

				{ 
						list<CObj*>& DeadMonsterBulletList = GET_SINGLE(CSceneManager)->GetDeadMonsterBulletList();
						list<CObj*>::iterator m_iter;
						list<CObj*>::iterator m_iterEnd = DeadMonsterBulletList.end();


						for (m_iter = DeadMonsterBulletList.begin(); m_iter != m_iterEnd; ++m_iter) // *iter = *Bullet
						{

							if (!(*m_iter)->GetLife())
								continue;

							if (Math::CollisionCheck((*m_iter), this)) // 총알과 스킬과 충돌
							{
								(*m_iter)->Die();
							}
				
						}


				}
		break;

	case BT_PLAYER_SHIELD:

		// 플레이어 쉴드는 몬스터 총알과 충돌하면 몬스터 총알을 모두 없앤다.


				iterEnd = MonsterList.end();

				for (iter = MonsterList.begin(); iter != iterEnd; ++iter) 
				{
					if ((*iter) == PLAYER)
						continue;

					if (!(*iter)->GetLife())
						continue;

					// Monster Bullet Vs Shield

					list<class CBullet*> MonsterBulletList = ((CMoveObj*)*iter)->GetBulletList(); 

					SkillToBullet(MonsterBulletList); // 몬스터 총알과 쉴드를 비교하여 충돌 처리.

				}

				// 죽은 몬스터의 Bullet List도 처리해준다.
				{ 
					list<CObj*>& DeadMonsterBulletList = GET_SINGLE(CSceneManager)->GetDeadMonsterBulletList();
					list<CObj*>::iterator m_iter;
					list<CObj*>::iterator m_iterEnd = DeadMonsterBulletList.end();


					for (m_iter = DeadMonsterBulletList.begin(); m_iter != m_iterEnd; ++m_iter) // *iter = *Bullet
					{

						if (!(*m_iter)->GetLife() || ((CBullet*)*m_iter)->GetBulletType() == BT_EFFECT)
							continue;

						if (Math::CollisionCheck((*m_iter), this)) // 총알과 스킬과 충돌
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
						// 각 아이템은 플레이어에게 효과를 줘야한다.


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
							// 스피드 증가
							PLAYER->AddSpeedLv();
							break;

						case IT_REDRUBY:
							PLAYER->AddAttackLv();
							break;


							// 몬스터계 아이템
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

		// 몬스터의 Bullet의 경우에는 Player의 정보가 필요하다.
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

				case BT_BOSS_SPECIAL2_2: // 주먹질.
				case BT_BOSS_NORMAL:  // 기본 공격
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

void CBullet::SkillToBullet(list<class CBullet*>& MonsterBulletList) // SKillToBullet함수는 Skill Bullet을 기준으로 실행하는 함수이다. 
{
	list<class CBullet*>::iterator iter;
	list<class CBullet*>::iterator iterEnd = MonsterBulletList.end();

	for (iter = MonsterBulletList.begin(); iter != iterEnd; ) 
	{

		if (!(*iter)->GetLife()) // 이미 죽어 있는 경우
		{
			++iter;
			continue;
		}

		// 살아 있는 경우에는 충돌 처리를 한다. Bullet과 Bullet을.
		if (Math::CollisionCheck((*iter), this))
		{
			// 충돌한 경우에는 Monster Bullet을 죽인다.
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
		if ((*iter)->GetLife()) // 살아있는 경우에만.
		{
			(*iter)->AddRef(); // 이전의 BulletList에서 Release 예정 

			pDest.push_back((*iter));
		}

	}
}

void CBullet::RandomItem(POSITION tPos)
{

		int idx = rand() % 100 + 1; // 1 ~ 100

		if (idx <= 75) 
		{
			// 아이템 생성.
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
