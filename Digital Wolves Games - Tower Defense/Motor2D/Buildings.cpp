#include "Buildings.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Animation.h"
#include "p2Log.h"
#include "j1Map.h"
#include "j1EntityManager.h"
#include "j1Pathfinding.h"
#include "j1Audio.h"
#include "Camera.h"
#include "j1Scene.h"

Building::Building(BUILDING_TYPE b_type, iPoint pos, bool builded) : Entity(E_BUILDING, S_ALLY), building_type(b_type), center_position(pos), build_rect(GetPosition(), 375, 170)
{
	SDL_Rect rect;
	building_fire = new AnimationManager(App->anim->GetAnimationType(ANIM_BUILDINGS_FIRE));

	if (builded == true)
		state = BS_BUILT;

	//Construction Texture
	SetRect({ 98,0,100,75 });
	SetPivot(0.55 * 100, 75 * 0.643836);

	SetAttack(0);
	SetSide(S_ALLY);

	//not done yet
	SetPositions();

	switch (b_type)
	{
	case B_TURRET:
		SetTextureID(T_TURRET);
		break;

	case B_WOOD_WALL:
		SetTextureID(T_TURRET);
		SetHp(250);
		SetMaxHP(250);
		SetArmor(4);
		break;

	case B_TOWNHALL:
		SetHp(1500);
		SetMaxHP(1500);
		SetArmor(6);
		SetTextureID(T_TOWNHALL);
		break;

	case B_UNIVERSITY:
		SetHp(1500);
		SetMaxHP(1500);
		SetArmor(8);
		SetTextureID(T_TOWNHALL);
		break;

	default:
		LOG("Error BUILDING TYPE STATS NULL");
		building_type = B_NO_BUILDING;
		break;
	}

	buildtimer.Start();
	iPoint p = App->map->WorldToMap(pos.x, pos.y);

	App->pathfinding->MakeNoWalkable(p);
	App->pathfinding->MakeNoConstruible_neutral(p);
	App->pathfinding->MakeNoConstruible_ally(p);
}

Building::~Building()
{
}

void Building::Update(float dt)
{
	AI();
	Draw();
}

void Building::AI()
{
	switch (state)
	{
	case BS_IN_CONSTRUCTION:
		if (!IsAlive())
			Destroied();

		if (buildtimer.ReadSec() >= 6.0f)
		{
			state_changed = true;
			state = BS_BUILT;
			//need to change texture for everything but TH and uni
		}
		break;

	case BS_BUILT:
		if (!IsAlive())
			Destroied();

		if (App->entity_manager->AreUnitsInRect(GetInWorldTextureRect()))
		{
			if (building_type == B_TOWNHALL || building_type == B_UNIVERSITY)
				SetTextureID(T_TOWNHALL_ALPHA_DOWN);
		}
		else
		{
			if (building_type == B_TOWNHALL || building_type == B_UNIVERSITY)
				SetTextureID(T_TOWNHALL);
		}

		break;

	case BS_DESTROIED:
		if (alive == false && DieTimer.ReadSec() >= 2.0f)
			DestroyBuilding();
		break;

	default:
		LOG("Can not change building texture with null building state state");
		break;
	}

	if (state_changed == true)
	{
		ChangeTexture();
		state_changed = false;
	}
	//SetRect({ 610,289,100,106 });
	//SetPivot(0.49 * 100, 106 * 0.754717);
}

void Building::Draw()
{
	App->render->PushInGameSprite(this);
}

void Building::SetPositions()
{
	switch (building_type)
	{
	case B_TURRET:
		break;

	}
}

