#pragma once
#include "PlayerState.h"
#include "Player.h"
#include "PlayerDuckingState.h"
#include "Framework//DirectInput.h"
class PlayerDuckingPunchingState : public PlayerState
{
public:
	PlayerDuckingPunchingState();
	~PlayerDuckingPunchingState();
	virtual void Update(float dt);
	virtual void Draw();
	BoundingBox GetBoundingBox() override;
	virtual void HandleInput(float dt);
	float time_punch;
	bool IsGong;
};

