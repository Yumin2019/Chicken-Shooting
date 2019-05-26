#include "Object\CBulletA.h"
#include "Object\CSkillBullet.h"
#include "Object\CMinionA.h"
#include "Texture\CTexture.h"
#include "Animation\CAnimation.h"
#include "Core\CCreateManager.h"
#include "Object\CMinionB.h"
#include "Object/CBoss.h"
#include "Object/CItem.h"

DEFINITION_SINGLE(CCreateManager);

CCreateManager::CCreateManager()
{

}


CCreateManager::~CCreateManager()
{
	Safe_Release_Map(m_mapObj);
}

bool CCreateManager::Init(HINSTANCE hInst, HDC hDC)
{

	m_hInst = hInst;
	m_hDC = hDC;


	// 프로토타입을 정의 하는 함수이다.



	// MinionA

	MinionACreate();
	
	// MinionB

	MinionBCreate();

	// Boss

	BossCreate();


	// 총알 계열

	// Boss Attack
	BossAttackBullet();


	// 플레이어 피격 이펙트 CBulletA

	CBulletA* pPlayerAttackedEffect = new CBulletA;

	pPlayerAttackedEffect->SetEffect();
	pPlayerAttackedEffect->SetBulletType(BT_EFFECT_PLAYER);

	pPlayerAttackedEffect->SetLeftTop(-55.f, -50.f);
	pPlayerAttackedEffect->SetRenderSize(128.f, 128.f);

	CAnimation* pPlayerAttackedEffectAni = pPlayerAttackedEffect->CreateAnimation("PlayerAttacked");

	pPlayerAttackedEffectAni->AddClip(L"Player/PlayerAttacked.bmp", "PlayerAttacked", AO_ONCE_DESTROY,
		0.5f, 0, 0, 6, 1, 0.f, 0.f, 0.f, 128.f, 128.f, 255, 0, 255);

	pPlayerAttackedEffectAni->SetDefaultClip("PlayerAttacked");


	m_mapObj.insert(make_pair("PlayerAttacked", pPlayerAttackedEffect));



	// Shield 사용 이펙트

	CBulletA* pShieldEffect = new CBulletA;

	pShieldEffect->SetEffect();
	pShieldEffect->SetBulletType(BT_EFFECT_PLAYER);


	CAnimation* pShieldAni = pShieldEffect->CreateAnimation("ShieldEffect");

	pShieldAni->AddClip(L"Player/ShieldEffect.bmp", "ShieldEffect", AO_TIME_DESTROY, 1.f, 0, 0, 4, 1, 6.f, 0.f, 0.f,
		200, 200.f, 255, 255, 255);

	pShieldAni->SetDefaultClip("ShieldEffect");

	SetRender(POSITION(-105.f, -160.f), _SIZE(200.f, 200.f), pShieldEffect);

	m_mapObj.insert(make_pair("ShieldEffect", pShieldEffect));


	// 몬스터 사망 이펙트

	CBulletA* pDieEffect = new CBulletA;

	pDieEffect->SetEffect();

	CAnimation* pDieEffectAni = pDieEffect->CreateAnimation("DieEffect");

	pDieEffectAni->AddClip(L"Monster/DieEffect.bmp", "DieEffect", AO_ONCE_DESTROY,
		0.5f, 0, 0, 6, 1, 0.f, 0.f, 0.f, 50.f, 50.f, 255, 0, 255);

	pDieEffectAni->SetDefaultClip("DieEffect");

	pDieEffect->SetLeftTop(0.f, 0.f);
	pDieEffect->SetRenderSize(50.f, 50.f);

	m_mapObj.insert(make_pair("DieEffectBullet", pDieEffect));


	// Item

	CItem* aItem = CreateItem(_SIZE(40.f, 40.f), 0.f, 0.f, ST_CIRCLE, 0, 9.f, IT_SHIELD);

	CreateTexture(L"Item/Shield.bmp", 0, 0, 0, aItem);

	SetRender(POSITION(-10.f, -10.f), _SIZE(60.f, 60.f), aItem);

	aItem->SetBulletType(BT_ITEM);

	aItem->SetBlendLimitTime(4.f);

	m_mapObj.insert(make_pair("ShieldItem", aItem));


	CItem* bItem = CreateItem(_SIZE(40.f, 25.f), 0.f, 0.f, ST_RECT, 0, 9.f, IT_BOMB);

	CreateTexture(L"Item/Bomb.bmp", 0, 0, 0, bItem);

	SetRender(POSITION(-10.f, -20.f), _SIZE(60.f, 60.f), bItem);

	bItem->SetBulletType(BT_ITEM);
	bItem->SetBlendLimitTime(4.f);


	m_mapObj.insert(make_pair("BombItem", bItem));


	CItem* cItem = CreateItem(_SIZE(40.f, 40.f), 0.f, 0.f, ST_CIRCLE, 0, 9.f, IT_PORTION);

	CreateTexture(L"Item/Portion.bmp", 0, 0, 0, cItem);

	SetRender(POSITION(-10.f, -10.f), _SIZE(60.f, 60.f), cItem);

	cItem->SetBulletType(BT_ITEM);
	cItem->SetBlendLimitTime(4.f);

	m_mapObj.insert(make_pair("PortionItem", cItem));



	CItem* dItem = CreateItem(_SIZE(38.f, 38.f), 0.f, 0.f, ST_CIRCLE, 0, 9.f, IT_BLUERUBY);

	CreateTexture(L"Item/BlueRuby.bmp", 255, 255, 255, dItem);

	SetRender(POSITION(-10.f, -10.f), _SIZE(60.f, 60.f), dItem);

	dItem->SetBulletType(BT_ITEM);
	dItem->SetBlendLimitTime(4.f);

	m_mapObj.insert(make_pair("SpeedItem", dItem));



	CItem* eItem = CreateItem(_SIZE(38.f, 38.f), 0.f, 0.f, ST_CIRCLE, 0, 9.f, IT_REDRUBY);

	CreateTexture(L"Item/RedRuby.bmp", 255, 255, 255, eItem);

	SetRender(POSITION(-10.f, -10.f), _SIZE(60.f, 60.f), eItem);

	eItem->SetBulletType(BT_ITEM);
	eItem->SetBlendLimitTime(4.f);

	m_mapObj.insert(make_pair("DamageItem", eItem));


	// MinionA Bullet

	CBulletA* pBullet = new CBulletA;

	pBullet->SetSize(23.f, 23.f); // 23 23

	pBullet->SetSpeed(350.f);

	pBullet->SetLimitDist(430.f);

	pBullet->SetShape(ST_CIRCLE);

	pBullet->SetDamage(3);

	pBullet->SetTexture(CTexture::SetTexture(L"Bullet/MinionABullet.bmp", 0, 0, 0));

	pBullet->SetLeftTop(0.f, 0.f);
	pBullet->SetRenderSize(23.f, 24.f);

	m_mapObj.insert(make_pair("MinionABullet", pBullet));



	// MinionB prev

	CSkillBullet* pBulletB = new CSkillBullet;

	pBulletB->SetSize(20.f, 20.f);  // 20 20

	pBulletB->SetSpeed(150.f);

	pBulletB->SetLimitDist(0.f);

	pBulletB->SetDamage(7);

	pBulletB->SetLifeLimitTime(5.1f);

	pBulletB->SetAngleLimitTime(1.6f);

	pBulletB->SetShape(ST_CIRCLE);


	pBulletB->SetTexture(CTexture::SetTexture(L"Bullet/MinionBBullet.bmp", 255, 0, 255));

	pBulletB->SetLeftTop(-2.f, -2.f);

	pBulletB->SetRenderSize(24.f, 24.f);

	m_mapObj.insert(make_pair("MinionBBulletPrev", pBulletB));


	// MinionB Bullet

	CBulletA* pMinionBBullet = new CBulletA;

	pMinionBBullet->SetSize(20.f, 20.f);

	pMinionBBullet->SetSpeed(400.f);

	pMinionBBullet->SetLimitDist(550.f);

	pMinionBBullet->SetDamage(3);

	pMinionBBullet->SetShape(ST_CIRCLE);

	pMinionBBullet->SetTexture(CTexture::SetTexture(L"Bullet/MinionBBullet.bmp", 255, 0, 255));

	pMinionBBullet->SetLeftTop(-2.f, -2.f);

	pMinionBBullet->SetRenderSize(24.f, 24.f);

	m_mapObj.insert(make_pair("MinionBBullet", pMinionBBullet));





	// 플레이어 공격 시작 전 도는 Bullet (Egg)

	CSkillBullet* pAttackPrev = new CSkillBullet;

	pAttackPrev->SetSize(12.f, 15.f);

	pAttackPrev->SetSpeed(350.f);

	pAttackPrev->SetLimitDist(0.f);

	pAttackPrev->SetShape(ST_RECT);

	pAttackPrev->SetDamage(0);

	pAttackPrev->SetLifeLimitTime(5.f); // 최대로 도는 시간은 5초

	CreateTexture(L"Bullet/Egg30.bmp", 255, 255, 255, pAttackPrev);

	SetRender(POSITION(-9.f, -9.f), _SIZE(30.f, 30.f), pAttackPrev);

	m_mapObj.insert(make_pair("AttackPrev", pAttackPrev));


	// 공격

	CSkillBullet* pAttack = new CSkillBullet;

	pAttack->SetSize(12.f, 15.f);

	pAttack->SetSpeed(370.f);

	pAttack->SetLimitDist(0.f);

	pAttack->SetShape(ST_RECT);

	pAttack->SetDamage(1);

	pAttack->SetLifeLimitTime(1.3f);


	CreateTexture(L"Bullet/Egg30.bmp", 255, 255, 255, pAttack);

	SetRender(POSITION(-9.f, -9.f), _SIZE(30.f, 30.f), pAttack);

	m_mapObj.insert(make_pair("Attack", pAttack));



	// 미사일 총알

	CSkillBullet* pMisail = new CSkillBullet;

	pMisail->SetSize(15.f, 15.f); 

	pMisail->SetSpeed(520.f);

	pMisail->SetLimitDist(0.f);

	pMisail->SetShape(ST_RECT);

	pMisail->SetDamage(2);

	pMisail->SetLifeLimitTime(1.f); 


	CreateTexture(L"Bullet/Misail.bmp", 0, 0, 0, pMisail);


	SetRender(POSITION(-7.f,  -2.f), _SIZE(30.f, 25.f), pMisail);


	m_mapObj.insert(make_pair("Misail", pMisail));



	// 미사일 발사전 EggBread

	CSkillBullet* pMisailPrev = CreateSkillBullet(_SIZE(0.f, 0.f), 300.f, 0.f, ST_CIRCLE, 0, 2.f);

	CreateTexture(L"Bullet/Egg.bmp", 0, 0, 0, pMisailPrev);


	SetRender(POSITION(-13.f, -11.f), _SIZE(50.f, 50.f), pMisailPrev);


	m_mapObj.insert(make_pair("MisailPrev", pMisailPrev));


	// Shield

	CSkillBullet* pShield = CreateSkillBullet(_SIZE(436.f, 436.f), 0.f, 0.f, 
		ST_CIRCLE, 0, 6.f);

	SetRender(POSITION(-32.f, -28.f), _SIZE(500.f, 500.f), pShield);

	CreateTexture(L"Bullet/Shield.bmp", 255, 255, 255, pShield);

	m_mapObj.insert(make_pair("Shield", pShield));


	// Bomb

	CSkillBullet* sBullet = CreateSkillBullet(_SIZE(45.f, FULL_HEIGHT), 300.f, 0.f, ST_RECT, 15, 1.6f); // 45

	SetRender(POSITION( -40.f, 0.f), _SIZE(155.f, 1080.f), sBullet);

	CAnimation* pBombAni = sBullet->CreateAnimation("Bomb");

	pBombAni->AddClip(L"Bullet/Bomb.bmp", "Bomb", AO_LOOP,
		1.6f, 0, 0, 6, 1, 0.f, 30.f, 0.f, 155.f, 1080.f, 255, 0, 255);

	pBombAni->SetDefaultClip("Bomb");

	m_mapObj.insert(make_pair("Bomb", sBullet));


	return true;
}

