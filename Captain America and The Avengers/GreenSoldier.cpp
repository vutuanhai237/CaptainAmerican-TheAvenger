﻿#include "GreenSoldier.h"
#include "Shield.h"
#include "FrameWork/SoundManager.h"

void GreenSoldier::Update(float dt)
{
	Enemy::Update(dt);
	this->current_animation->Update(dt);
	if (this->IsExplode) 
	{
		this->time_explode += dt;
		this->current_state = GreenSoldierState::none;
		this->current_animation = explode_ani;
		this->SetVelocityX(0.0f);
		if (this->time_explode >= TIME_EXPLODE)
		{
			this->IsDead = true;
			this->IsActive = false;
		}
		return;
	}
	if (this->IsBeaten) 
	{
		this->current_state = GreenSoldierState::beaten;
		this->current_animation = beaten_ani;
		this->SetVelocityX(0.0f);
		this->time_beaten += dt;
		if (this->time_beaten >= TIME_BEATEN)
		{
			this->IsExplode = true;
			SoundManager::GetInstance()->Play(SoundManager::SoundList::entity_explode);
		}
		if (this->GetMoveDirection() == Entity::Entity_Direction::LeftToRight) 
		{
			this->position.x -= 1;
		}
		else 
		{
			this->position.x += 1;
		}
		return;
	}

	switch (this->level)
	{
	case Level::very_stupid:
		this->UpdateVeryStupidLevel(dt);
		break;
	case Level::stupid:
		this->UpdateStupidLevel(dt);
		break;
	case Level::normal:
		this->UpdateNormalLevel(dt);
		break;
	case Level::clever:
		this->UpdateCleverLevel(dt);
		break;
	case Level::master:
		this->UpdateMasterLevel(dt);
		break;
	}
}

void GreenSoldier::UpdateVeryStupidLevel(float dt)
{
	// Đang ở trên không
	if (IsChamDatLanDau == false)
	{
		if (this->IsCollisionWithGround(dt))
		{
			this->SetVelocityY(0.0f);
			this->position_spawn = this->position;
			IsChamDatLanDau = true;
		}
		else
		{
			this->SetVelocityY(GREEN_SOLDIER_VELOCITY_Y);

		}
		return;
	}
	// Quay mặt
	if ((Player::GetInstance()->GetPosition().x - this->GetPosition().x > 0)) 
	{
		this->SetMoveDirection(Entity::Entity_Direction::LeftToRight);
	}
	else
	{
		this->SetMoveDirection(Entity::Entity_Direction::RightToLeft);
	}
	// đã ở trên mặt đất
	if (this->current_state == GreenSoldierState::running) 
	{
		this->time_idle = 0.0f;
		this->time_ducking = 0.0f;
	}
	// Thực hiện động tác 0.5s ngồi một lần 
	if (this->IsIdle && this->current_state == GreenSoldierState::idle) 
	{
		this->time_idle += dt;
		if (this->IsFire && this->time_idle > 0.016 * 5)
		{
			if (this->GetMoveDirection() == Entity::Entity_Direction::LeftToRight) 
			{
				SceneManager::GetInstance()->GetCurrentScene()->GetCurrentGrid()->AddObject2Cell(
					new Bullet(
						D3DXVECTOR2((FLOAT)(this->position.x + 11.5), (FLOAT)(this->position.y + 17.5)),
						this->GetMoveDirection()
					)
				);
			}
			else
			{
				SceneManager::GetInstance()->GetCurrentScene()->GetCurrentGrid()->AddObject2Cell(
					new Bullet(
						D3DXVECTOR2((FLOAT)(this->position.x - 11.5), (FLOAT)(this->position.y + 17.5)),
						this->GetMoveDirection()
					)
				);
			}
			this->IsFire = false;
		}
	}
	if (this->IsDucking && this->current_state == GreenSoldierState::ducking) 
	{
		this->time_ducking += dt;
	}
	if (this->time_idle >= GREEN_SOLDIER_TIME_IDLE*4 && this->current_state == GreenSoldierState::idle)
	{
		this->IsIdle = false;
		this->IsDucking = true;
		this->current_animation = this->ducking_ani;
		this->current_state = GreenSoldierState::ducking;
		this->time_idle = 0.0f;
	}
	if (this->time_ducking >= GREEN_SOLDIER_TIME_DUCKING*4 && this->current_state == GreenSoldierState::ducking)
	{
		this->IsDucking = false;
		this->IsIdle = true;
		this->current_animation = this->idle_ani;
		this->current_state = GreenSoldierState::idle;
		this->time_ducking = 0.0f;
		this->IsFire = true;
	}
}

