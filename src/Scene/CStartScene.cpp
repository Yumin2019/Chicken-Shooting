#include "CStartScene.h"
#include "../Object/CUIButton.h"
#include "../Texture/CTexture.h"
#include "../Layer/CLayer.h"
#include "../Core/CSceneManager.h"
#include "../Math.h"
#include "../Core/CInput.h"
#include "../Core/CCore.h"

CStartScene::CStartScene()
{
}


CStartScene::~CStartScene()
{
}

void CStartScene::Input(float fDeltaTime)
{
	CScene::Input(fDeltaTime);
}

void CStartScene::Collision(float fDeltaTime)
{
	CScene::Collision(fDeltaTime);

	list<CObj*>& m_ButtonList = FindLayer("ButtonLayer")->GetList();

	list<CObj*>::iterator iter;
	list<CObj*>::iterator iterEnd = m_ButtonList.end();

	POSITION tPos;

	tPos.x = m_tMousePos.x;
	tPos.y = m_tMousePos.y;

	for (iter = m_ButtonList.begin(); iter != iterEnd; ++iter)
	{

		if (Math::PointInRect((*iter)->GetPos(), (*iter)->GetSize(), tPos))
		{
			if (((CUIButton*)(*iter))->GetButtonState() == BS_NONE)
			{
				((CUIButton*)(*iter))->SetButtonState(BS_MOUSEON);
				((CUIButton*)(*iter))->SetWhite(0.f, 130.f);

			}
			
		}
		else
		{
			((CUIButton*)(*iter))->SetButtonState(BS_NONE);
			((CUIButton*)(*iter))->SetWhite(0.f, 0.f);
		}



				if (((CUIButton*)(*iter))->GetButtonState() == BS_MOUSEON)
				{
							if (KEYDOWN("MouseLButton"))
							{
								// 버튼 타입에 따라서 처리가 달라진다.

									switch (((CUIButton*)(*iter))->GetButtonWhat())
									{
									case BW_START:
										// 인게임 씬 변환.
										GET_SINGLE(CSceneManager)->InGameScene();

										break;


									case BW_QUIT:
										GET_SINGLE(CCore)->DestroyGame();

										break;
									case BW_RETRY:
										break;
									}
							}

				}
	}

}

void CStartScene::Update(float fDeltaTime)
{
	CScene::Update(fDeltaTime);

	// mouseon click을 검사한다.

	GetCursorPos(&m_tMousePos);
	ScreenToClient(GET_SINGLE(CSceneManager)->GetHandle(), &m_tMousePos);
}

void CStartScene::Render(HDC hDC, float fDeltaTime)
{
	CScene::Render(hDC, fDeltaTime);

	TextOut(hDC, 100, 100, L"Chicken Shooting", 17); 


}

bool CStartScene::Init()
{
	m_eInfo = SI_MAIN;

	// 메인 화면을 만든다.
	m_pScreen = CTexture::SetTexture(L"Main.bmp", 0, 0, 0);

	// Layer를 추가한다.
	CLayer* ButtonLayer = CreateLayer("ButtonLayer", 0);


	// Start Button
	CUIButton* pStartBtn = CObj::CreateObj<CUIButton>(ButtonLayer);

	pStartBtn->SetTexture(CTexture::SetTexture(L"UI/PlayButton.bmp", 5, 0, 0));

	pStartBtn->SetRender(POSITION(0.f, 0.f), _SIZE(310.f, 130.f));

	pStartBtn->SetSize(310.f, 130.f);

	pStartBtn->SetButtonWhat(BW_START);

	pStartBtn->SetPos(RS_WIDTH / 2.f - 155.f, RS_HEIGHT / 2.f - 20.f);


	// End Button
	CUIButton* pEndBtn = CObj::CreateObj<CUIButton>(ButtonLayer);

	pEndBtn->SetTexture(CTexture::SetTexture(L"UI/ExitButton.bmp", 5, 0, 0));

	pEndBtn->SetRender(POSITION(0.f, 0.f), _SIZE(310.f, 130.f));

	pEndBtn->SetSize(310.f, 130.f);

	pEndBtn->SetButtonWhat(BW_QUIT);

	pEndBtn->SetPos(RS_WIDTH / 2.f - 155.f, RS_HEIGHT / 2.f + 160.f);
	

	return true;
}

