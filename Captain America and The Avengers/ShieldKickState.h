#pragma once
#include "ShieldState.h"

#define SHIELD_NOMAL_KICKING_DELTA_X -16.5f
#define SHIELD_NOMAL_KICKING_DELTA_Y 0.0f

class ShieldKickState : public ShieldState
{
public:
	ShieldKickState();
	virtual int GetDamage() override;
	void Update(float dt) override;
	void Draw() override;
};