void GreenSoldier::UpdateStupidLevel(float dt)
{
	// Đang ở trên không
	if (IsChamDatLanDau == false)
	{
		if (this->IsCollisionWithGround(dt)) {
			this->SetVelocityY(0.0f);
			this->position_spawn = this->position;
			IsChamDatLanDau = true;
		}
		else {
			this->SetVelocityY(GREEN_SOLDIER_VELOCITY_Y);
		}
		return;
	}
	// Quay mặt
	if ((Player::GetInstance()->GetPosition().x - this->GetPosition().x > 0)) 
	{
		this->SetMoveDirection(Entity::Entity_Direction::LeftToRight);
	}
	else 
	{
		this->SetMoveDirection(Entity::Entity_Direction::RightToLeft);
	}
	// đã ở trên mặt đất
	if (this->current_state == GreenSoldierState::running)
	{
		this->time_idle = 0.0f;
		this->time_ducking = 0.0f;
	}
	// Thực hiện động tác 0.5s ngồi một lần 
	if (this->IsIdle && this->current_state == GreenSoldierState::idle)
	{
		this->time_idle += dt;
		if (this->IsFire && this->time_idle > 0.016 * 5)
		{
			if (this->GetMoveDirection() == Entity::Entity_Direction::LeftToRight)
			{
				SceneManager::GetInstance()->GetCurrentScene()->GetCurrentGrid()->AddObject2Cell(
					new Bullet(
						D3DXVECTOR2((FLOAT)(this->position.x + 11.5), (FLOAT)(this->position.y + 17.5)),
						this->GetMoveDirection()
					)
				);
			}
			else 
			{
				SceneManager::GetInstance()->GetCurrentScene()->GetCurrentGrid()->AddObject2Cell(
					new Bullet(
						D3DXVECTOR2((FLOAT)(this->position.x - 11.5), (FLOAT)(this->position.y + 17.5)),
						this->GetMoveDirection()
					)
				);
			}
			this->IsFire = false;
		}
	}
	if (this->IsDucking && this->current_state == GreenSoldierState::ducking)
	{
		this->time_ducking += dt;
	}
	if (this->time_idle >= GREEN_SOLDIER_TIME_IDLE && this->current_state == GreenSoldierState::idle) 
	{
		this->IsIdle = false;
		this->IsDucking = true;
		this->current_animation = this->ducking_ani;
		this->current_state = GreenSoldierState::ducking;
		this->time_idle = 0.0f;
	}
	if (this->time_ducking >= GREEN_SOLDIER_TIME_DUCKING && this->current_state == GreenSoldierState::ducking)
	{
		this->IsDucking = false;
		this->IsIdle = true;
		this->current_animation = this->idle_ani;
		this->current_state = GreenSoldierState::idle;
		this->time_ducking = 0.0f;
		this->IsFire = true;
	}
}

void GreenSoldier::UpdateNormalLevel(float dt)
{
	this->SetVelocityX(GREEN_SOLDIER_VELOCITY_X * 2);
	// Đang ở trên không
	if (IsChamDatLanDau == false)
	{
		if (this->IsCollisionWithGround(dt)) 
		{
			this->SetVelocityY(0.0f);
			this->position_spawn = this->position;
			IsChamDatLanDau = true;
		}
		else 
		{
			this->SetVelocityY(GREEN_SOLDIER_VELOCITY_Y);
		}
		return;
	}
	// Cập nhật tọa độ khi đang nhảy
	if (this->current_state == GreenSoldierState::jumping) 
	{
		if (this->IsCollisionWithGround(dt) && this->IsJumpingFirst > 5)
		{
			this->current_state = GreenSoldierState::running;
			this->current_animation = running_ani;
			return;
		}
		this->IsJumpingFirst++;
		this->SetVelocityX(GREEN_SOLDIER_VELOCITY_X);
		this->SetPositionY(e->GetYFromX(this->GetPosition().x));
		return;
	}
	// Bắt việc shield có được ném hay không, thì nhảy để né 
	if (Shield::GetInstance()->GetShieldState()->GetCurrentState() == ShieldState::NameState::ShieldAttack
		&& this->IsJumping == false
		&& this->current_state == GreenSoldierState::running
		&& (this->GetMoveDirection() != Player::GetInstance()->GetMoveDirection())) {
		this->IsJumping = true;
		this->current_state = GreenSoldierState::jumping;
		this->current_animation = ducking_ani;
		int move = 0;
		if (this->position.x - this->position_goto.x > 0) 
		{
			move = -1;
		}
		else {
			move = 1;
		}
		e = new Equation(
			this->position,
			D3DXVECTOR2(this->position.x + move * 50, this->position.y));
		return;
	}
}

