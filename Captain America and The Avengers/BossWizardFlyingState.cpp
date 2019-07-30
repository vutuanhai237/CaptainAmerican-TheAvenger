﻿#include "BossWizardIdleState.h"
#include "BossWizardFlyingState.h"
#include "BossWizardRunningState.h"
#include "BossWizardFireState.h"
#include "BossWizardFlyingFireState.h"
#include "BossWizardRoad.h"
BossWizardFlyingState::BossWizardFlyingState()
{
	BossWizard* boss = BossWizard::GetInstance();
	boss->SetCurrentState(BossWizardState::NameState::flying);
	this->current_state = BossWizardState::NameState::flying;
	boss->SetSize(20, 45);
	
}


BossWizardFlyingState::~BossWizardFlyingState()
{
}

void BossWizardFlyingState::Update(float dt)
{
	BossWizard* boss = BossWizard::GetInstance();
	// Thay đổi animation thành bắn và new đạn tại đây
	if (boss->GetJumpDirection() == Entity::Entity_Jump_Direction::TopToBot) {
		boss->GetCurrentAnimation()->SetFrame(2);
		boss->GetCurrentAnimation()->Stop();
	}
	else {
		boss->GetCurrentAnimation()->SetFrame(1);
		boss->GetCurrentAnimation()->Stop();
	}
}

void BossWizardFlyingState::Draw()
{
}

BoundingBox BossWizardFlyingState::GetBoundingBox()
{
	return BoundingBox();
}

void BossWizardFlyingState::HandleInput(float dt)
{
}
