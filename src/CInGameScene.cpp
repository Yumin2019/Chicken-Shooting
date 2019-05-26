#include "Scene\CInGameScene.h"
#include "Layer/CLayer.h"
#include "Texture/CTexture.h"
#include "Object/CUIPanel.h"
#include "Object/CPlayer.h"
#include "Object/CMinionA.h"
#include "Object/CMinionB.h"
#include "Object/CBoss.h"
#include "Core\CInput.h"
#include "Core\CCreateManager.h"

CPlayer* CInGameScene::m_pPlayer = NULL;
bool CInGameScene::m_bPlayer;

CBoss* CInGameScene::m_pBoss = NULL;
bool CInGameScene::m_bBoss;

float CInGameScene::m_fGameTime;
float CInGameScene::l;
float CInGameScene::t;

CInGameScene::CInGameScene() :
	m_iWaveLv(1),
	m_eWave(WT_FIRST),
	m_fMonsterTime(0.f),
	m_fTermTime(0.f),
	m_fTermLimitTime(15.f),
	m_fMonsterLimitTime(5.f),
	m_bWaveTerm(false)
{
}


CInGameScene::~CInGameScene()
{
	SAFE_RELEASE(m_pPlayer);
	SAFE_RELEASE(m_pBoss);

	m_bBoss = false;
	m_bPlayer = false;
	
	GAMETIME = 0.f;
}

void CInGameScene::WaveSet()
{
	// 웨이브 셋에서는 WaveLv과 Wave Enum에 따라서 처리한다.


	switch (m_iWaveLv)
	{
	case 1:
		Lv1();
		break;
	case 2:
		Lv2();
		break;
	case 3:
		Lv3();
		break;
	case 4:
		Lv4();
		break;
	case 5:
		Lv5();
		break;
	case 6:
		Lv6();
		break;
		// 보스생성후 7레벨. 즉 아무 짓도 안함.
	}
}

void CInGameScene::Lv1()
{
	switch(m_eWave)
	{
	case WT_FIRST:
		
		m_eWave = WT_SECOND;

		MinionACreate(LEFT3, true, LEFT);
		MinionACreate(RIGHT3, true, RIGHT);

		m_fMonsterLimitTime = 15.f;
		
		break;

	case WT_SECOND:

		
		MinionACreate(TOP4, false, TOP);
		MinionACreate(BOTTOM2, false, BOTTOM);

		++m_iWaveLv;
		m_eWave = WT_FIRST;
		m_bWaveTerm = true;

		m_fMonsterLimitTime = 15.f; 

		break;
	}
}

void CInGameScene::Lv2()
{

	switch (m_eWave)
	{
	case WT_FIRST:

		m_eWave = WT_SECOND;

		MinionACreate(LEFT3, true, LEFT);
		MinionACreate(RIGHT3, true, RIGHT);

		m_fMonsterLimitTime = 20.f;

		break;

	case WT_SECOND:


		MinionBCreate(LEFT3, true, LEFT);
		MinionBCreate(RIGHT3, true, RIGHT);

		++m_iWaveLv;
		m_eWave = WT_FIRST;
		m_bWaveTerm = true;

		m_fMonsterLimitTime = 20.f;

		break;
	}
}

void CInGameScene::Lv3()
{
	switch (m_eWave)
	{
	case WT_FIRST:

		m_eWave = WT_SECOND;

		MinionBCreate(LEFT5, true, LEFT);
		MinionBCreate(RIGHT1, true, RIGHT);
		MinionACreate(BOTTOM3, false, BOTTOM);

		m_fMonsterLimitTime = 25.f;

		break;

	case WT_SECOND:


		MinionBCreate(LEFT3, true, LEFT);
		MinionBCreate(RIGHT3, true, RIGHT);

		MinionACreate(BOTTOM2, false, BOTTOM);
		MinionACreate(BOTTOM4, false , BOTTOM);

		++m_iWaveLv;
		m_eWave = WT_FIRST;
		m_bWaveTerm = true;

		m_fMonsterLimitTime = 25.f;

		break;
	}
}

