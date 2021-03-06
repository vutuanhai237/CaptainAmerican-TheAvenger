#pragma once
#include "BossWizardState.h"
#include "BossWizard.h"

class BossWizardFireState : public BossWizardState
{
public:
	float time_fire;
	float time_firing;
	float time_wait_before_fire_energy_bullet;
	int count_bullet;
	int max_laser_bullet;
	BossWizardFireState(int max_laser_bullet);
	~BossWizardFireState();
	virtual void Update(float dt);
	virtual void Draw();
	BoundingBox GetBoundingBox() override;
	virtual void HandleInput(float dt);
};

