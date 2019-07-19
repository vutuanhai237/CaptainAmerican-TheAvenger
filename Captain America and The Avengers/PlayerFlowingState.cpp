﻿#include "PlayerFlowingState.h"
#include "PlayerDivingState.h"
#include "Framework//Debug.h"
PlayerFlowingState::PlayerFlowingState()
{
	Player* player = Player::GetInstance();
	player->SetCurrentState(PlayerState::NameState::flowing);

	this->current_state = PlayerState::NameState::flowing;
	player->SetTimeBuffer(0);
	player->SetPositionY(34.0f);
	player->SetVelocity(0, 0);

}
PlayerFlowingState::~PlayerFlowingState()
{

}

void PlayerFlowingState::Update(float dt)
{
	Player* player = Player::GetInstance();
	player->GetCurrentAnimation()->Update(dt);
	player->SetPositionX(player->GetPosition().x - VELOCITY_X / 2 * dt);

}

void PlayerFlowingState::Draw()
{

}

void PlayerFlowingState::Render()
{
}

void PlayerFlowingState::HandleInput(float dt)
{
	Player* player = Player::GetInstance();
	auto keyboard = DirectInput::GetInstance();
	
	if (keyboard->KeyDown(JUMP_KEY)) {
		player->ChangeState(new PlayerJumpingState());
		return;
	}
	if (keyboard->KeyPress(RIGHT_KEY) && keyboard->KeyPress(DOWN_KEY)) {
		player->SetMoveDirection(Entity::Entity_Direction::LeftToRight);
		player->SetPositionX(player->GetPosition().x + DELTA_JUMP * dt);
		player->GetCurrentAnimation()->Continue();

		return;
	}
	if (keyboard->KeyPress(LEFT_KEY) && keyboard->KeyPress(DOWN_KEY)) {
		player->SetMoveDirection(Entity::Entity_Direction::RightToLeft);
		player->SetPositionX(player->GetPosition().x - DELTA_JUMP * dt);
		player->GetCurrentAnimation()->Continue();

		return;
	}
	if (keyboard->KeyPress(RIGHT_KEY)) {
		player->SetMoveDirection(Entity::Entity_Direction::LeftToRight);
		player->SetPositionX(player->GetPosition().x + DELTA_JUMP * dt);
		player->GetCurrentAnimation()->Continue();

		return;
	}
	if (keyboard->KeyPress(LEFT_KEY)) {
		player->SetMoveDirection(Entity::Entity_Direction::RightToLeft);
		player->SetPositionX(player->GetPosition().x - DELTA_JUMP * dt);
		player->GetCurrentAnimation()->Continue();

		return;
	}
	if (keyboard->KeyPress(DOWN_KEY)) {
		player->SetCurrentAnimation(player->GetAnimation(PlayerState::NameState::diving));
		player->GetCurrentAnimation()->SetFrame(1);
		player->ChangeState(new PlayerDivingState());
		return;
	}
	
	player->GetCurrentAnimation()->SetFrame(1);
	player->GetCurrentAnimation()->Stop();
}