void CInGameScene::Lv4()
{
	switch (m_eWave)
	{
	case WT_FIRST:

		m_eWave = WT_SECOND;

		MinionBCreate(LEFT2, true, LEFT);
		MinionACreate(LEFT3, true, LEFT);
		MinionBCreate(LEFT4, true, LEFT);


		MinionBCreate(RIGHT2, true, RIGHT);
		MinionACreate(RIGHT3, true, RIGHT);
		MinionBCreate(RIGHT4, true, RIGHT);


		m_fMonsterLimitTime = 35.f;

		break;

	case WT_SECOND:


		MinionBCreate(TOP2, false, TOP);
		MinionACreate(TOP3, false, TOP);
		MinionBCreate(TOP4, false, TOP);


		MinionBCreate(BOTTOM2, false, BOTTOM);
		MinionACreate(BOTTOM3, false, BOTTOM);
		MinionBCreate(BOTTOM4, false, BOTTOM);

		++m_iWaveLv;
		m_eWave = WT_FIRST;
		m_bWaveTerm = true;

		m_fMonsterLimitTime = 35.f;

		break;
	}
}

void CInGameScene::Lv5()
{
	switch (m_eWave)
	{
	case WT_FIRST:

		m_eWave = WT_SECOND;

		MinionBCreate(LEFT2, true, LEFT);
		MinionACreate(LEFT3, true, LEFT);
		MinionBCreate(LEFT4, true, LEFT);


		MinionBCreate(BOTTOM2, false, BOTTOM);
		MinionACreate(BOTTOM3, false, BOTTOM);
		MinionBCreate(BOTTOM4, false, BOTTOM);

		m_fMonsterLimitTime = 35.f;

		break;

	case WT_SECOND:


		MinionBCreate(TOP2, false, TOP);
		MinionACreate(TOP3, false, TOP);
		MinionBCreate(TOP4, false, TOP);


		MinionBCreate(RIGHT2, true, RIGHT);
		MinionACreate(RIGHT3, true, RIGHT);
		MinionBCreate(RIGHT4, true, RIGHT);
		

		++m_iWaveLv;
		m_eWave = WT_FIRST;
		m_bWaveTerm = true;

		m_fMonsterLimitTime = 35.f;

		break;
	}
}

void CInGameScene::Lv6()
{
	// 보스전은 Wave와 상관없다.

	CBoss* pBoss = FIND(CBoss)->FindObj("Boss")->Clone();

	pBoss->SetAnimation();

	pBoss->AddRef();

	m_pBoss = pBoss;

	m_bBoss = true;

	FindLayer("MonsterLayer")->GetList().push_front(pBoss);

	// Boss를 따라다니는 UI가 필요하다.

	CLayer* pUILayer = FindLayer("UILayer");

	// Boss Hp Border

	CUIPanel* pBossHPBorder = new CUIPanel;

	pBossHPBorder->SetRender(POSITION(0.f, 0.f), _SIZE(500.f, 50.f));

	pBossHPBorder->SetTexture(CTexture::SetTexture(L"UI/HPBorder.bmp", 255, 255, 255));

	pBossHPBorder->SetPos(750.f, 10.f);

	pBossHPBorder->SetSize(0.f, 0.f);

	pBossHPBorder->SetPanelType(PT_IMAGE);

	pUILayer->GetList().push_back(pBossHPBorder);


	// Boss HP Bar

	CUIPanel* pBossHPBar = new CUIPanel;

	pBossHPBar->SetRender(POSITION(0.f, 0.f), _SIZE(496.f, 46.f));

	pBossHPBar->SetTexture(CTexture::SetTexture(L"UI/HPBar.bmp", 255, 255, 255));

	pBossHPBar->SetPos(752.f, 12.f);

	pBossHPBar->SetSize(0.f, 0.f);

	pBossHPBar->SetPanelType(PT_BOSS_HPBAR);

	pUILayer->GetList().push_back(pBossHPBar);


	// HP Bar Image Boss

	CUIPanel* pBossImage = new CUIPanel;

	pBossImage->SetRender(POSITION(0.f, 0.f), _SIZE(41.f, 57.f));

	pBossImage->SetTexture(CTexture::SetTexture(L"UI/Boss.bmp", 255, 255, 255));

	pBossImage->SetPos(730.f, 25.f);

	pBossImage->SetSize(0.f, 0.f);

	pBossImage->SetPanelType(PT_IMAGE);

	pUILayer->GetList().push_back(pBossImage);

	++m_iWaveLv;
}

