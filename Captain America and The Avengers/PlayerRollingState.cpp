﻿#include "PlayerRollingState.h"
#include "PlayerIdleState.h"
#include "Framework//Debug.h"
PlayerRollingState::PlayerRollingState()
{
	Player* player = Player::GetInstance();
	player->SetCurrentState(PlayerState::NameState::rolling);
	this->current_state = PlayerState::NameState::rolling;
	player->SetVelocityX(0);
	if (player->IsRolling == false) {
		player->SetVelocityY(VELOCITY_Y);
		player->IsRolling = true;
	}
	player->SetTimeBuffer(0);
	//player->SetJumpDirection(Entity::Entity_Jump_Direction::BotToTop);
	// Khi từ đá chuyển về nhảy thì mới có quyền đá tiếp
	player->time_kicking = 0;
}
PlayerRollingState::~PlayerRollingState()
{

}

void PlayerRollingState::Update(float dt)
{
	Player* player = Player::GetInstance();
	player->GetCurrentAnimation()->Update(dt);
	player->SetVelocityY(player->GetVelocityY() - ROLLING_ACCELERATION);
	if (player->GetVelocityY() <= 0) {
		player->SetJumpDirection(Entity::Entity_Jump_Direction::TopToBot);
	}
	else {
		player->SetJumpDirection(Entity::Entity_Jump_Direction::BotToTop);
	}
	
}

void PlayerRollingState::Draw()
{

}

void PlayerRollingState::Render()
{
}

void PlayerRollingState::HandleInput(float dt)
{
	Player* player = Player::GetInstance();
	auto keyboard = DirectInput::GetInstance();
	
	player->time_air_rolling += dt;
	// Thêm xử lý va chạm cho rolling chứ ko cần
	if (player->IsCollisionWithGround(dt, 6))
	{
		player->ChangeState(new PlayerIdleState());
		return;
	}
	if (player->GetVelocityY() <= -VELOCITY_Y)
	{
		player->ChangeState(new PlayerJumpingDownState());
		return;
	}
	// Đang ở trên không, nếu ấn left thì dịch qua trái
	if (keyboard->KeyPress(LEFT_KEY)) {
		player->SetMoveDirection(Entity::Entity_Direction::RightToLeft);
		player->SetPositionX(player->GetPosition().x - DELTA_JUMP);
	}
	// Đang ở trên không, nếu ấn left thì dịch qua phải
	if (keyboard->KeyPress(RIGHT_KEY)) {
		player->SetMoveDirection(Entity::Entity_Direction::LeftToRight);
		player->SetPositionX(player->GetPosition().x + DELTA_JUMP);
	}
	if (keyboard->KeyDown(ATTACK_KEY)) {
		player->ChangeState(new PlayerKickingState());
		return;
	}
	
}
