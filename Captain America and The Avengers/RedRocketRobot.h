#pragma once
#include "Enemy.h"
#include "RedRocket.h"
#include "Equation.h"
#define RED_ROCKET_ROBOT_HP 5
#define TIME_DUCKING 2.0f
#define TIME_IDLE 2.0f
#define RED_ROCKET_ROBOT_SIZE_WIDTH 8
#define RED_ROCKET_ROBOT_SIZE_HEIGHT 44
#define RED_ROCKET_ROBOT_FOOT_HEIGHT 8
#define RED_ROCKET_ROBOT_VELOCITY 80
#define RED_ROCKET_ROBOT_VELOCITY_X 80
class RedRocketRobot : public Enemy
{
public:
	enum RedRocketRobotState {
		none,
		idle,
		running,
		ducking,
		beaten,
		jumping,
		walking_throwing
	};
	enum Level {
		stupid,
		normal,
		clever,
		master
	};
	virtual void Update(float dt);
	virtual void UpdateStupidLevel(float dt);
	virtual void UpdateNormalLevel(float dt);
	virtual void UpdateMasterLevel(float dt);
	virtual void UpdateCleverLevel(float dt);
	virtual int OnCollision(Entity*, float dt);
	virtual void Draw();
	BoundingBox GetBoundingBox() override;
	bool IsCollisionWithGround(float dt, int delta_y = 12);
	int IsLui;
	int IsJumpingFirst;
	float time_ducking;
	float time_idle;
	bool IsRunning;
	bool IsDucking;
	bool IsIdle;
	bool IsExplode;
	bool IsHaveJump;
	bool IsLoop;
	bool IsCrossed;
	bool IsLock;
	bool IsChamDatLanDau;
	bool IsChamLanHai;
	bool IsFire;
	bool IsCapNhatVanToc;
	bool IsCapNhatPositionMotLan;
	Entity::Entity_Direction goto_direction;
	Level level;
	RedRocketRobotState current_state;
	RedRocketRobotState previous_state;
	int i = 0;
	Equation *e;
	int NumberRocket;
	// clever zone
	float distance;
	float distance_goto;
	bool Update_position_one_time;
	Entity::Entity_Direction clever_direction;
	void UpdateRunningCleverState(float dt);
	void UpdateJumpingCleverState(float dt);
	void UpdateUPDOWNNormalState(float dt);
	// clever zone
	void UpdateRunningState(float dt);
	void UpdateJumpingState(float dt);
	void UpdateIdleState(float dt);
	void UpdateWalkingThrowState(float dt);
	RedRocketRobot(int level, D3DXVECTOR2 position_spawn, D3DXVECTOR2 position_goto, bool IsCrossed);
	~RedRocketRobot() override;
protected:
	Animation* running_ani;
	Animation* ducking_ani;
	Animation* idle_ani;
};