void GreenSoldier::UpdateCleverLevel(float dt)
{
	// Đang ở trên không
	if (IsChamDatLanDau == false)
	{
		if (this->IsCollisionWithGround(dt)) {
			this->SetVelocityY(0.0f);
			this->position_spawn = this->position;
			IsChamDatLanDau = true;
		}
		else {
			this->SetVelocityY(GREEN_SOLDIER_VELOCITY_Y);
		}
		return;
	}
	// Quay mặt
	if ((Player::GetInstance()->GetPosition().x - this->GetPosition().x > 0)) 
	{
		this->SetMoveDirection(Entity::Entity_Direction::LeftToRight);
	}
	else
	{
		this->SetMoveDirection(Entity::Entity_Direction::RightToLeft);
	}
	// đã ở trên mặt đất
	if (this->current_state == GreenSoldierState::running) 
	{
		this->time_idle = 0.0f;
		this->time_ducking = 0.0f;
	}
	// Thực hiện động tác 0.5s ngồi một lần 
	if (this->IsIdle && this->current_state == GreenSoldierState::idle)
	{
		this->time_idle += dt;
		if (this->IsFire && this->time_idle > 0.016 * 5)
		{
			if (this->GetMoveDirection() == Entity::Entity_Direction::LeftToRight) 
			{
				SceneManager::GetInstance()->GetCurrentScene()->GetCurrentGrid()->AddObject2Cell(
					new Bullet(
						D3DXVECTOR2((FLOAT)(this->position.x + 11.5), (FLOAT)(this->position.y + 17.5)),
						this->GetMoveDirection()
					)
				);
				SceneManager::GetInstance()->GetCurrentScene()->GetCurrentGrid()->AddObject2Cell(
					new Bullet(
						D3DXVECTOR2((FLOAT)(this->position.x + 11.5), (FLOAT)(this->position.y + 17.5)),
						this->GetMoveDirection(),
						-135
					)
				);
				SceneManager::GetInstance()->GetCurrentScene()->GetCurrentGrid()->AddObject2Cell(
					new Bullet(
						D3DXVECTOR2((FLOAT)(this->position.x + 11.5), (FLOAT)(this->position.y + 17.5)),
						this->GetMoveDirection(),
						135
					)
				);
			}
			else {
				SceneManager::GetInstance()->GetCurrentScene()->GetCurrentGrid()->AddObject2Cell(
					new Bullet(
						D3DXVECTOR2((FLOAT)(this->position.x - 11.5), (FLOAT)(this->position.y + 17.5)),
						this->GetMoveDirection()
					)
				);
				SceneManager::GetInstance()->GetCurrentScene()->GetCurrentGrid()->AddObject2Cell(
					new Bullet(
						D3DXVECTOR2((FLOAT)(this->position.x - 11.5), (FLOAT)(this->position.y + 17.5)),
						this->GetMoveDirection(),
						135
					)
				);
				SceneManager::GetInstance()->GetCurrentScene()->GetCurrentGrid()->AddObject2Cell(
					new Bullet(
						D3DXVECTOR2((FLOAT)(this->position.x - 11.5), (FLOAT)(this->position.y + 17.5)),
						this->GetMoveDirection(),
						-135
					)
				);

			}
			this->IsFire = false;
		}

	}

	if (this->IsDucking && this->current_state == GreenSoldierState::ducking) 
	{
		this->time_ducking += dt;
	}

	if (this->time_idle >= GREEN_SOLDIER_TIME_IDLE * 3 && this->current_state == GreenSoldierState::idle)
	{
		this->IsIdle = false;
		this->IsDucking = true;
		this->current_animation = this->ducking_ani;
		this->current_state = GreenSoldierState::ducking;
		this->time_idle = 0.0f;
	}

	if (this->time_ducking >= GREEN_SOLDIER_TIME_DUCKING * 3 && this->current_state == GreenSoldierState::ducking)
	{
		this->IsDucking = false;
		this->IsIdle = true;
		this->current_animation = this->idle_ani;
		this->current_state = GreenSoldierState::idle;
		this->time_ducking = 0.0f;
		this->IsFire = true;
	}
}

