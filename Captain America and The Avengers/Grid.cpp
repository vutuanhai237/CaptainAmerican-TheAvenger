#include "Grid.h"
#include <math.h>
#include "RedRocketRobot.h"
#include "BlueSoldier.h"
#include "GreenSoldier.h"
#include "GrayRobot.h"
#include "Turret.h"
#include "Bat.h"
#include "GrayRocketRobot.h"
Grid::Grid(SIZE MapSize)
{
	Init(MapSize.cx, MapSize.cy);
}

Grid::Grid(int MapSizeWidth, int MapSizeHeight)
{
	Init(MapSizeWidth, MapSizeHeight);
}

Grid::~Grid()
{
	for (int i = 0; i < CellX; i++)
	{
		for (int j = 0; j < CellY; j++)
			delete grid[i][j];
		delete grid[i];
	}
	delete grid;
}

void Grid::AddObject2Cell(int WorldX, int WorldY, int* object)
{
	grid[WorldX / GRID_CELL_SIZE_WIDTH][WorldY / GRID_CELL_SIZE_HEIGHT]->InitObject->push_back(object);
}

void Grid::AddObject2Cell(Entity *object)
{
	int col = int(object->GetPosition().x / GRID_CELL_SIZE_WIDTH);
	int row = int(object->GetPosition().y / GRID_CELL_SIZE_HEIGHT);
	if (IsActivated(col, row))
		grid[col][row]->Object->push_back(object);
}

void Grid::Update(float dt)
{
	UpdateActivatedZone();
	RemoveAndReswampObject();
	UpdateGrid();
	CheckCollision(dt);
	UpdateEntity(dt);
}

void Grid::Init(int MapSizeWidth, int MapSizeHeight)
{
	CellX = (int)ceilf(float(MapSizeWidth + 1) / GRID_CELL_SIZE_WIDTH) + 1;
	CellY = (int)ceilf(float(MapSizeHeight + 1) / GRID_CELL_SIZE_HEIGHT) + 1;
	grid = new Cell**[CellX];
	for (int i = 0; i < CellX; i++)
	{
		grid[i] = new Cell*[CellY];
		for (int j = 0; j < CellY; j++)
			grid[i][j] = new Cell;
	}

	ItemCounter = 0;
	EnemyCounter = 0;

	Xfrom = Yfrom = 0;
	Xto = CellX - 1;
	Yto = CellY - 1;
}

void Grid::UpdateActivatedZone()
{
	D3DXVECTOR2 pos = Camera::GetInstance()->GetCameraPosition();
	Xfrom = (int)(floor((pos.x - 1) / GRID_CELL_SIZE_WIDTH) - 1);
	Xto = (int)(ceil((pos.x + 1 + GAME_SCREEN_WIDTH) / GRID_CELL_SIZE_WIDTH) + 1);
	Yfrom = (int)(floor(pos.y / GRID_CELL_SIZE_HEIGHT));
	Yto = (int)(ceil((pos.y + GAME_SCREEN_HEIGHT) / GRID_CELL_SIZE_HEIGHT));
	if (Xfrom < 0)
		Xfrom = 0;
	if (Xto >= CellX)
		Xto = CellX - 1;
}

