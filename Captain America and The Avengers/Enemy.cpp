#include "Enemy.h"
#include "PlayerBeatenState.h"
#include "Shield.h"
#include "BossWizard.h"
Animation * Enemy::GetCurrentAnimation()
{
	return this->current_animation;
}

void Enemy::Update(float dt)
{
	Entity::Update(dt);

}

int Enemy::OnCollision(Entity* obj, float dt)
{
	if (this->IsBeaten == false) 
	{
		if (obj->GetType() == Entity::Entity_Type::player_weapon_type
			&& Collision::getInstance()->IsCollide(this->GetBoundingBox(), obj->GetBoundingBox()))
		{
			if (this->time_beaten <= 0)
			{
				
				if (obj->GetTag() == Entity::Entity_Tag::shield)
				{
					if (Shield::GetInstance()->GetShieldState()->GetCurrentState() == ShieldState::NameState::ShieldDown) 
					{
						this->hp -= Shield::GetInstance()->GetShieldState()->GetDamage();
						this->time_beaten = ENEMY_TIME_BEATEN;
						if (this->GetTag() == Entity::Entity_Tag::boss)
						{
							this->time_beaten = ENEMY_TIME_BEATEN * 3;
						}
						goto CHECK;
					}

					if (Shield::GetInstance()->GetShieldState()->GetCurrentState() == ShieldState::NameState::Nomal)
					{
						goto CHECK2;
					}
					this->hp -= Shield::GetInstance()->GetShieldState()->GetDamage();
					this->time_beaten = ENEMY_TIME_BEATEN;
					if (this->GetTag() == Entity::Entity_Tag::boss) 
					{
						this->time_beaten = ENEMY_TIME_BEATEN * 3;
					}
				}
				else
				{		
					this->hp -= 2;
				}
			}
		}
		if (this->hp <= 0)
		{
			this->IsBeaten = true;
			goto CHECK;
		}
		CHECK2:
		Player *player = Player::GetInstance();
		if (obj->GetType() == Entity::Entity_Type::player_type
			&& player->GetCurrentState() != PlayerState::shield_down
			&& player->GetCurrentState() != PlayerState::die
			&& player->time_invisible <= 0
			&& Collision::getInstance()->IsCollide(this->GetBoundingBox(), obj->GetBoundingBox()))
		{
			if (this->time_beaten == 0) {
				this->time_beaten = ENEMY_TIME_BEATEN;
				if (this->GetTag() == Entity::Entity_Tag::boss) 
				{
					this->time_beaten = ENEMY_TIME_BEATEN * 3;
				}
				this->hp--;			
			}
			if (player->GetCurrentState() != PlayerState::dashing)
			{
				player->ChangeState(new PlayerBeatenState(ENEMY_DAMAGE));
			}
			else {
				this->hp -= 5;
			}
		}	
		if (this->hp <= 0) 
		{
			this->IsBeaten = true;
		}
	}
	CHECK:
	if (this->IsDead)
	{		
		return 1;
	}
	return 0;
}

void Enemy::Spawn()
{
}

Enemy::Enemy(): Entity()
{
	Entity::type = Entity_Type::enemy_type;
	this->explode_ani = new Animation(7, 3); 
	this->explode_ani->SetTime(0.083f, 10000.0f);
	this->jump_direction= Entity::Entity_Jump_Direction::TopToBot;
	this->IsLocking = true;
	this->IsDead = false;
	this->IsBeaten = false;
}

Enemy::~Enemy()
{
}

bool Enemy::IsCollisionWithGround(float dt, int delta_y)
{
	return false;
}

void Enemy::Draw()
{
	if (this->GetMoveDirection())
	{
		this->GetCurrentAnimation()->SetScale(1, 1);
	}
	else
	{
		this->GetCurrentAnimation()->SetScale(-1, 1);
	}
	if (this->IsBeaten)
	{
		this->current_animation->Draw(this->position);
		return;
	}
	if (this->time_beaten == 0) 
	{
		this->current_animation->Draw(this->position);
	}
	else 
	{
		this->time_beaten -= 0.016f;
		if (this->time_beaten <= 0)
		{
			this->time_beaten = 0;
		}
		if ((i++) % 3 == 1) 
		{
			this->current_animation->Draw(this->position);
		}
	}	
}

void Enemy::DrawInt()
{
	if (this->GetMoveDirection()) 
	{
		this->GetCurrentAnimation()->SetScale(1, 1);
	}
	else 
	{
		this->GetCurrentAnimation()->SetScale(-1, 1);
	}
	if (this->IsBeaten) 
	{
		this->current_animation->DrawInt(this->position);
		return;
	}
	if (this->time_beaten == 0) 
	{
		this->current_animation->DrawInt(this->position);
	}
	else 
	{
		this->time_beaten -= 0.016f;
		if (this->time_beaten <= 0)
		{
			this->time_beaten = 0;
		}
		if ((i++) % 3 == 1) 
		{
			this->current_animation->DrawInt(this->position);
		}
	}	
}
