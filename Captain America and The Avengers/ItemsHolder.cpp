#include "ItemsHolder.h"
#include "Item.h"
#include "SceneManager.h"
#include "Shield.h"
#include "Framework/SoundManager.h"

int ItemsHolder::AnimationID = -1;
int ItemsHolder::AnimationID2 = -1;
int ItemsHolder::ID_1up = -1;
int ItemsHolder::ID_5Point = -1;
int ItemsHolder::ID_ExitOrb= -1;
int ItemsHolder::ID_FullHeal = -1;
int ItemsHolder::ID_HalfHeal = -1;
int ItemsHolder::ID_HighPoint = -1;
int ItemsHolder::ID_LargeGem = -1;
int ItemsHolder::ID_SmallGem = -1;

ItemsHolder::ItemsHolder(float x, float y, int code)
{
	Entity::SetTag(Entity::Entity_Tag::item_container);
	Entity::SetType(Entity::Entity_Type::item_type);
	Entity::position.x = x;
	Entity::position.y = y;
	if (code == 0)
	{
		animation = new Animation(AnimationID, 2);
	}
	else
	{
		animation = new Animation(AnimationID2, 2);
	}
	animation->SetAutoPlayBack(false);
	animation->SetTime(0.25f);
	animation->Stop();
	SIZE size;
	size.cx = ITEMS_HOLDER_SIZE_WIDTH;
	size.cy = ITEMS_HOLDER_SIZE_HEIGHT;
	HitBox = BoundingBox(D3DXVECTOR2(x, y), size, 0, 0);
	Items = new std::vector<Entity*>();
}

ItemsHolder::~ItemsHolder()
{
	delete animation;
}

BoundingBox ItemsHolder::GetBoundingBox()
{
	return HitBox;
}

int ItemsHolder::OnCollision(Entity *obj, float dt)
{
	if (animation->GetNumberCurrentFrame() == 2)
	{
		return 0;
	}
	if (obj->GetType() == Entity::Entity_Type::player_weapon_type)
	{
		if (obj->GetTag() == Entity::Entity_Tag::shield && Shield::GetInstance()->GetShieldState()->GetCurrentState() != ShieldState::NameState::ShieldAttack)
		{
			return 0;
		}
		if (Collision::getInstance()->SweptAABB(obj->GetBoundingBox(), GetBoundingBox()).CollisionTime > 1.0f)
		{
			return 0;
		}
		animation->SetFrame(2);
		animation->Continue();
		auto grid = SceneManager::GetInstance()->GetCurrentScene()->GetCurrentGrid();
		if (Items->empty() || grid->ItemCounter >= CAPACITY_ITEM)
		{
			return 0;
		}
		SoundManager::GetInstance()->Play(SoundManager::SoundList::item_holder);
		// drop item
		grid->ItemCounter++;
		Entity *item = (*Items)[0];
		if (item->GetTag() == Entity::Entity_Tag::item_exit_orb)
		{
			if (SceneManager::GetInstance()->GetCurrentScene()->IsExitAble)
			{
				delete (*Items)[0];
				Items->erase(Items->begin());
				item = (*Items)[0];
			}
			else
			{
				grid->AddObject2Cell(new Item(position.x, position.y, Item::Item_Tag::ExitOrb));
				return 0;
			}
		}
		grid->AddObject2Cell(item);
		Items->erase(Items->begin());
	}
	return 0;
}

void ItemsHolder::Update(float dt)
{
	animation->Update(dt);
}

void ItemsHolder::Draw()
{
	if (SceneManager::GetInstance()->GetCurrentScene()->GetMode() & 1)
	{
		animation->DrawInt(position);
	}
}

void ItemsHolder::PutOnItem(int Item_Tag)
{
	Items->push_back(new Item(position.x, position.y, Item_Tag));
}