void CInGameScene::MinionACreate(POSITION tPos, bool XY, DIR eStart)
{

	CMinionA* pMinion = FIND(CMinionA)->FindObj("MinionA")->Clone();


	if (!pMinion)
	{
		SAFE_RELEASE(pMinion);
		return;

	}

	pMinion->SetPos(MovePos(eStart, tPos));

	pMinion->SetAnimation();


	switch (XY) // x축 y축 지정.
	{
	case true:
		pMinion->SetXY(true);

		if (pMinion->GetCenterX() <= FULL_WIDTH / 2.f)
			pMinion->SetStandDir(SD_RIGHT);
		else
			pMinion->SetStandDir(SD_LEFT);

		break;

	case false:
		pMinion->SetXY(false);

		if (pMinion->GetCenterY() >= FULL_HEIGHT / 2.f)
			pMinion->SetStandDir(SD_LEFT);
		else
			pMinion->SetStandDir(SD_RIGHT);

		break;
	}

	pMinion->Init();

	FindLayer("MonsterLayer")->GetList().push_front(pMinion);
}

void CInGameScene::MinionBCreate(POSITION tPos, bool XY, DIR eStart)
{

	CMinionA* pMinion = FIND(CMinionA)->FindObj("MinionB")->Clone();


	if (!pMinion)
	{
		SAFE_RELEASE(pMinion);
		return;

	}

	pMinion->SetPos(MovePos(eStart, tPos));

	pMinion->SetAnimation();


	switch (XY) // x축 y축 지정.
	{
	case true:
		pMinion->SetXY(true);

		if (pMinion->GetCenterX() <= FULL_WIDTH / 2.f)
			pMinion->SetStandDir(SD_RIGHT);
		else
			pMinion->SetStandDir(SD_LEFT);

		break;

	case false:
		pMinion->SetXY(false);

		if (pMinion->GetCenterY() >= FULL_HEIGHT / 2.f)
			pMinion->SetStandDir(SD_LEFT);
		else
			pMinion->SetStandDir(SD_RIGHT);

		break;
	}

	pMinion->Init();

	FindLayer("MonsterLayer")->GetList().push_front(pMinion);
}

POSITION CInGameScene::MovePos(DIR eStart, POSITION tPos)
{
	//x y 충돌 Size (15.f, 40.f)

	POSITION m_tPos;

	switch(eStart)
	{
	case TOP:
		// x좌표만 절반만큼 왼쪽.

		m_tPos.x = tPos.x - 7.5f;
		m_tPos.y = tPos.y;
		
		break;

	case BOTTOM:
		// y Size만큼 올라간다.
		// x 절반만큼 왼쪽

		m_tPos.x = tPos.x - 7.5f;
		m_tPos.y = tPos.y - 40.f;

		break;

	case RIGHT:
		// x size 만큼 왼쪽 
		// y size 절반만큼 위로
		
		m_tPos.x = tPos.x - 15.f;
		m_tPos.y = tPos.y - 20.f;

		break;

	case LEFT:

		m_tPos.x = tPos.x;
		m_tPos.y = tPos.y - 20.f;

		break;
	}

	return m_tPos;
}