CObj * CCreateManager::FindObj(const string & strKey)
{
	unordered_map<string, CObj*>::iterator iter = m_mapObj.find(strKey);

	if (iter == m_mapObj.end())
		return NULL;

	return iter->second; 
}

void CCreateManager::MinionACreate()
{
	CMinionA* pMinion = new CMinionA;

	pMinion->SetSize(15.f, 40.f); // 충돌 영역.
	pMinion->SetSpeed(200.f);
	pMinion->SetBulletTime(1.5f);
	pMinion->SetAdjustDist(20.f);
	pMinion->SetShape(ST_RECT);

	pMinion->SetBulletTime(1.0f);
	pMinion->SetBulletTime2(1.101f);
	pMinion->SetHP(15);

	pMinion->SetLeftTop(-40.f, -35.f);
	pMinion->SetRenderSize(100.f, 100.f);

	//애니메이션 tag
	CAnimation* pMinionAnimation = pMinion->CreateAnimation("MinionA");


	// Right Move
	pMinionAnimation->AddClip(L"Monster/MinionA.bmp", "MinionARight", AO_LOOP,
		2.f, 0, 0, 6, 1, 0.f, 10.f, 10.f, 128.f, 128.f,
		255, 0, 255);

	// Left Move
	pMinionAnimation->AddClip(L"Monster/MinionA.bmp", "MinionALeft", AO_LOOP,
		2.f, 0, 1, 6, 1, 0.f, 15.f, 10.f, 128.f, 128.f,
		255, 0, 255);

	// Right Attacked (바라보는 방향 기준)
	pMinionAnimation->AddClip(L"Monster/MinionA.bmp", "MinionARightAttacked", AO_ONCE_RETURN,
		0.5f, 6, 0, 2, 1, 0.5f, 10.f, 10.f, 128.f, 128.f,
		255, 0, 255);

	// Left Attacked
	pMinionAnimation->AddClip(L"Monster/MinionA.bmp", "MinionALeftAttacked", AO_ONCE_RETURN,
		0.5f, 6, 1, 2, 1, 0.5f, 15.f, 10.f, 128.f, 128.f,
		255, 0, 255);


	pMinionAnimation->SetDefaultClip("MinionARight");

	m_mapObj.insert(make_pair("MinionA", pMinion));
}

