#pragma once
#include "PlayerState.h"
#include "Player.h"
#include "PlayerShieldUpState.h"
class PlayerRunningState: public PlayerState
{
public:
	PlayerRunningState();
	~PlayerRunningState();
	virtual void Update(float dt);
	virtual void Draw();
	BoundingBox GetBoundingBox() override;
	virtual void HandleInput(float dt);
};

