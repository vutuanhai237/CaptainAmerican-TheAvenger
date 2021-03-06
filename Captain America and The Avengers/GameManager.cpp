#include "GameManager.h"
#include "Framework/GameTimer.h"
#include "Framework/Debug.h"
#include "Framework/d3d.h"
#include "Framework/DirectInput.h"
#include "Framework/Texture.h"
#include "Framework/Sprite.h"
#include "SceneManager.h"
#include "Charleston.h"
#include "Player.h"
#include "RedRocketRobot.h"
#include "ItemsHolder.h"
#include "Door.h"
#include "Platform.h"
#include "GrayRocket.h"
#include "BossGragas.h"
#include "BossWizard.h"
#include "ElectricShockwaveBarrier.h"
DWORD GameManager::StTime = GetTickCount();

GameManager* GameManager::Instance = NULL;
int GameManager::TotalFrame = 0;
float GameManager::delta = 0.0f;

GameManager* GameManager::GetInstance()
{
	if (!Instance)
		Instance = new GameManager();
	return Instance;
}

void GameManager::Init(int fps)
{
	FPS = fps;
	TPF = 1.0f / fps; // 1.0f / fps;

	Texture *texture = Texture::GetInstance();
	int ID;
	texture->Add(ID, L"Resources/Map/TitleMap.png", D3DCOLOR_ARGB(0, 0, 0, 0));

	// Red robot rocket zone 1 2 3 4
	texture->Add(ID, L"Resources/Enemy/RedRocketRobot/RedRobotRocketIdle.png", D3DCOLOR_XRGB(255, 0, 255));
	texture->Add(ID, L"Resources/Enemy/RedRocketRobot/RedRobotRocketRunning.png", D3DCOLOR_XRGB(255, 0, 255));
	texture->Add(ID, L"Resources/Enemy/RedRocketRobot/RedRobotRocketDucking.png", D3DCOLOR_XRGB(255, 0, 255));
	texture->Add(ID, L"Resources/Enemy/RedRocketRobot/RedRobotRocketBeaten.png", D3DCOLOR_XRGB(255, 0, 255));
	// Red rocket 5 6, explode 7
	texture->Add(ID, L"Resources//Enemy//RedRocketRobot//RedRocketCrossed.png", D3DCOLOR_XRGB(255, 0, 255));
	texture->Add(ID, L"Resources//Enemy//RedRocketRobot//RedRocketHorizontal.png", D3DCOLOR_XRGB(255, 0, 255));
	texture->Add(ID, L"Resources//Enemy//Explode.png", D3DCOLOR_XRGB(255, 0, 255));
	// blue soldier 8 9 10 11
	texture->Add(ID, L"Resources//Enemy//BlueSoldier//BlueSoldierIdleState.png", D3DCOLOR_XRGB(255, 0, 255));
	texture->Add(ID, L"Resources//Enemy//BlueSoldier//BlueSoldierRunningState.png", D3DCOLOR_XRGB(255, 0, 255));
	texture->Add(ID, L"Resources//Enemy//BlueSoldier//BlueSoldierDuckingState.png", D3DCOLOR_XRGB(255, 0, 255));
	texture->Add(ID, L"Resources//Enemy//BlueSoldier//BlueSoldierBeatenState.png", D3DCOLOR_XRGB(255, 0, 255));
	// bullet 12
	texture->Add(ID, L"Resources//Enemy//BlueSoldier//SoldierBullet.png", D3DCOLOR_XRGB(255, 0, 255));
	// laser bullet 13
	texture->Add(ID, L"Resources//Enemy//BossWizard//LaserBullet.png", D3DCOLOR_XRGB(255, 0, 255));
	// energy bullet 14 15 16
	texture->Add(ID, L"Resources//Enemy//BossWizard//EnergyBulletHorizontal.png", D3DCOLOR_XRGB(255, 0, 255));
	texture->Add(ID, L"Resources//Enemy//BossWizard//EnergyBulletVertical.png", D3DCOLOR_XRGB(255, 0, 255));
	texture->Add(ID, L"Resources//Enemy//BossWizard//EnergyBulletCrossed.png", D3DCOLOR_XRGB(255, 0, 255));
	// green soldier 17 18 19 20
	texture->Add(ID, L"Resources//Enemy//GreenSoldier//GreenSoldierIdleState.png", D3DCOLOR_XRGB(255, 0, 255));
	texture->Add(ID, L"Resources//Enemy//GreenSoldier//GreenSoldierRunningState.png", D3DCOLOR_XRGB(255, 0, 255));
	texture->Add(ID, L"Resources//Enemy//GreenSoldier//GreenSoldierDuckingState.png", D3DCOLOR_XRGB(255, 0, 255));
	texture->Add(ID, L"Resources//Enemy//GreenSoldier//GreenSoldierBeatenState.png", D3DCOLOR_XRGB(255, 0, 255));
	// bat 21 22 23 24
	texture->Add(ID, L"Resources//Enemy//Bat//BatIdle.png", D3DCOLOR_XRGB(255, 0, 255));
	texture->Add(ID, L"Resources//Enemy//Bat//BatIdle2.png", D3DCOLOR_XRGB(255, 0, 255));
	texture->Add(ID, L"Resources//Enemy//Bat//BatFlying.png", D3DCOLOR_XRGB(255, 0, 255));
	texture->Add(ID, L"Resources//Enemy//Bat//BatFlying2.png", D3DCOLOR_XRGB(255, 0, 255));
	// gray robot 25 26 27
	texture->Add(ID, L"Resources//Enemy//GrayRobot//GrayRobotIdle.png", D3DCOLOR_XRGB(255, 0, 255));
	texture->Add(ID, L"Resources//Enemy//GrayRobot//GrayRobotFiring.png", D3DCOLOR_XRGB(255, 0, 255));
	texture->Add(ID, L"Resources//Enemy//GrayRobot//GrayRobotBeaten.png", D3DCOLOR_XRGB(255, 0, 255));
	// super laser bullet 28
	texture->Add(ID, L"Resources//Enemy//BossGragas//BossGragasLaser.png", D3DCOLOR_XRGB(255, 0, 255));
	// barrel 29
	texture->Add(ID, L"Resources//Enemy//BossGragas//BossGragasBarrel.png", D3DCOLOR_XRGB(255, 0, 255));
	// turret 30
	texture->Add(ID, L"Resources//Enemy//Turret//Turret.png", D3DCOLOR_XRGB(255, 0, 255));
	// bullet turret 31
	texture->Add(ID, L"Resources//Enemy//Turret//BulletTurret.png", D3DCOLOR_XRGB(255, 0, 255));
	// Gray robot rocket zone 32 33 34
	texture->Add(ID, L"Resources/Enemy/GrayRocketRobot/GrayRocketRobotRunning.png", D3DCOLOR_XRGB(255, 0, 255));
	texture->Add(ID, L"Resources/Enemy/GrayRocketRobot/GrayRocketRobotDucking.png", D3DCOLOR_XRGB(255, 0, 255));
	texture->Add(ID, L"Resources/Enemy/GrayRocketRobot/GrayRocketRobotBeaten.png", D3DCOLOR_XRGB(255, 0, 255));






	texture->Add(ItemsHolder::AnimationID, L"Resources/Items/ItemHolder.png", D3DCOLOR_ARGB(0, 0, 0, 0));
	texture->Add(ItemsHolder::AnimationID2, L"Resources/Items/ItemHolder2.png", D3DCOLOR_XRGB(255, 0, 255));
	texture->Add(ItemsHolder::ID_ExitOrb, L"Resources/Items/ExitOrb.png", D3DCOLOR_XRGB(255, 0, 255));
	texture->Add(ItemsHolder::ID_1up, L"Resources/Items/1up.png", D3DCOLOR_XRGB(255, 0, 255));
	texture->Add(ItemsHolder::ID_5Point, L"Resources/Items/5Point.png", D3DCOLOR_XRGB(255, 0, 255));
	texture->Add(ItemsHolder::ID_FullHeal, L"Resources/Items/FullHeal.png", D3DCOLOR_XRGB(255, 0, 255));
	texture->Add(ItemsHolder::ID_HalfHeal, L"Resources/Items/HalfHeal.png", D3DCOLOR_XRGB(255, 0, 255));
	texture->Add(ItemsHolder::ID_HighPoint, L"Resources/Items/HighPoint.png", D3DCOLOR_XRGB(255, 0, 255));
	texture->Add(ItemsHolder::ID_LargeGem, L"Resources/Items/LargeGem.png", D3DCOLOR_XRGB(255, 0, 255));
	texture->Add(ItemsHolder::ID_SmallGem, L"Resources/Items/SmallGem.png", D3DCOLOR_XRGB(255, 0, 255));
	texture->Add(Door::IDDarkDoor, L"Resources/Map/door_dark.png", D3DCOLOR_ARGB(0, 0, 0, 0));
	texture->Add(Door::IDLightDoor, L"Resources/Map/door_light.png", D3DCOLOR_ARGB(0, 0, 0, 0));
	texture->Add(Platform::DisappearPlatformID, L"Resources/DisappearPlatform.png", D3DCOLOR_ARGB(0, 0, 0, 0));
	texture->Add(Platform::MovingPlatformID, L"Resources/MovingPlatform.png", D3DCOLOR_XRGB(255, 0, 255));
	texture->Add(Platform::JetEngineVerticalID, L"Resources/JetEngineVertical.png", D3DCOLOR_XRGB(255, 0, 255));
	texture->Add(Platform::JetEngineHorizontalID, L"Resources/JetEngineHorizontal.png", D3DCOLOR_XRGB(255, 0, 255));
	texture->Add(GrayRocket::ID, L"Resources/Enemy/GrayRobot/GrayRocket.png", D3DCOLOR_XRGB(255, 0, 255));
	texture->Add(ElectricShockwaveBarrier::ID, L"Resources/ElectricShockwaveBarrier.png", D3DCOLOR_XRGB(255, 0, 255));

	SceneManager::GetInstance()->ReplaceScene(new Charleston());
}