void CCreateManager::MinionBCreate()
{
	CMinionB* pMinionB = new CMinionB;

	pMinionB->SetSize(15.f, 40.f); // 충돌 영역.
	pMinionB->SetSpeed(200.f);
	pMinionB->SetBulletTime(3.5f);
	pMinionB->SetAdjustDist(20.f);
	pMinionB->SetShape(ST_RECT);

	pMinionB->SetHP(20); 
	pMinionB->SetLeftTop(-40.f, -35.f);
	pMinionB->SetRenderSize(100.f, 100.f);



	//애니메이션 tag
	CAnimation* pMinionBAnimation = pMinionB->CreateAnimation("MinionB");

	// Right Move
	pMinionBAnimation->AddClip(L"Monster/MinionB.bmp", "MinionBRight", AO_LOOP,
		2.f, 0, 0, 6, 1, 0.f, 10.f, 10.f, 128.f, 128.f,
		255, 0, 255);

	// Left Move
	pMinionBAnimation->AddClip(L"Monster/MinionB.bmp", "MinionBLeft", AO_LOOP,
		2.f, 0, 1, 6, 1, 0.f, 15.f, 10.f, 128.f, 128.f,
		255, 0, 255);

	// Right Attacked (바라보는 방향 기준)
	pMinionBAnimation->AddClip(L"Monster/MinionB.bmp", "MinionBRightAttacked", AO_ONCE_RETURN,
		0.5f, 6, 0, 2, 1, 0.5f, 10.f, 10.f, 128.f, 128.f,
		255, 0, 255);

	// Left Attacked
	pMinionBAnimation->AddClip(L"Monster/MinionB.bmp", "MinionBLeftAttacked", AO_ONCE_RETURN,
		0.5f, 6, 1, 2, 1, 0.5f, 15.f, 10.f, 128.f, 128.f,
		255, 0, 255);


	pMinionBAnimation->SetDefaultClip("MinionBLeft");



	m_mapObj.insert(make_pair("MinionB", pMinionB));
}