bool CInGameScene::Init()
{

	// INGAME UI 
	m_eInfo = SI_INGAME;


	m_pScreen = CTexture::SetTexture(L"Screen.bmp", 0, 0, 0);

	CreateLayer("FirstEffectLayer", 0);

	// Player 생성
	CPlayer* pPlayer = new CPlayer;

	if (!pPlayer->Init())
		return false;

	m_pPlayer = pPlayer;

	m_pPlayer->AddRef();

	m_bPlayer = true;

	CreateLayer("MonsterLayer", 1)->GetList().push_back(m_pPlayer);

	CreateLayer("DeadMonsterBulletLayer", 2);
	CreateLayer("SecondEffectLayer", 3);

	CLayer* pUILayer = CreateLayer("UILayer", 4);


	// HP Border

	CUIPanel* pHPBorder = new CUIPanel;

	pHPBorder->SetRender(POSITION(0.f, 0.f), _SIZE(500.f, 50.f));

	pHPBorder->SetTexture(CTexture::SetTexture(L"UI/HPBorder.bmp", 255, 255, 255));

	pHPBorder->SetPos(30.f, 10.f);

	pHPBorder->SetSize(0.f, 0.f);

	pHPBorder->SetPanelType(PT_IMAGE);

	pUILayer->GetList().push_back(pHPBorder);


	// HP Bar
	CUIPanel* pHPBar = new CUIPanel;

	pHPBar->SetRender(POSITION(0.f, 0.f), _SIZE(496.f, 46.f));

	pHPBar->SetTexture(CTexture::SetTexture(L"UI/HPBar.bmp", 255, 255, 255));

	pHPBar->SetPos(32.f, 12.f);

	pHPBar->SetSize(0.f, 0.f);

	pHPBar->SetPanelType(PT_PLAYER_HPBAR);

	pUILayer->GetList().push_back(pHPBar);


	// HP Bar Image

	CUIPanel* pPlayerImage = new CUIPanel;

	pPlayerImage->SetRender(POSITION(0.f, 0.f), _SIZE(63.f, 50.f));

	pPlayerImage->SetTexture(CTexture::SetTexture(L"UI/Player.bmp", 255, 255, 255));

	pPlayerImage->SetPos(5.f, 25.f);

	pPlayerImage->SetSize(0.f, 0.f);

	pPlayerImage->SetPanelType(PT_IMAGE);

	pUILayer->GetList().push_back(pPlayerImage);

	


	// Shield Pic

	CUIPanel* pShield = new CUIPanel;

	pShield->SetRender(POSITION(0.f, 0.f), _SIZE(60.f, 60.f));

	pShield->SetTexture(CTexture::SetTexture(L"Item/Shield.bmp", 0, 0, 0));

	pShield->SetPos(10.f, 140.f);

	pShield->SetSize(0.f, 0.f);

	pShield->SetPanelType(PT_IMAGE);

	pUILayer->GetList().push_back(pShield);


	// Bomb Pic

	CUIPanel* pBomb = new CUIPanel;

	pBomb->SetRender(POSITION(0.f, 0.f), _SIZE(60.f, 60.f));

	pBomb->SetTexture(CTexture::SetTexture(L"Item/Bomb.bmp", 0, 0, 0));

	pBomb->SetPos(10.f, 70.f);

	pBomb->SetSize(0.f, 0.f);

	pBomb->SetPanelType(PT_IMAGE);

	pUILayer->GetList().push_back(pBomb);


	// AttackLv

	CUIPanel* pAttack = new CUIPanel;

	pAttack->SetRender(POSITION(0.f, 0.f), _SIZE(50.f, 48.f));

	pAttack->SetTexture(CTexture::SetTexture(L"Item/RedRuby.bmp", 255, 255, 255));

	pAttack->SetPos(405.f, 70.f);

	pAttack->SetWhite(5.f, 5.f);

	pAttack->SetSize(0.f, 0.f);

	pAttack->SetPanelType(PT_ATTACK_LV);

	pUILayer->GetList().push_back(pAttack);


	// Speed Lv

	CUIPanel* pSpeed = new CUIPanel;

	pSpeed->SetRender(POSITION(0.f, 0.f), _SIZE(45.f, 45.f));

	pSpeed->SetTexture(CTexture::SetTexture(L"Item/BlueRuby.bmp", 255, 255, 255));

	pSpeed->SetPos(465.f, 70.f);

	pSpeed->SetWhite(7.f, 7.f);

	pSpeed->SetSize(0.f, 0.f);

	pSpeed->SetPanelType(PT_SPEED_LV);

	pUILayer->GetList().push_back(pSpeed);



	// Attack UI

	CUIPanel* pAttackUI = new CUIPanel;

	pAttackUI->SetRender(POSITION(0.f, 0.f), _SIZE(60.f, 222.f));

	pAttackUI->SetTexture(CTexture::SetTexture(L"UI/AttackUI.bmp", 0, 255, 255));

	pAttackUI->SetPos(0.f, RS_HEIGHT /2.f -111.f);

	pAttackUI->SetSize(0.f, 0.f);

	pAttackUI->SetPanelType(PT_IMAGE);

	pUILayer->GetList().push_back(pAttackUI);


	// Skill CoolDown

	CUIPanel* pSkillCoolDown1 = new CUIPanel;

	pSkillCoolDown1->SetRender(POSITION(0.f, 0.f), _SIZE(47.f, 47.f));

	pSkillCoolDown1->SetTexture(CTexture::SetTexture(L"UI/SkillCoolDown.bmp", 0, 255, 255));

	pSkillCoolDown1->SetPos(8.f, RS_HEIGHT / 2.f - 106.f);

	pSkillCoolDown1->SetBlend(true);

	pSkillCoolDown1->SetSize(0.f, 0.f);

	pSkillCoolDown1->SetPanelType(PT_SKILL_COOLDOWN_A);

	pUILayer->GetList().push_back(pSkillCoolDown1);



	CUIPanel* pSkillCoolDown2 = new CUIPanel;

	pSkillCoolDown2->SetRender(POSITION(0.f, 0.f), _SIZE(47.f, 47.f));

	pSkillCoolDown2->SetTexture(CTexture::SetTexture(L"UI/SkillCoolDown.bmp", 0, 255, 255));

	pSkillCoolDown2->SetPos(8.f, RS_HEIGHT / 2.f - 106.f + 55.f);

	pSkillCoolDown2->SetBlend(true);

	pSkillCoolDown2->SetSize(0.f, 0.f);

	pSkillCoolDown2->SetPanelType(PT_SKILL_COOLDOWN_S);

	pUILayer->GetList().push_back(pSkillCoolDown2);



	CUIPanel* pSkillCoolDown3 = new CUIPanel;

	pSkillCoolDown3->SetRender(POSITION(0.f, 0.f), _SIZE(47.f, 47.f));

	pSkillCoolDown3->SetTexture(CTexture::SetTexture(L"UI/SkillCoolDown.bmp", 0, 255, 255));

	pSkillCoolDown3->SetPos(8.f, RS_HEIGHT / 2.f - 106.f + 55.f * 2.f);

	pSkillCoolDown3->SetBlend(true);

	pSkillCoolDown3->SetSize(0.f, 0.f);

	pSkillCoolDown3->SetPanelType(PT_SKILL_COOLDOWN_D);

	pUILayer->GetList().push_back(pSkillCoolDown3);




	CUIPanel* pSkillCoolDown4 = new CUIPanel;

	pSkillCoolDown4->SetRender(POSITION(0.f, 0.f), _SIZE(47.f, 47.f));

	pSkillCoolDown4->SetTexture(CTexture::SetTexture(L"UI/SkillCoolDown.bmp", 0, 255, 255));

	pSkillCoolDown4->SetPos(8.f, RS_HEIGHT / 2.f - 106.f + 55.f * 3.f);

	pSkillCoolDown4->SetBlend(true);

	pSkillCoolDown4->SetSize(0.f, 0.f);

	pSkillCoolDown4->SetPanelType(PT_SKILL_COOLDOWN_R);

	pUILayer->GetList().push_back(pSkillCoolDown4);





	return true;
}

