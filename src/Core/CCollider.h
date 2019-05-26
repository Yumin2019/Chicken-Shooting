#pragma once

#include "../Game.h"

class CCollider
{
	DECLARE_SINGLE(CCollider);


private:


	class CObj*			m_pObj; // 자기 소속 Obj


	// 충돌체 기반으로 움직이고 충돌체 기반으로 충돌처리를 할 예정이기 때문에, 
	// obj에 있던 대부분의 정보를 여기서 처리한다.
	

	// Obj에서는 Render를 기준으로 움직이고, 그걸 충돌체가 따라가는 것으로 한다.



};