void CCreateManager::BossCreate()
{
	CBoss* pBoss = new CBoss;

	if (!pBoss->Init())
	{
		SAFE_RELEASE(pBoss);
		return;
	}
	
		pBoss->SetLeftTop(-40.f, -43.f);
		pBoss->SetRenderSize(96.f, 75.f);
		pBoss->SetSize(17.f, 17.f);


	CAnimation* pAnimation = pBoss->CreateAnimation("Boss");

	// Attack
	pAnimation->AddClip(L"Boss/Attack/BossLeftAttack.bmp", "BossLeftAttack", AO_ONCE_RETURN,
		1.f, 0, 0, 5, 1, 0.f, 0.f, 0.f, 96.f, 96.f, 255, 255, 255);


	pAnimation->AddClip(L"Boss/Attack/BossRightAttack.bmp", "BossRightAttack", AO_ONCE_RETURN,
		1.f, 0, 0, 5, 1, 0.f, 0.f, 0.f, 96.f, 96.f, 255, 255, 255);


	// Attacked
	pAnimation->AddClip(L"Boss/Attacked/BossLeftAttacked.bmp", "BossLeftAttacked", AO_ONCE_RETURN,
		1.f, 0, 0, 5, 1, 0.f, 0.f, 0.f, 96.f, 96.f, 255, 255, 255);


	pAnimation->AddClip(L"Boss/Attacked/BossRightAttacked.bmp", "BossRightAttacked", AO_ONCE_RETURN,
		1.f, 0, 0, 5, 1, 0.f, 0.f, 0.f, 96.f, 96.f, 255, 255, 255);


	// Move
	pAnimation->AddClip(L"Boss/Move/BossLeft.bmp", "BossLeft", AO_LOOP,
		1.f, 0, 0, 5, 1, 0.f, 0.f, 0.f, 96.f, 96.f, 255, 255, 255);


	pAnimation->AddClip(L"Boss/Move/BossRight.bmp", "BossRight", AO_LOOP,
		1.f, 0, 0, 5, 1, 0.f, 0.f, 0.f, 96.f, 96.f, 255, 255, 255);

	
	pAnimation->AddClip(L"Boss/Move/BossUp.bmp", "BossUp", AO_LOOP,
		1.f, 0, 0, 5, 1, 0.f, 0.f, 0.f, 96.f, 96.f, 255, 255, 255);


	pAnimation->AddClip(L"Boss/Move/BossDown.bmp", "BossDown", AO_LOOP,
		1.f, 0, 0, 5, 1, 0.f, 0.f, 0.f, 96.f, 96.f, 255, 255, 255);

	// Die
	pAnimation->AddClip(L"Boss/BossDie.bmp", "BossDie", AO_ONCE_DESTROY,
		1.f, 0, 0, 7, 1, 0.f, 0.f, 0.f, 96.f, 96.f, 255, 255, 255);

	pAnimation->SetDefaultClip("BossRight");

	m_mapObj.insert(make_pair("Boss", pBoss));
}

