#pragma once


#define SAFE_DELETE(p) if(p){delete p; p = NULL;}
#define SAFE_DELETE_ARRAY(p) if(p){delete[] p; p = NULL;}

#define DECLARE_SINGLE(Type) \
private: \
static Type* m_pInst;\
public:\
static Type* GetInst()\
{\
if(!m_pInst) m_pInst = new Type; \
return m_pInst;\
}\
static void DestroyInst()\
{\
SAFE_DELETE(m_pInst); \
}\
private:\
Type();\
~Type();

#define GET_SINGLE(Type) Type::GetInst()
#define DESTROY_SINGLE(Type) Type::DestroyInst()
#define DEFINITION_SINGLE(Type) Type* Type::m_pInst = NULL
#define SAFE_RELEASE(p) if(p) {p->Release(); p = NULL;}
#define GET_RS() CCore::GetInst()->GetRS()

#define FULL_WIDTH 1920
#define FULL_HEIGHT 1080

#define RS_WIDTH 1280
#define RS_HEIGHT 720

#define CAMERA_WIDTH2 640
#define CAMERA_HEIGTH2 360

#define ATTACK_LV_MAX 5
#define SPEED_LV_MAX 5
#define WAVE_LV_MAX 6

// Boss Position

#define SRC_1 POSITION(0.f, 0.f)
#define SRC_2 POSITION(1920, 0.f)

#define SRC_3 POSITION(0.f, 1080.f)
#define SRC_4 POSITION(1920, 1080.f)

#define DEST POSITION(960.f, 540.f)

// Wave Position 윈도우 좌표계의 순서대로

#define TOP1 POSITION(60.f, 0.f)
#define TOP2 POSITION(510.f, 0.f)
#define TOP3 POSITION(960.f, 0.f)
#define TOP4 POSITION(1410.f, 0.f)
#define TOP5 POSITION(1860.f, 0.f)

#define LEFT1 POSITION(0.f, 40.f)
#define LEFT2 POSITION(0.f, 290.f)
#define LEFT3 POSITION(0.f, 540.f)
#define LEFT4 POSITION(0.f, 790.f)
#define LEFT5 POSITION(0.f, 1040.f)

#define BOTTOM1 POSITION(60.f, 1080.f)
#define BOTTOM2 POSITION(510.f, 1080.f)
#define BOTTOM3 POSITION(960.f, 1080.f)
#define BOTTOM4 POSITION(1410.f, 1080.f)
#define BOTTOM5 POSITION(1860.f, 1080.f)

#define RIGHT1 POSITION(1920.f, 40.f)
#define RIGHT2 POSITION(1920.f, 290.f)
#define RIGHT3 POSITION(1920.f, 540.f)
#define RIGHT4 POSITION(1920.f, 790.f)
#define RIGHT5 POSITION(1920.f, 1040.f)




#define FIND(Type) (Type*)GET_SINGLE(CCreateManager)

#define PLAYER CInGameScene::m_pPlayer
#define BOSS CInGameScene::m_pBoss
#define GAMETIME CInGameScene::m_fGameTime

#define KEYDOWN(Key) CInput::GetInst()->KeyDown(Key)
#define KEYPRESS(Key) CInput::GetInst()->KeyPress(Key)
#define KEYUP(Key) CInput::GetInst()->KeyUp(Key)

// 사용법 FIND(TYPE)->FindObj("Key");

template<typename T>
void Safe_Delete_VecList(T& p)
{
	T::iterator iter;
	T::iterator iterEnd = p.end();

	for (iter = p.begin(); iter != iterEnd; ++iter)
	{
		SAFE_DELETE((*iter));
	}

	p.clear();
}




template<typename T>
void Safe_Release_VecList(T& p)
{
	T::iterator iter;
	T::iterator iterEnd = p.end();

	for (iter = p.begin(); iter != iterEnd; ++iter)
	{
		SAFE_RELEASE((*iter));
	}

	p.clear();
}


template<typename T>
void Safe_Release_Map(T& p)
{
	T::iterator iter;
	T::iterator iterEnd = p.end();

	for (iter = p.begin(); iter != iterEnd; ++iter)
	{
		SAFE_RELEASE(iter->second);
	}

	p.clear();
}


template<typename T>
void Safe_Delete_Map(T& p)
{
	T::iterator iter;
	T::iterator iterEnd = p.end();

	for (iter = p.begin(); iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}

	p.clear();
}
