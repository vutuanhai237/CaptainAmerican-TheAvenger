#include "Framework/SoundManager.h"
#include "LaserBullet.h"
#include "SceneManager.h"
#include "PlayerBeatenState.h"
#include "BossWizard.h"

void LaserBullet::Update(float dt)
{
	EnemyWeapon::Update(dt);
	if (this->e != NULL) 
	{
		this->SetPositionY(this->e->GetYFromX(this->GetPosition().x));
	}
	this->distance += abs(this->GetVelocityX()*dt);
	this->IsComeBack = true;
}

int LaserBullet::OnCollision(Entity* obj, float dt)
{
	if (this->IsDead == true) {
		return 1;
	}
	Player *player = Player::GetInstance();

	if (obj->GetType() == Entity::Entity_Type::player_weapon_type
		&& Collision::getInstance()->IsCollide(this->GetBoundingBox(), obj->GetBoundingBox()))
	{
		if (player->time_invisible > 0) 
		{
			if (this->GetMoveDirection() == player->GetMoveDirection())
			{
				return 0;
			}
		}
		if (IsComeBack) 
		{
			this->SetVelocityX(100.0f);
			this->IsComeBack = false;
			e = new Equation(
				this->position,
				D3DXVECTOR2(this->position.x + (this->GetMoveDirection() == Entity::Entity_Direction::LeftToRight ? -1 : 1) * 30, this->position.y + 5));
			if (Player::GetInstance()->GetMoveDirection() == Entity::Entity_Direction::LeftToRight)
			{
				this->SetMoveDirection(Entity::Entity_Direction::LeftToRight);
			}
			if (Player::GetInstance()->GetMoveDirection() == Entity::Entity_Direction::RightToLeft)
			{
				this->SetMoveDirection(Entity::Entity_Direction::RightToLeft);
			}
			SoundManager::GetInstance()->Play(SoundManager::SoundList::shield_collision);
		}
		return 0;
	}
	if (obj->GetType() == Entity::Entity_Type::player_type
		&& player->time_invisible <= 0
		&& Collision::getInstance()->IsCollide(this->GetBoundingBox(), obj->GetBoundingBox()))
	{
		player->ChangeState(new PlayerBeatenState(LASER_BULLET_DAMAGE));
		return 1;
	}
	return 0;
}

void LaserBullet::Draw()
{
	if (BossWizard::GetInstance()->GetMoveDirection())
	{
		this->current_ani->SetScale(1, 1);
	}
	else
	{
		this->current_ani->SetScale(-1, 1);
	}
	this->current_ani->Draw(this->GetPosition());
}

LaserBullet::LaserBullet(D3DXVECTOR2 position, Entity::Entity_Direction direction, FLOAT velocity_y)
{
	//
	this->SetTag(Entity::Entity_Tag::boss_bullet);
	this->SetType(Entity::Entity_Type::enemy_weapon_type);
	this->SetVelocityX(LASER_BULLET_VELOCITY_X);

	// Animation zone
	this->current_ani = new Animation(13, 1);
	if (velocity_y == -1) {
		this->SetJumpDirection(Entity::Entity_Jump_Direction::BotToTop);
		float radian = atan(abs(Player::GetInstance()->GetPosition().y - 54) / (256 - Player::GetInstance()->GetPosition().x));
		this->SetVelocityY(LASER_BULLET_VELOCITY_X*radian);
		this->current_ani->SetRotation(radian);
		goto CHECK;
	}
	if (velocity_y > 10) {
		this->SetJumpDirection(Entity::Entity_Jump_Direction::BotToTop);
		float radian = atan(velocity_y / (BossWizard::GetInstance()->GetPosition().x - Player::GetInstance()->GetPosition().x));
		this->SetVelocityY(LASER_BULLET_VELOCITY_X*radian);
		this->current_ani->SetRotation(radian);
	}
CHECK:
	this->size.cx = 5; this->size.cy = 5;
	this->SetPosition(position);
	this->IsLocking = true;
	this->distance = 0;
	this->IsDead = false;
	this->SetMoveDirection(direction);
	this->e = NULL;
	SoundManager::GetInstance()->Play(SoundManager::SoundList::wizard_laze);
}

LaserBullet::~LaserBullet()
{
}