int GameManager::GetFPS()
{
	return FPS;
}

void GameManager::Running()
{
	delta += GameTimer::GetInstance()->GetCounter();
	
	if (GetTickCount() - StTime > 1000)
	{
		Debug::PrintOut(L"FPS: %d\n", TotalFrame);
		TotalFrame = 0;
		StTime = GetTickCount();
	}

	if (delta >= TPF)
	{
		Update(delta);
		delta = 0;
	}
	else
	{
		Sleep((DWORD)((TPF - delta)*900));
		delta = TPF;
	}

	Render();
}

void GameManager::Release()
{
	delete Instance;
}

GameManager::~GameManager()
{
	SceneManager::GetInstance()->Release();
	
	Player::GetInstance()->Release();
	GameTimer::GetInstance()->Release();
	d3d::GetInstance()->Release();
	DirectInput::GetInstance()->Release();
	Texture::GetInstance()->Release();
}

void GameManager::Update(float delta)
{
	DirectInput::GetInstance()->KeySnapShot(delta);
	SceneManager::GetInstance()->GetCurrentScene()->Update(delta);
}

void GameManager::Render()
{
	auto d3d = d3d::GetInstance();
	auto d3ddev = d3d->GetDevice();
	auto CurrentScene = SceneManager::GetInstance()->GetCurrentScene();
	d3d->GetDevice()->Clear(0, NULL, D3DCLEAR_TARGET, CurrentScene->GetBackcolor(), 1.0f, 0);
	if (d3ddev->BeginScene())
	{
		// something
		CurrentScene->Draw();
		d3ddev->EndScene();
		//Debug::PrintOut(L"%d\n", ++TotalFrame);
		TotalFrame++;
	}
	d3ddev->Present(NULL, NULL, NULL, NULL);
}
