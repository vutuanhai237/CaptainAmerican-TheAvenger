﻿#include "BossWizardIdleRoad.h"
#include "BossWizardUMaxRoad.h"
#include "BossWizardRunningState.h"
#include "BossWizardPunchingState.h"
#include "BossWizardFireRoad.h"
#include "BossWizard.h"
void BossWizardIdleRoad::Update(float dt)
{
	BossWizard* boss = BossWizard::GetInstance();
	if (this->GetOneTime == false) {
		// tạo ngoại cảnh
		boss->SetVelocity(0, BOSS_WIZARD_VELOCITY_Y);
		boss->SetJumpDirection(Entity::Entity_Jump_Direction::TopToBot);
		boss->ChangeState(new BossWizardIdleState());
		if (boss->GetPosition().x > Player::GetInstance()->GetPosition().x) {
			boss->SetMoveDirection(Entity::Entity_Direction::RightToLeft);
		}
		else {
			boss->SetMoveDirection(Entity::Entity_Direction::LeftToRight);
		}
		this->GetOneTime = true;
	}
	// xử lý nội cảnh
	// nếu cả 2 ko ở cùng độ cao thì thằng này chỉ biết đứng cười
	if (abs(boss->GetPosition().y - Player::GetInstance()->GetPosition().y) > 10.0f
		&& IsChamDatLanDau) {
		if (boss->GetPosition().x > Player::GetInstance()->GetPosition().x) {
			boss->SetMoveDirection(Entity::Entity_Direction::RightToLeft);
		}
		else {
			boss->SetMoveDirection(Entity::Entity_Direction::LeftToRight);
		}
		return;
	}
	// Ở gần thì đấm
	if (abs(boss->GetPosition().x - Player::GetInstance()->GetPosition().x) < 20.0f
		&& IsChamDatLanDau) {
		boss->ChangeState(new BossWizardPunchingState());
		return;
	}
	// Chạy ngắn
	if (this->IsRunning && IsChamDatLanDau) {
		this->time_running += dt;
		boss->SetPositionX(boss->GetPosition().x + 3);
		if (this->time_running >= 1.0f) {
			this->IsRunning = false;
		
		}
	}
	this->time_idle += dt;
	if (this->time_idle >= 1.0f) {
		if (this->IsRunning == false && this->time_running <= 1.0f) {
			if (boss->GetPosition().x > Player::GetInstance()->GetPosition().x) {
				boss->SetMoveDirection(Entity::Entity_Direction::RightToLeft);
			}
			else {
				boss->SetMoveDirection(Entity::Entity_Direction::LeftToRight);
			}
			boss->ChangeRoad(new BossWizardFireRoad());
			return;
		}
	
		return;
	}

	// Bay xa
	if (boss->IsCollisionWithWall(dt)) {
		this->IsChamDatLanDau = true;
		boss->SetVelocity(0, 0);
	/*	if (((0 <= boss->GetPosition().x <= 48) || (208 <= boss->GetPosition().x <= 256))
			&& boss->IsUMax == false) {
			boss->ChangeRoad(new BossWizardUMaxRoad());
			return;
		}*/
		return;
	}
}

BossWizardIdleRoad::BossWizardIdleRoad()
{
	this->GetOneTime = false;
	BossWizard* boss = BossWizard::GetInstance();
	boss->SetCurrentRoad(BossWizardRoad::RoadType::idle);
	this->current_road = BossWizardRoad::RoadType::idle;
	this->time_idle = 0.0f;
	this->phase = 1;
	this->IsChamDatLanDau = false;
	this->time_running = 0.0f;
	this->IsRunning = false;
}


BossWizardIdleRoad::~BossWizardIdleRoad()
{
}