void Grid::RemoveAndReswampObject()
{
	for (int i = 0; i < CellX; i++)
		for (int j = 0; j < CellY; j++)
			if (IsActivated(i, j))
			{
				if (!grid[i][j]->IsActive)
				{
					for (auto item : *grid[i][j]->InitObject)
						switch (item[0])
						{
						case Entity::Entity_Tag::redrobotrocket:
							if (this->EnemyCounter < CAPACITY_ENEMY)
							{
								grid[i][j]->Object->push_back(new RedRocketRobot(item[3], D3DXVECTOR2((FLOAT)item[1], (FLOAT)item[2]), D3DXVECTOR2((FLOAT)item[4], (FLOAT)item[5]), item[6]));
								this->EnemyCounter++;
							}
							break;
						case Entity::Entity_Tag::blue_soldier:
							if (this->EnemyCounter < CAPACITY_ENEMY)
							{
								grid[i][j]->Object->push_back(new BlueSoldier(item[3], D3DXVECTOR2((FLOAT)item[1], (FLOAT)item[2]), item[4]));
								this->EnemyCounter++;
							}
							break;
						case Entity::Entity_Tag::green_soldier:
							if (this->EnemyCounter < CAPACITY_ENEMY)
							{
								grid[i][j]->Object->push_back(new GreenSoldier(item[3], D3DXVECTOR2((FLOAT)item[1], (FLOAT)item[2]), item[4]));
								this->EnemyCounter++;
							}
							break;
						case Entity::Entity_Tag::gray_robot:
							if (this->EnemyCounter < CAPACITY_ENEMY)
							{
								grid[i][j]->Object->push_back(new GrayRobot(D3DXVECTOR2((FLOAT)item[1], (FLOAT)item[2]), (float)item[3]));
								this->EnemyCounter++;
							}
							break;
						case Entity::Entity_Tag::cannon:
							if (this->EnemyCounter < CAPACITY_ENEMY)
							{
								grid[i][j]->Object->push_back(new Turret(D3DXVECTOR2((FLOAT)item[1], (FLOAT)item[2]), (float)item[3]));
							}
							break;
						case Entity::Entity_Tag::bat:
							if (this->EnemyCounter < CAPACITY_ENEMY)
							{
								grid[i][j]->Object->push_back(new Bat(item[3], D3DXVECTOR2((FLOAT)item[1], (FLOAT)item[2])));
								this->EnemyCounter++;
							}
							break;
						case Entity::Entity_Tag::gray_rocket_robot:
							if (this->EnemyCounter < CAPACITY_ENEMY)
							{
								grid[i][j]->Object->push_back(new GrayRocketRobot(D3DXVECTOR2((FLOAT)item[1], (FLOAT)item[2]), D3DXVECTOR2((FLOAT)item[3], (FLOAT)item[4]), item[5]));
								this->EnemyCounter++;
							}
							break;
						default:
							break;
						}
				}
				grid[i][j]->IsActive = true;
			}
			else
			{
				auto it = grid[i][j]->Object->begin();
				while (it != grid[i][j]->Object->end())
					if ((*it)->GetTag() == Entity::Entity_Tag::shield || (*it)->GetTag() == Entity::Entity_Tag::player)
					{
						// shield & player exception
						D3DXVECTOR2 pos = (*it)->GetPosition();
						int LocX = (int)(pos.x / GRID_CELL_SIZE_WIDTH);
						int LocY = (int)(pos.y / GRID_CELL_SIZE_HEIGHT);
						if ((LocX != i || LocY != j) && 0 < LocX && LocX < CellX)
						{
							grid[LocX][LocY]->Object->push_back(*it);
							auto del = it;
							it++;
							grid[i][j]->Object->erase(del);
						}
						else
							it++;
					}
					else 
						RemoveObjectInList(grid[i][j]->Object, it);
				grid[i][j]->IsActive = false;
			}
}

void Grid::UpdateGrid()
{
	D3DXVECTOR2 pos;
	int LocX, LocY;

	for (int i = Xfrom; i <= Xto; i++)
		for (int j = Yfrom; j <= Yto; j++)
		{
			auto it = grid[i][j]->Object->begin();
			while (it != grid[i][j]->Object->end())
			{
				pos = (*it)->GetPosition();
				LocX = (int)(pos.x / GRID_CELL_SIZE_WIDTH);
				LocY = (int)(pos.y / GRID_CELL_SIZE_HEIGHT);
				if ((LocX != i || LocY != j) && 0 <= LocX && LocX < CellX && 0 <= LocY && LocY < CellY) 
				{
					grid[LocX][LocY]->Object->push_back(*it);
					auto del = it;
					it++;
					grid[i][j]->Object->erase(del);
				}
				else
					it++;
			}
		}
}

void Grid::CheckCollision(float dt)
{
	int ret;

	for (int i = Xfrom; i <= Xto; i++)
		for (int j = Yfrom; j <= Yto; j++)
		{
			auto objs = grid[i][j]->Object;
			// Check self cell
			if (objs->size() >= 2)
			{
				auto it_i = objs->begin();
				while (it_i != objs->end())
				{
					auto it_j = it_i;
					it_j++;
					while (it_j != objs->end())
					{
						ret = (*it_i)->OnCollision(*it_j, dt);
						if (ret == 1)
						{
							if (RemoveObjectInList(objs, it_i))
								goto CHECK_OTHER;
						}
						else if (ret == -1)
						{
							if (RemoveObjectInList(objs, it_j))
								break;
						}
						else
						{
							ret = (*it_j)->OnCollision(*it_i, dt);
							if (ret == -1)
							{
								if (RemoveObjectInList(objs, it_i))
									goto CHECK_OTHER;
							}
							else if (ret == 1)
							{
								if (RemoveObjectInList(objs, it_j))
									break;
							}
						}
						it_j++;
					}
					it_i++;
				}
			}
			// Check another cell
		CHECK_OTHER:
			if (!objs->empty())
			{
				if (i > 0)
				{
					CollisionCall(objs, grid[i - 1][j]->Object, dt);
					if (j < CellY - 1)
						CollisionCall(objs, grid[i - 1][j + 1]->Object, dt);
					if (j > 0)
						CollisionCall(objs, grid[i - 1][j - 1]->Object, dt);
				}
				if (j > 0)
					CollisionCall(objs, grid[i][j - 1]->Object, dt);
			}
		}
}