void GreenSoldier::UpdateMasterLevel(float dt)
{
	// Đang ở trên không
	if (this->position.x <= 10) {
		this->SetVelocityX(0.0f);
		return;
	}
	if (IsChamDatLanDau == false)
	{
		if (this->IsCollisionWithGround(dt)) 
		{
			this->SetVelocityY(0.0f);
			this->position_spawn = this->position;
			IsChamDatLanDau = true;
		}
		else
		{
			this->SetVelocityY(GREEN_SOLDIER_VELOCITY_Y);
		}
		return;
	}
	// Cập nhật tọa độ khi đang nhảy
	if (this->current_state == GreenSoldierState::jumping)
	{
		if (this->IsCollisionWithGround(dt) && this->IsJumpingFirst > 5) 
		{
			this->current_state = GreenSoldierState::running;
			this->current_animation = running_ani;
			return;
		}
		this->IsJumpingFirst++;
		this->SetVelocityX(GREEN_SOLDIER_VELOCITY_X);
		this->SetPositionY(e->GetYFromX(this->GetPosition().x));
		return;
	}
	// Bắt việc shield có được ném hay không, thì nhảy để né 
	if (Shield::GetInstance()->GetShieldState()->GetCurrentState() == ShieldState::NameState::ShieldAttack
		&& this->IsJumping == false
		&& this->current_state == GreenSoldierState::running
		&& (this->GetMoveDirection() != Player::GetInstance()->GetMoveDirection()))
	{
		this->IsJumping = true;
		this->current_state = GreenSoldierState::jumping;
		this->current_animation = ducking_ani;
		e = new Equation(
			this->position,
			D3DXVECTOR2(this->position.x + (this->GetMoveDirection() == Entity::Entity_Direction::LeftToRight ? 1 : -1) * 50, this->position.y));
		return;
	}
	// Thực hiện động tác 1s đứng một lần 
	if (this->IsIdle && this->current_state == GreenSoldierState::idle) 
	{
		this->time_idle += dt;
		if (this->IsFire && this->time_idle > 0.016 * 5)
		{
			if (this->GetMoveDirection() == Entity::Entity_Direction::LeftToRight) 
			{
				SceneManager::GetInstance()->GetCurrentScene()->GetCurrentGrid()->AddObject2Cell(
					new Bullet(
						D3DXVECTOR2((FLOAT)(this->position.x + 11.5), (FLOAT)(this->position.y + 17.5)),
						this->GetMoveDirection()
					)
				);
			}
			else
			{
				SceneManager::GetInstance()->GetCurrentScene()->GetCurrentGrid()->AddObject2Cell(
					new Bullet(
						D3DXVECTOR2((FLOAT)(this->position.x - 11.5), (FLOAT)(this->position.y + 17.5)),
						this->GetMoveDirection()
					)
				);
			}
			this->IsFire = false;
		}
	}

	if (this->IsRunning && this->current_state == GreenSoldierState::running) 
	{
		this->time_running += dt;
	}
	if (this->time_idle >= GREEN_SOLDIER_TIME_IDLE_TO_SHOT && this->current_state == GreenSoldierState::idle) 
	{
		this->IsIdle = false;
		this->IsRunning = true;
		this->current_animation = this->running_ani;
		this->current_state = GreenSoldierState::running;
		this->time_idle = 0.0f;
		this->SetVelocityX(GREEN_SOLDIER_VELOCITY_X);
	}

	if (this->time_running >= GREEN_SOLDIER_TIME_RUNNING_TO_SHOT && this->current_state == GreenSoldierState::running) 
	{
		this->IsRunning = false;
		this->IsIdle = true;
		this->current_animation = this->idle_ani;
		this->current_state = GreenSoldierState::idle;
		this->time_running = 0.0f;
		this->IsFire = true;
		this->SetVelocityX(0.0f);
	}
}

int GreenSoldier::OnCollision(Entity* obj, float dt)
{
	if (Camera::GetInstance()->GetCameraFreeze() == true) 
	{
		if (Player::GetInstance()->IsBornSoldier == false) 
		{
			if (this->hp <= 0)
			{
				Player::GetInstance()->number_soldier += 1;
				Player::GetInstance()->IsBornSoldier = true;
				this->IsExplode = true;
				SoundManager::GetInstance()->Play(SoundManager::SoundList::entity_explode);
			}
			if (this->GetPosition().x >= 515 && this->GetPosition().y <= 120)
			{
				Player::GetInstance()->IsBornSoldier = true;
			}
		}		
	}
	return Enemy::OnCollision(obj, dt);
}

