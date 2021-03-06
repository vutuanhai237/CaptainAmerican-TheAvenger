#pragma once
#include "PlayerState.h"
#include "Player.h"
class PlayerRollingState : public PlayerState
{
public:
	PlayerRollingState();
	~PlayerRollingState();
	virtual void Update(float dt);
	virtual void Draw();
	BoundingBox GetBoundingBox() override;
	virtual void HandleInput(float dt);
};

