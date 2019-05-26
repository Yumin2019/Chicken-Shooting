#include "Core\CCore.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{

	srand((unsigned int)time(NULL));

	if (!GET_SINGLE(CCore)->Init(hInstance))
	{
		DESTROY_SINGLE(CCore);
		return 0;
	}

	GET_SINGLE(CCore)->Run();

	DESTROY_SINGLE(CCore);


	

	return 0;
}