void GreenSoldier::Draw()
{
	Enemy::Draw();
}

BoundingBox GreenSoldier::GetBoundingBox()
{
	SIZE size;
	size.cx = 24;
	size.cy = 46;
	return BoundingBox(Entity::position, size, Entity::velocity.x, Entity::velocity.y);
}

bool GreenSoldier::IsCollisionWithGround(float dt, int delta_y)
{
	SIZE FootSize;
	FootSize.cx = GREEN_SOLDIER_SIZE_WIDTH;
	FootSize.cy = GREEN_SOLDIER_FOOT_HEIGHT;
	BoundingBox foot(D3DXVECTOR2(position.x, position.y - delta_y), FootSize, velocity.x*dt, velocity.y*dt);
	auto Checker = Collision::getInstance();
	vector<Entity*> obj = *SceneManager::GetInstance()->GetCurrentScene()->GetCurrentMap()->GetMapObj();
	CollisionOut tmp;
	BoundingBox box2;
	for (auto item : obj)
	{
		box2 = BoundingBox(item->GetPosition(), item->GetSize(), 0, 0);
		tmp = Checker->SweptAABB(foot, box2);
		if (tmp.side == CollisionSide::bottom)
		{
			position.y = item->GetPosition().y + (GREEN_SOLDIER_SIZE_HEIGHT + item->GetSize().cy) / 2 - 4;
			return true;
		}
	}
	return false;
}

GreenSoldier::GreenSoldier(int level, D3DXVECTOR2 position_spawn, int direction) : Enemy()
{
	// init animation zone
	this->SetTag(Entity::Entity_Tag::green_soldier);
	this->SetType(Entity::Entity_Type::enemy_type);
	this->idle_ani = new Animation(GreenSoldierState::idle, 1);
	this->running_ani = new Animation(GreenSoldierState::running, 3);
	this->ducking_ani = new Animation(GreenSoldierState::ducking, 1);
	this->beaten_ani = new Animation(GreenSoldierState::beaten, 1);
	// set animation zone
	this->idle_ani->SetTime(0.5f);
	this->running_ani->SetTime(0.1f);
	this->ducking_ani->SetTime(0.5f);
	// set properties zone
	this->position = position_spawn;
	this->current_animation = idle_ani;
	this->hp = GREEN_SOLDIER_HP;
	if (direction == 0) {
		this->SetMoveDirection(Entity::Entity_Direction::LeftToRight);
	}
	else {
		this->SetMoveDirection(Entity::Entity_Direction::RightToLeft);
	}
	// time zone
	this->time_idle = 0.0f;
	this->time_ducking = 0.0f;
	this->time_running = 0.0f;
	// flag zone
	this->IsIdle = false;
	this->IsDucking = false;
	this->IsRunning = false;
	this->IsChamDatLanDau = false;
	this->IsFire = false;
	// handle follow level zone
	switch (level) {
	case -1:
		this->level = Level::very_stupid;
		break;
	case 0:
		this->level = Level::stupid;
		break;
	case 1:
		this->level = Level::normal;
		break;
	case 2:
		this->level = Level::clever;
		break;
	case 3:
		this->level = Level::master;
		break;
	}
	switch (this->level) {
	case Level::very_stupid:
		this->IsIdle = true;
		this->current_state = GreenSoldierState::idle;
		this->current_animation = idle_ani;
		this->SetVelocityX(0.0f);
		break;
	case Level::stupid:
		this->IsIdle = true;
		this->current_state = GreenSoldierState::idle;
		this->current_animation = idle_ani;
		this->SetVelocityX(0.0f);
		break;
	case Level::normal:
		this->current_state = GreenSoldierState::running;
		this->current_animation = running_ani;
		this->SetVelocityX(GREEN_SOLDIER_VELOCITY_X);
		break;
	case Level::clever:
		this->IsIdle = true;
		this->current_state = GreenSoldierState::idle;
		this->current_animation = idle_ani;
		this->SetVelocityX(0.0f);
		break;
	case Level::master:
		this->IsJumpingFirst = 0;
		this->current_animation = running_ani;
		this->current_state = GreenSoldierState::running;
		this->SetVelocityX(GREEN_SOLDIER_VELOCITY_X);
		this->IsRunning = true;
		break;
	}
}

GreenSoldier::~GreenSoldier()
{
}