void CCreateManager::BossAttackBullet()
{

	// 패시브 : 체력 회복

	CBulletA* pBossPassive = CreateBulletA(_SIZE(0.f, 0.f), 40.f, 50.f, ST_CIRCLE, 0);

	CreateTexture(L"Boss/Bullet/BossPassive.bmp", 255, 255, 255, pBossPassive);

	SetRender(POSITION(0.f, 0.f), _SIZE(22.f, 22.f), pBossPassive);

	m_mapObj.insert(make_pair("BossPassive", pBossPassive));


	// 기본 공격 

	CSkillBullet* pBossNormalBullet = CreateSkillBullet(_SIZE(23.f,15.f), 250.f, 0.f, ST_RECT, 12, 7.f);

	CreateTexture(L"Boss/Bullet/BossNormalBullet.bmp", 255, 255, 255, pBossNormalBullet);

	SetRender(POSITION(-4.f, -6.f), _SIZE(31.f, 24.f), pBossNormalBullet);

	m_mapObj.insert(make_pair("BossNormalBullet", pBossNormalBullet));


	// 기본 공격에서 날라가는 작은 탄환

	CBulletA* pBossNormalBullet2 = CreateBulletA(_SIZE(11.f, 11.f), 450.f, 1200.f, ST_RECT, 6);

	CreateTexture(L"Boss/Bullet/BossNormalBullet2.bmp", 255, 255, 255, pBossNormalBullet2);

	SetRender(POSITION(-3.f, -3.f), _SIZE(17.f, 17.f), pBossNormalBullet2);

	m_mapObj.insert(make_pair("BossNormalBullet2", pBossNormalBullet2));



	// 스페셜 공격 1 : 처음으로 날라가는 꽃

	CSkillBullet* pBossSpecialAttack1_1 = CreateSkillBullet(_SIZE(0.f, 0.f), 240.f, 0.f, ST_RECT, 0, 7.f); // . 2s 에 한번씩  but 횟수 제한이 걸림.

	pBossSpecialAttack1_1->SetAngleLimitTime(2.f);

	CreateTexture(L"Boss/Bullet/BossSpecialAttack1.bmp", 255, 255, 255, pBossSpecialAttack1_1);
	
	SetRender(POSITION(-9.f, -11.f), _SIZE(49.f, 49.f), pBossSpecialAttack1_1);

	m_mapObj.insert(make_pair("BossSpecialAttack1_1", pBossSpecialAttack1_1));



	// 스페셜 공격 1 : 사라지는 구슬 : 플레이어가 먹으면 false 먹지 않으면 true 
	// 미니언의 활성화에 대한 것.
	CItem* BlueMarble = CreateItem(_SIZE(40.f, 40.f), 0.f, 0.f, ST_CIRCLE, 0, 5.f, IT_BLUEMARBLE); 

	CreateTexture(L"Boss/Bullet/BlueMarble.bmp", 0, 0, 0, BlueMarble);

	SetRender(POSITION(-5.f, -5.f), _SIZE(50.f, 50.f), BlueMarble);

	BlueMarble->SetBulletType(BT_ITEM);
	BlueMarble->SetBlendLimitTime(4.f);

	m_mapObj.insert(make_pair("BlueMarble", BlueMarble));


	CItem* PinkMarble = CreateItem(_SIZE(40.f, 40.f), 0.f, 0.f, ST_CIRCLE, 0, 5.f, IT_PINKMARBLE);

	CreateTexture(L"Boss/Bullet/PinkMarble.bmp", 0, 0, 0, PinkMarble);

	SetRender(POSITION(-5.f, -5.f), _SIZE(50.f, 50.f), PinkMarble);

	PinkMarble->SetBulletType(BT_ITEM);
	PinkMarble->SetBlendLimitTime(4.f);

	m_mapObj.insert(make_pair("PinkMarble", PinkMarble));




	// 스페셜 공격 1 : 날라가는 꽃의 잔상의 죽음 Effect

	CBulletA* pBossSpecialAttack1_3 = new CBulletA;

	pBossSpecialAttack1_3->SetEffect();

	CAnimation* pFlowerDie = pBossSpecialAttack1_3->CreateAnimation("FlowerDie");

	pFlowerDie->AddClip(L"Boss/Bullet/FlowerDie.bmp", "FlowerDie", AO_TIME_DESTROY, 1.f, 0, 0, 9, 1, 1.f, 0.f, 0.f,
		105.f, 96.f, 255, 0, 255);

	pFlowerDie->SetDefaultClip("FlowerDie");

	SetRender(POSITION(5.f, -40.f), _SIZE(105.f, 96.f), pBossSpecialAttack1_3);

	m_mapObj.insert(make_pair("FlowerDie", pBossSpecialAttack1_3));

	// 스페셜 공격 1 : 이후에 미니언 랜덤 생성



	// 스페셜 공격 2 : Death의 등장

	CBulletA* pBossSpecialAttack2_1 = new CBulletA;

	pBossSpecialAttack2_1->SetEffect();

	pBossSpecialAttack2_1->SetBulletType(BT_BOSS_SPECIAL2_1);

	CAnimation* pDeathAni = pBossSpecialAttack2_1->CreateAnimation("Death");

	// 펑 ~ effect
	pDeathAni->AddClip(L"Boss/Bullet/DeathIdle.bmp", "DeathIdle", AO_TIME_RETURN, 0.5f, 0, 0, 10, 1, 1.f,
		0.f, 0.f, 96.f, 96.f, 255, 255, 255);

	pDeathAni->AddClip(L"Boss/Bullet/DeathDie.bmp", "DeathDie", AO_ONCE_DESTROY, 1.f, 0, 0, 10, 1, 0.f,
		0.f, 0.f, 96.f, 96.f, 255, 255, 255);

	pDeathAni->SetDefaultClip("DeathDie");

	SetRender(POSITION(0.f, 0.f), _SIZE(96.f, 96.f), pBossSpecialAttack2_1);

	m_mapObj.insert(make_pair("Death", pBossSpecialAttack2_1));

	// 이후에 죽으면 그곳을 향해 거대한 미사일이 빠른속도로 나아감.

	// 스페셜 공격 2 주먹질

	CSkillBullet* pEnergy = CreateSkillBullet(_SIZE(120.f, 79.f), 850.f, 0.f, ST_RECT, 25, 2.8f); // 900 speed 120 79   widht 2150

	CAnimation* pEnergyAni = pEnergy->CreateAnimation("Energy");

	pEnergyAni->AddClip(L"Boss/Bullet/Energy.bmp", "Energy", AO_LOOP, 0.5f, 0, 0, 4, 1, 0.f,
		1.5f, 0.f, 138.f, 99.f, 0, 0, 255);

	pEnergyAni->SetDefaultClip("Energy");

	SetRender(POSITION(-10.f, -13.f), _SIZE(137.f, 99.f), pEnergy);

	m_mapObj.insert(make_pair("Energy", pEnergy));
	
}

