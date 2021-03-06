﻿#pragma once
#include "Entity.h"
#include "PlayerState.h"
#include <map>
#include "Collision.h"
// Player HP = 8
#define PLAYER_HP 8
#define PLAYER_SIZE_WIDTH 8
#define PLAYER_SIZE_HEIGHT 40
#define PLAYER_FOOT_HEIGHT 8
#define PLAYER_ARM_HEIGHT 8
//
#define VELOCITY_X 80.0f
#define VELOCITY_Y 160.0f
// jumping
#define DELTA_JUMP VELOCITY_X
#define DISTANCE_JUMPING 90.0f
#define TIME_AIR 0.40f
#define TIME_JUMPING 0.30f
#define TIME_ROLLING 0.3f
#define TIME_KICKING 0.2f
#define JUMPING_ACCELERATION 20.0F
#define JUMPING_VELOCITY_BEGIN 300.0f
#define ROLLING_ACCELERATION 11.4f
// Dashing
#define TIME_DASHING 0.016f * 30
#define TIME_DUCK_BEFORE_DASHING  0.016f * 1
#define TIME_WAIT_DASHING 0.016f * 1
// Throwing
#define TIME_THROWING 0.016 * 14
#define TIME_WAIT_THROWING 0.016f * 4
// Punching
#define TIME_PUNCHING 0.016 * 13
#define TIME_WAIT_PUNCHING 0.016f * 4
// Ducking - Punching
#define TIME_DUCKING_PUNCHING 0.016f*10
#define TIME_WAITING_DUCKING_PUNCHING 0.016f*4
// Ducking
#define TIME_DUCKING_BEFORE_IDLE 0.016f*5
// flowing
#define TIME_JUMPING_DOWN_BEFORE_FLOWING 0.016f*23
#define TIME_ANIMATION_BEFORE_FLOWING 0.5f
// diving
// don tho
#define TIME_DON_THO 0.016f*10
// beaten
#define TIME_BEATEN_MIN 0.016f*8
#define TIME_INVISIBLE 2.0f
#define DAMAGE_SPIKE 2
// hang on
#define TIME_JUMP_FROM_ROPE 0.016f * 8
// die
#define TIME_GUC_BEFORE_NAM 0.016f * 10
#define TIME_DIE 2.0f
// shock
#define TIME_SHOCKING 0.016f*8
using namespace std;
class Player : public Entity
{
public:
	static Player* GetInstance();
	void Release();
	void Update(float dt);
	void Draw();
	void HandleInput(float dt);
	void Init();
	// get
	PlayerState::NameState GetCurrentState();
	PlayerState* GetPlayerState();
	Animation* GetCurrentAnimation();
	Animation* GetAnimation(PlayerState::NameState state);
	D3DXVECTOR2 GetPositionIdle();
	Entity::Entity_Direction previous_direction;
	int GetPreviousState();
	bool GetIsDuckingPunching();
	bool GetIsThrowing();
	bool GetIsRolling();
	float GetTimeBuffer();
	void AddTimeBuffer(float dt);
	// set
	void ChangeState(PlayerState* new_state);
	void SetCurrentState(PlayerState::NameState new_state);
	void SetCurrentAnimation(Animation* animation);
	void SetTimeBuffer(float dt);
	void SetIsRolling(bool IsRolling);
	void SetIsThrowing(bool IsThrowing);
	void SetIsDuckingPunching(bool IsDuckingpunching);
	// collision
	virtual int OnCollision(Entity *, float dt);
	bool IsCollisionWithGround(float dt, int delta_y = 12);
	bool IsCollisionWithPlatform(float dt, Entity *obj = NULL, int delta_y = 14);
	bool IsCollisionWithWater(float dt, int delta_y = 12);
	bool IsCollisionWithWall(float dt, int delta_y = 3);
	bool IsCollisionWithRope(float dt, int delta_y = 22);
	bool IsCollisionWithSpike(float dt, int delta_y = 12);
	BoundingBox GetBoundingBox();
	// Các biến này đáng lẽ phải ở riêng mỗi state, nhưng ở kicking, khi chuyển từ
	// kicking về jumping, jumpingdown hay rolling thì lại phải tạo mới state,
	// như vậy thì time_on_air sẽ được reset lại
	int number_rocket_robot;
	int number_soldier;
	int previous_state;
	int hp;
	int i;
	float time_air_rolling;
	float time_air_jumping;
	float time_kicking;
	float time_ducking_before_idle;
	float time_jumping_before_flowing;
	float time_don_tho;
	float time_invisible;
	float time_guc;
	// Code đi rời để biết tại sao phải tạo những biến này, có một vấn đề là các trạng thái
	// khi new sẽ khởi tạo vận tốc khác nhau, nên khi jumping -> kicking -> jumping thì vận tốc
	// bị khởi động lại 1 lần nữa, những biến này sẽ do idle có quyền định đoạt
	bool IsBornSoldier;
	bool IsBornRocketRobot;
	bool IsJumpingDown;
	bool IsJumping;
	bool IsRolling;
	bool IsThrowing;
	bool IsDuckingPunching;
	bool IsFlowing;
	bool IsDonTho;
	bool IsOnAir;
	bool IsLockCollision;
	bool IsShieldDown;
	bool OnTheWater;
	bool LockState;
	Entity *CarrierObject;
protected:
	static Player *instance;
	float time_buffer;
	Sprite *HPstatus;
	std::map<int, Animation*> animations;
	PlayerState::NameState current_state;
	PlayerState* player_state;
	Animation* animation;
	D3DXVECTOR2 position_idle;
	void DrawHP();
private:
	Player();
	~Player();
};

