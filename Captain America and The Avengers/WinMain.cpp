#include "GameManager.h"
#include "Framework/d3d.h"
#include "Framework/DirectInput.h"
#include "Framework/GameTimer.h"
#include "Framework/SoundManager.h"
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if (d3d::GetInstance()->Init(hInstance, nCmdShow))
		return FALSE;
	if (DirectInput::GetInstance()->Init(d3d::GetInstance()->GetHWND()))
		return FALSE;
	if (SoundManager::GetInstance()->Init(d3d::GetInstance()->GetHWND())) 
		return FALSE;
	SoundManager::GetInstance()->LoadResources();
	GameManager::GetInstance()->Init(60);

	MSG msg;
	bool done = false;
	GameTimer::GetInstance()->StartCounter();

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				done = true;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			GameManager::GetInstance()->Running();
			GameTimer::GetInstance()->StartCounter();
		}
	}

	GameManager::GetInstance()->Release();
	return (int)msg.wParam;
}