CTexture * CCreateManager::CreateTexture(const wstring & strFileName, unsigned int r, unsigned int g, unsigned int b, CObj * pObj)
{

	CTexture* pTexture = CTexture::SetTexture(strFileName, r, g, b);

	pObj->SetTexture(pTexture);
	
	return pTexture;
}

CSkillBullet * CCreateManager::CreateSkillBullet(_SIZE tSize, float fSpeed, float fLimitDist, SHAPE_TYPE eShape, int iDamage, float fLifeLimitTime)
{
	CSkillBullet* pSkill = new CSkillBullet;

	pSkill->SetSize(tSize);

	pSkill->SetSpeed(fSpeed);

	pSkill->SetLimitDist(fLimitDist);

	pSkill->SetShape(eShape);

	pSkill->SetDamage(iDamage);

	pSkill->SetLifeLimitTime(fLifeLimitTime);

	return pSkill;
}

CItem * CCreateManager::CreateItem(_SIZE tSize, float fSpeed, float fLimitDist, SHAPE_TYPE eShape, int iDamage, float fLifeLimitTime, ITEM_TYPE eItem)
{
	CItem* pItem = new CItem;

	pItem->SetSize(tSize);

	pItem->SetSpeed(fSpeed);

	pItem->SetLimitDist(fLimitDist);

	pItem->SetShape(eShape);

	pItem->SetDamage(iDamage);

	pItem->SetLifeLimitTime(fLifeLimitTime);

	pItem->SetItem(eItem);

	return pItem;
}

CBulletA * CCreateManager::CreateBulletA(_SIZE tSize, float fSpeed, float fLimitDist, SHAPE_TYPE eShape, int iDamage)
{
	CBulletA* pBullet = new CBulletA;

	pBullet->SetSize(tSize);

	pBullet->SetSpeed(fSpeed);

	pBullet->SetLimitDist(fLimitDist);

	pBullet->SetShape(eShape);

	pBullet->SetDamage(iDamage);

	return pBullet;
}

void CCreateManager::SetRender(POSITION tLeftTop, _SIZE tRenderSize, CObj* pObj)
{
	// 해당 오브젝트에 대해서 render 사이즈와 left top을 지정.
	pObj->SetRenderSize(tRenderSize);
	pObj->SetLeftTop(tLeftTop);
}