void Grid::UpdateEntity(float dt)
{
	for (int i = Xfrom; i <= Xto; i++)
		for (int j = Yfrom; j <= Yto; j++)
			for (auto obj : *grid[i][j]->Object)
				if (obj->GetTag() != Entity::Entity_Tag::player && obj->GetTag() != Entity::Entity_Tag::shield)
					obj->Update(dt);
}

void Grid::DrawActivatedObject()
{
	for (int i = Xfrom; i <= Xto; i++)
		for (int j = Yfrom; j <= Yto; j++)
			for (auto obj : *grid[i][j]->Object)
				if (obj->GetType() == Entity::Entity_Type::platform)
					obj->Draw();

	for (int i = Xfrom; i <= Xto; i++)
		for (int j = Yfrom; j <= Yto; j++)
			for (auto obj : *grid[i][j]->Object)
				if (obj->GetType() == Entity::Entity_Type::static_type)
					obj->Draw();

	for (int i = Xfrom; i <= Xto; i++)
		for (int j = Yfrom; j <= Yto; j++)
			for (auto obj : *grid[i][j]->Object)
				if (obj->GetType() == Entity::Entity_Type::item_type)
					obj->Draw();

	for (int i = Xfrom; i <= Xto; i++)
		for (int j = Yfrom; j <= Yto; j++)
			for (auto obj : *grid[i][j]->Object)
				if (obj->GetType() == Entity::Entity_Type::enemy_type &&  obj->GetActive())
					obj->Draw();

	for (int i = Xfrom; i <= Xto; i++)
		for (int j = Yfrom; j <= Yto; j++)
			for (auto obj : *grid[i][j]->Object)
				if (obj->GetType() == Entity::Entity_Type::enemy_weapon_type && obj->GetActive())
					obj->Draw();

}

void Grid::ForceEnemyExplode()
{
	for (int i = Xfrom; i <= Xto; i++)
		for (int j = Yfrom; j <= Yto; j++)
		{
			std::list<Entity*> *objs = grid[i][j]->Object;
			std::list<Entity*>::iterator it = objs->begin();
			while (it != objs->end())
			{
				if ((*it)->GetType() == Entity::Entity_Type::enemy_type)
					dynamic_cast<Enemy*>(*it)->hp = 0;
				it++;
			}
		}
}

bool Grid::IsActivated(int column, int row)
{
	return Xfrom <= column && column <= Xto && Yfrom <= row && row <= Yto;
}

void Grid::CollisionCall(std::list<Entity*> *ListObject1, std::list<Entity*> *ListObject2, float dt)
{
	int ret = 0;

	auto it_i = ListObject1->begin();
	while (it_i != ListObject1->end())
	{
		auto it_j = ListObject2->begin();
		while (it_j != ListObject2->end())
		{
			ret = (*it_i)->OnCollision(*it_j, dt);
			if (ret == 1) // remove it_i;
			{
				if (RemoveObjectInList(ListObject1, it_i))
					return;
				else
					goto ENDLOOP;
			}
			else if (ret == -1) // remove it_j
			{
				if (RemoveObjectInList(ListObject2, it_j))
					break;
				else
					continue;
			}
			else
			{
				ret = (*it_j)->OnCollision(*it_i, dt);
				if (ret == -1) // remove it_i;
				{
					if (RemoveObjectInList(ListObject1, it_i))
						return;
					else
						goto ENDLOOP;
				}
				else if (ret == 1) // remove it_j
				{
					if (RemoveObjectInList(ListObject2, it_j))
						break;
					else
						continue;
				}
			}
			it_j++;
		}
		it_i++;
	ENDLOOP:
		{}
	}
}

bool Grid::RemoveObjectInList(std::list<Entity*>* list, std::list<Entity*>::iterator &it)
{
	std::list<Entity*>::iterator del = it;
	it++;
	switch ((*del)->GetType())
	{
	case Entity::Entity_Type::static_type:
	case Entity::Entity_Type::platform:
	case Entity::Entity_Type::player_type:
		goto UN_DELETE;
	case Entity::Entity_Type::item_type:
		if ((*del)->GetTag() == Entity::Entity_Tag::item_container)
			goto UN_DELETE;
		else
			this->ItemCounter--;
		break;
	case Entity::Entity_Type::enemy_type:
		if ((*del)->GetTag() == Entity::Entity_Tag::boss)
			goto UN_DELETE;
		this->EnemyCounter--;
		break;
	default:
		break;
	}
	delete (*del);
	list->erase(del);

UN_DELETE:

	if (it == list->end())
		return true;
	else
		return false;
}