void Building::ChangeTexture()
{
	switch (state)
	{
	case BS_IN_CONSTRUCTION:
		SetRect({ 98,0,100,75 });
		SetPivot(0.55 * 100, 75 * 0.643836);
		break;

	case BS_BUILT:
		switch (building_type)
		{
		case B_TURRET:
			break;
		case B_WOOD_WALL:
			SetRect({ 610,289,100,106 });
			SetPivot(0.49 * 100, 106 * 0.754717);
			break;
		case B_TOWNHALL:
			SetRect({ 477,0,366,317 });
			SetPivot(0.52459 * 366, 0.72555 * 317);
			break;

		case B_UNIVERSITY:
			SetRect({ 0,158,483,291 });
			SetPivot(0.509317 * 483, 0.726923 * 291);
			break;

		default:
			LOG("Error BUILDING TYPE STATS NULL");
			building_type = B_NO_BUILDING;
			break;
		}
		break;

	case BS_DESTROIED:
		SetRect({ 313, 1, 91, 51 });
		SetPivot(0.362637 * 91, 0.431373 * 51);
		break;

	default:
		LOG("Can not change building texture with null building state state");
		break;
	}
}

void Building::Destroied()
{
	state = BS_DESTROIED;
	state_changed = true;
	App->audio->PlayFx(App->audio->fx_building_destroyed);
	DieTimer.Start();
}

const iPoint & Building::GetPosition() const
{
	return center_position;
}

const BUILDING_TYPE Building::GetBuildingType() const
{
	return building_type;
}

void Building::SetBuildingType(BUILDING_TYPE type)
{
	building_type = type;
}

/*
void Building::UpgradeWall(BUILDING_TYPE type)
{
	if (this->IsBuilt())
	{
		switch (type)
		{
		case B_WOOD_WALL:
			if (App->scene->resources->CanUpgradeWall(type) == true)
			{
				App->scene->resources->UpgradeWall(type);
				SetRect({ 1020,12,99,178 });
				SetPivot(0.494949 * 99, 178 * 0.865169);
				building_type = B_STONE_WALL;
				SetHp(500);
				SetMaxHP(500);
				Alpha_rect = { (int)GetX() - (GetRect().w / 2),(int)GetY() - GetRect().h, GetRect().w, GetRect().h };
			}
			break;
		case B_STONE_WALL:
			if (App->scene->resources->CanUpgradeWall(type) == true)
			{
				App->scene->resources->UpgradeWall(type);
				SetRect({ 0,66,95,169 });
				SetPivot(0.454211 * 96, 169 * 0.899822);
				building_type = B_BRICK_WALL;
				SetHp(750);
				SetMaxHP(750);
				Alpha_rect = { (int)GetX() - (GetRect().w / 2),(int)GetY() - GetRect().h, GetRect().w, GetRect().h };
			}
			break;
		default:
			break;
		}
	}
}*/

IsoRect Building::GetBuildRectangle()
{
	return build_rect;
}

const double Building::GetBuildTime() const
{
	return buildtimer.ReadSec();
}

const double Building::GetDieTime() const
{
	return DieTimer.ReadSec();
}

bool Building::IsAlive() const
{
	return alive;
}

void Building::DestroyBuilding()
{
	for (std::vector<iPoint>::iterator it = positions.begin(); it != positions.end(); ++it)
		App->pathfinding->MakeWalkable(*it);

	this->Die();
}

const int Building::GetMaxHp() const
{
	return max_hp;
}

void Building::SetMaxHP(int maxhp)
{
	max_hp = maxhp;
}

/*void Building::Save(pugi::xml_node &data)
{
		pugi::xml_node build = data.child("buildings");
		pugi::xml_node ActualBuilding = build.append_child("building");
		ActualBuilding.append_attribute("building_type") = GetBuildingType();
		ActualBuilding.append_attribute("posx") = GetX();
		ActualBuilding.append_attribute("posy") = GetY();
		ActualBuilding.append_attribute("hp") = GetHp();
}*/

bool Building::IsTower()
{
	if (building_type == B_TURRET || building_type == B_CANNON || building_type == B_TURRET_UPGRADED_FIRE || building_type == B_TURRET_UPGRADED_ICE	|| 
		building_type == B_TURRET_UPGRADED_AIR || building_type == B_CANNON_UPGRADED_FIRE || building_type == B_CANNON_UPGRADED_ICE || building_type == B_CANNON_UPGRADED_AIR)
		return true;
}
