#include "CEndScene.h"

#include "../Object/CUIButton.h"
#include "../Texture/CTexture.h"
#include "../Layer/CLayer.h"
#include "../Core/CSceneManager.h"
#include "../Math.h"
#include "../Core/CInput.h"


CEndScene::CEndScene()
{
}


CEndScene::~CEndScene()
{
}

void CEndScene::Input(float fDeltaTime)
{
	CScene::Input(fDeltaTime);
}

void CEndScene::Collision(float fDeltaTime)
{
	CScene::Collision(fDeltaTime);

	// 마우스의 위치를 가지고 있는 StartScene에서
	// 버튼의 충돌처리를 한다.

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

				case BW_HOME:
					GET_SINGLE(CSceneManager)->StartScene();
					


					break;


				case BW_RETRY:
					GET_SINGLE(CSceneManager)->InGameScene();
					


					break;
				}
			}

		}
	}

}

void CEndScene::Update(float fDeltaTime)
{
	CScene::Update(fDeltaTime);


	GetCursorPos(&m_tMousePos);
	ScreenToClient(GET_SINGLE(CSceneManager)->GetHandle(), &m_tMousePos);

}

void CEndScene::Render(HDC hDC, float fDeltaTime)
{
	CScene::Render(hDC, fDeltaTime);

}

bool CEndScene::Init()
{

	m_eInfo = SI_END;

	// End 화면을 만든다.
	m_pScreen = CTexture::SetTexture(L"End.bmp", 0, 255, 9);

	// Layer를 추가한다.
	CLayer* ButtonLayer = CreateLayer("ButtonLayer", 0);

	// Home Button
	CUIButton* pHomeBtn = CObj::CreateObj<CUIButton>(ButtonLayer);

	pHomeBtn->SetTexture(CTexture::SetTexture(L"UI/HomeButton.bmp", 5, 255, 255));

	pHomeBtn->SetRender(POSITION(0.f, 0.f), _SIZE(310.f, 130.f));

	pHomeBtn->SetSize(310.f, 130.f);

	pHomeBtn->SetButtonWhat(BW_HOME);

	pHomeBtn->SetPos(RS_WIDTH / 2.f - 155.f, RS_HEIGHT / 2.f -20.f);


	// Retry Button
	CUIButton* pEndBtn = CObj::CreateObj<CUIButton>(ButtonLayer);

	pEndBtn->SetTexture(CTexture::SetTexture(L"UI/RetryButton.bmp", 5, 255, 255));

	pEndBtn->SetRender(POSITION(0.f, 0.f), _SIZE(310.f, 130.f));

	pEndBtn->SetSize(310.f, 130.f);

	pEndBtn->SetButtonWhat(BW_RETRY);

	pEndBtn->SetPos(RS_WIDTH / 2.f - 155.f, RS_HEIGHT / 2.f + 160.f );


	return true;
}