void CInGameScene::Input(float fDeltaTime)
{
	CScene::Input(fDeltaTime);


	// 개발용 인터페이스 그냥 냅두는 걸로.
	

	if (KEYDOWN("MinionA")) 
	{
		CMinionA* pMinion = FIND(CMinionA)->FindObj("MinionA")->Clone();

		pMinion->SetAnimation();

		pMinion->SetPos(500.f, 500.f);
		pMinion->SetStandDir(SD_RIGHT);
		pMinion->SetXY(false);

		FindLayer("MonsterLayer")->GetList().push_front(pMinion);

	}

	if (KEYDOWN("MinionB"))
	{
		CMinionB* pMinion = FIND(CMinionB)->FindObj("MinionB")->Clone();

		pMinion->SetAnimation();

		pMinion->SetPos(200.f, 500.f);
		pMinion->SetStandDir(SD_RIGHT);
		pMinion->SetXY(false);


		FindLayer("MonsterLayer")->GetList().push_front(pMinion);

	}

	if (KEYDOWN("Boss") && !m_bBoss) 
	{
		CBoss* pBoss = FIND(CBoss)->FindObj("Boss")->Clone();

		pBoss->SetAnimation();

		pBoss->AddRef();

		m_pBoss = pBoss;

		m_bBoss = true;

		FindLayer("MonsterLayer")->GetList().push_front(pBoss);

		// Boss를 따라다니는 UI가 필요하다.

		CLayer* pUILayer = FindLayer("UILayer");

		// Boss Hp Border

		CUIPanel* pBossHPBorder = new CUIPanel;

		pBossHPBorder->SetRender(POSITION(0.f, 0.f), _SIZE(500.f, 50.f));

		pBossHPBorder->SetTexture(CTexture::SetTexture(L"UI/HPBorder.bmp", 255, 255, 255));

		pBossHPBorder->SetPos(750.f, 10.f);

		pBossHPBorder->SetSize(0.f, 0.f);

		pBossHPBorder->SetPanelType(PT_IMAGE);

		pUILayer->GetList().push_back(pBossHPBorder);


		// Boss HP Bar

		CUIPanel* pBossHPBar = new CUIPanel;

		pBossHPBar->SetRender(POSITION(0.f, 0.f), _SIZE(496.f, 46.f));

		pBossHPBar->SetTexture(CTexture::SetTexture(L"UI/HPBar.bmp", 255, 255, 255));

		pBossHPBar->SetPos(752.f, 12.f);

		pBossHPBar->SetSize(0.f, 0.f);

		pBossHPBar->SetPanelType(PT_BOSS_HPBAR);

		pUILayer->GetList().push_back(pBossHPBar);


		// HP Bar Image Boss

		CUIPanel* pBossImage = new CUIPanel;

		pBossImage->SetRender(POSITION(0.f, 0.f), _SIZE(41.f, 57.f));

		pBossImage->SetTexture(CTexture::SetTexture(L"UI/Boss.bmp", 255, 255, 255));

		pBossImage->SetPos(730.f, 25.f);

		pBossImage->SetSize(0.f, 0.f);

		pBossImage->SetPanelType(PT_IMAGE);

		pUILayer->GetList().push_back(pBossImage);




	}


	
}

