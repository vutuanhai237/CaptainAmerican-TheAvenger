#pragma once
#include "EnemyWeapon.h"
#define RED_ROCKET_VELOCITY_X 200.0f
#define DISTANCE_OUT 400.0f
#define TIME_OUT_STRAIGHT 0.016*15
#define RED_ROCKET_DAMAGE 1

class RedRocket : public EnemyWeapon
{
public:
	enum RedRocketState {
		crossed = 5,
		horizontal = 6,
		explode = 7
	};
	virtual void Update(float dt) override;
	virtual int OnCollision(Entity*, float dt) override;
	virtual void Exploding(float dt);
	virtual void Release();
	virtual void Draw();
	RedRocket(D3DXVECTOR2, bool IsCrossed, Entity::Entity_Direction direction);
	~RedRocket() override;
	float distance;
	float time_out_explode;
	float time_out_straight;
	bool IsCrossed;
	bool IsDead;
	bool IsStraight;
	bool IsExploding;
	Animation* current_ani;
protected:
	Animation* crossed_ani;
	Animation* horizontal_ani;
};

