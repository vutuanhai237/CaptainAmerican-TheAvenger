﻿#include "PlayerRollingState.h"
#include "PlayerIdleState.h"
#include "PlayerShieldDownState.h"
#include "Framework/SoundManager.h"
PlayerRollingState::PlayerRollingState()
{
	Player* player = Player::GetInstance();
	player->SetCurrentState(PlayerState::NameState::rolling);
	this->current_state = PlayerState::NameState::rolling;
	player->SetVelocityX(0);
	player->SetTimeBuffer(0);
	player->IsRolling = true;
	player->IsOnAir = true;
	player->time_kicking = 0;
	player->IsShieldDown = true;
	// Khi từ đá chuyển về nhảy thì mới có quyền đá tiếp
	SoundManager::GetInstance()->Play(SoundManager::SoundList::player_rolling);	
}
PlayerRollingState::~PlayerRollingState()
{

}

void PlayerRollingState::Update(float dt)
{
	Player* player = Player::GetInstance();
	player->GetCurrentAnimation()->Update(dt);
	player->SetVelocityY(player->GetVelocityY() - ROLLING_ACCELERATION);
	if (player->GetVelocityY() <= 0)
	{
		player->SetJumpDirection(Entity::Entity_Jump_Direction::TopToBot);
	}
	else 
	{
		player->SetJumpDirection(Entity::Entity_Jump_Direction::BotToTop);
	}	
}

void PlayerRollingState::Draw()
{

}

BoundingBox PlayerRollingState::GetBoundingBox()
{
	return BoundingBox();
}

void PlayerRollingState::HandleInput(float dt)
{
	Player* player = Player::GetInstance();
	auto keyboard = DirectInput::GetInstance();
	player->time_air_rolling += dt;
	// Thêm xử lý va chạm cho rolling chứ ko cần
	if (player->IsCollisionWithGround(dt, 6) && player->IsLockCollision == false)
	{
		player->ChangeState(new PlayerIdleState());
		player->IsLockCollision = true;
		return;
	}
	if (player->GetVelocityY() <= -VELOCITY_Y || player->time_air_rolling > TIME_ROLLING)
	{
		player->previous_state = 9;
		player->ChangeState(new PlayerJumpingDownState());
		return;
	}
	if (keyboard->KeyPress(DOWN_KEY) && player->time_air_rolling > 0.1f) 
	{
		player->ChangeState(new PlayerShieldDownState());
		return;
	}
	// Đang ở trên không, nếu ấn left thì dịch qua trái
	if (keyboard->KeyPress(LEFT_KEY))
	{
		player->SetMoveDirection(Entity::Entity_Direction::RightToLeft);
		player->SetPositionX(player->GetPosition().x - DELTA_JUMP*dt);
	}
	// Đang ở trên không, nếu ấn left thì dịch qua phải
	if (keyboard->KeyPress(RIGHT_KEY))
	{
		player->SetMoveDirection(Entity::Entity_Direction::LeftToRight);
		player->SetPositionX(player->GetPosition().x + DELTA_JUMP*dt);
	}
	if (keyboard->KeyDown(ATTACK_KEY))
	{
		player->ChangeState(new PlayerKickingState());
		return;
	}
}