void CInGameScene::Collision(float fDeltaTime)
{
	CScene::Collision(fDeltaTime);


}

void CInGameScene::Update(float fDeltaTime)
{
	CScene::Update(fDeltaTime);



	// Camera Update

	POSITION tPlayerPos = m_pPlayer->GetCenter();

	float r, b;

	l = tPlayerPos.x - CAMERA_WIDTH2;
	t = tPlayerPos.y - CAMERA_HEIGTH2;

	r = tPlayerPos.x + CAMERA_WIDTH2;
	b = tPlayerPos.y + CAMERA_HEIGTH2;


	if (l < 0)
		l = 0;
	
	if (r > FULL_WIDTH)
	{
		r = FULL_WIDTH;
		l = FULL_WIDTH - RS_WIDTH;
	}


	if (t < 0)
		t = 0;


	if (b > FULL_HEIGHT)
	{
		b = FULL_HEIGHT;
		t = FULL_HEIGHT - RS_HEIGHT;
	}

	////////////////////////////////////////////

	if (m_bWaveTerm)
	{
		m_fTermTime += fDeltaTime;

		if (m_fTermTime >= fDeltaTime)
		{
			m_fTermTime = 0.f;
			m_bWaveTerm = false;
		}
	}
	else
	{
		// TermTime이 아닐 경우에만 몬스터 시간을 재야 한다.

		m_fMonsterTime += fDeltaTime;

		if (m_fMonsterTime >= m_fMonsterLimitTime)
		{
			WaveSet();
			m_fMonsterTime = 0.f;

		}
	}
}

void CInGameScene::Render(HDC hDC, float fDeltaTime)
{
	CScene::Render(hDC, fDeltaTime);



	// Bomb & Shield Count Text

	TCHAR strBomb[10];
	wsprintf(strBomb, L"X %d", m_pPlayer->GetBombCount());
	TextOut(hDC, 80, 80, strBomb, lstrlen(strBomb));

	wsprintf(strBomb, L"X %d", m_pPlayer->GetShieldCount());
	TextOut(hDC, 80, 150, strBomb, lstrlen(strBomb));

	if (GAMETIME)
	{
		swprintf_s(strBomb, L"%.1f", 3.f - GAMETIME);
		TextOut(hDC, RS_WIDTH / 2.f - 25.f , RS_HEIGHT / 2.f - 25.f, strBomb, lstrlen(strBomb));
	}


}
