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


Building::Building(BUILDING_TYPE b_type, fPoint pos, bool builded) : Entity(E_BUILDING, pos, S_ALLY), building_type(b_type)
{
	SDL_Rect rect;
	SetTextureID(T_TURRET);
	if (builded == true)
		totally_built = true;
	switch (b_type)
	{
	case B_TURRET:
		SetSide(S_ALLY);
		build_rect = IsoRect( GetPosition(), 96, 47);
		break;

	case B_WOOD_WALL:
		SetSide(S_ALLY);
		SetHp(250);
		SetAttack(0);
		SetRect({ 610,289,100,106 });
		SetPivot(0.49 * 100, 106 * 0.754717);
		SetArmor(8);
		build_rect = IsoRect(GetPosition(), 96, 47);
		break;

	case B_TOWNHALL:
		SetSide(S_ALLY);
		SetHp(1500);
		SetAttack(0);
		SetArmor(8);
		rect = { 477,0,366,317 };
		SetRect(rect);
		SetPivot(0.52459 * 366, 0.72555 * 317);
		SetTextureID(T_TOWNHALL);
		totally_built = true;
		build_rect = IsoRect(GetPosition(), 375, 170);
		break;
	case B_UNIVERSITY:
		SetSide(S_ALLY);
		SetHp(1500);
		SetAttack(0);
		SetArmor(8);
		rect = {0,158,483,291};
		SetRect(rect);
		SetPivot(0.509317*483, 0.726923*291);
		SetTextureID(T_TOWNHALL);
		totally_built = true;
		build_rect = IsoRect(GetPosition(), 483, 210);
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
	Draw();

	if (this->GetHp() <= 0 && alive == true)
		ConvertToRubble();

	if (alive == false && DieTimer.ReadSec() >= 2)
		DestroyBuilding();
}

void Building::AI()
{

}

void Building::Draw()
{
	if (totally_built == true)
	{
		if (App->render->camera->InsideRenderTarget(App->render->camera->GetPosition().x + GetX(), App->render->camera->GetPosition().y + GetY()))
			App->render->PushInGameSprite(this);
	}
	else
	{
		if (buildtimer.ReadSec() <= 6)
		{
			SetRect({ 98,0,100,75 });
			SetPivot(0.55 * 100, 75 * 0.643836);
		}

		else
		{
			SetRect({ 610,289,100,106 });
			SetPivot(0.49 * 100, 106 * 0.754717);
			totally_built = true;
		}

		if (App->render->camera->InsideRenderTarget(App->render->camera->GetPosition().x + GetX(), App->render->camera->GetPosition().y + GetY()))
			App->render->PushInGameSprite(this);
	}
}

const BUILDING_TYPE Building::GetBuildingType() const
{
	return building_type;
}

void Building::SetBuildingType(BUILDING_TYPE type)
{
	building_type = type;
}

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
			}
			break;
		default:
			break;
		}
	}
}

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

bool Building::IsBuilt() const
{
	return totally_built;
}

void Building::BuildingComplete()
{
	totally_built = true;
}

bool Building::IsAlive() const
{
	return alive;
}

void Building::ConvertToRubble()
{
	//TODO: If townhall, change the rect
	if (App->render->camera->InsideRenderTarget(App->render->camera->GetPosition().x + GetX(), App->render->camera->GetPosition().y + GetY())) App->audio->PlayFx(App->audio->fx_building_destroyed);
	
	SetTextureID(T_TOWNHALL);
	SDL_Rect rect;
	rect = { 313, 1, 91, 51 };
	SetRect(rect);
	SetPivot(0.362637 * 91, 0.431373 * 51);
	DieTimer.Start();
	SetHp(0);
	alive = false;
	totally_built = true;
	iPoint p = App->map->WorldToMap(GetX(), GetY());
	App->pathfinding->MakeWalkable(p);
}

void Building::DestroyBuilding()
{
	iPoint p = App->map->WorldToMap(GetX(), GetY());
	App->pathfinding->MakeConstruible_neutral(p);
	App->pathfinding->MakeConstruible_ally(p);
	this->Die();
}

const int Building::GetRange() const
{
	//TODO
	return 250;
}

void Building::Save(pugi::xml_node &data)
{
		pugi::xml_node build = data.child("buildings");
		pugi::xml_node ActualBuilding = build.append_child("building");
		ActualBuilding.append_attribute("building_type") = GetBuildingType();
		ActualBuilding.append_attribute("posx") = GetX();
		ActualBuilding.append_attribute("posy") = GetY();
		ActualBuilding.append_attribute("hp") = GetHp();

}
