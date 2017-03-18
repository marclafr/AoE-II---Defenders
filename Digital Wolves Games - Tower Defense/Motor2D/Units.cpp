
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Animation.h"
#include "Units.h"
#include "p2Log.h"
#include "j1Pathfinding.h"
#include "j1Map.h"

Unit::Unit(UNIT_TYPE u_type, iPoint pos): Entity(UNIT, pos), unit_type(u_type), direction(EAST), action_type(IDLE)
{
	switch (u_type)
	{
		//ADD UNIT: IF ANY UNIT IS ADDED ADD CODE HERE:
	case TWOHANDEDSWORDMAN:
		SetHp(60);
		attack = 12;
		SetArmor(1);
		speed = 0.9;
		rate_of_fire = 2;
		range = 1;
		unit_class = INFANTRY;
		unit_radius = 6;
		break;

	case CAVALRYARCHER:
		SetHp(50);
		attack = 6;
		SetArmor(1);
		speed = 1.4;
		rate_of_fire = 2;
		range = 4;
		unit_class = ARCHER;
		unit_radius = 12;
		break;

	case SIEGERAM:
		SetHp(270);
		attack = 4;
		SetArmor(-5);
		speed = 0.6;
		rate_of_fire = 5;
		range = 1;
		unit_class = SIEGE;
		unit_radius = 15;
		break;

	default:
		LOG("Error UNIT TYPE STATS NULL");
		unit_class = NO_CLASS;
		break;
	}
}

void Unit::Update()
{
	AI();
	Move();
	Draw();
}

void Unit::Move()
{
	if (App->input->GetMouseButtonDown(3) == KEY_DOWN && this->GetEntityStatus() == E_SELECTED)
	{
		this->path_list.clear();
		App->input->GetMousePosition(destination.x, destination.y);
		destination.x -= App->render->camera.x;
		destination.y -= App->render->camera.y;
		if (this->GetPath({ destination.x, destination.y }) != -1)
		{
			path_list.pop_front();
			GetNextTile();
			this->action_type = WALK;
			this->moving = true;
		}
		else
		{
			this->moving = false;
			this->action_type = IDLE;
		}
	}

	//Draw x y axis test
	App->render->DrawLine(0, 0, 1000, 0, 255, 255, 255);
	App->render->DrawLine(0, 0, 0, 1000, 255, 255, 255);

	if (this->moving == true)
	{


		this->SetPosition(GetX() + move_vector.x*speed, GetY() + move_vector.y*speed);

		iPoint unit_world;
		unit_world.x = GetX();
		unit_world.y = GetY();
		if (path_objective.DistanceTo(unit_world) < 3)
		{
			//center the unit to the tile
			this->SetPosition(path_objective.x, path_objective.y);
			if (!GetNextTile())
			{
				moving = false;
				this->action_type = IDLE;
			}

		}
	}
}

void Unit::AI()
{
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
	{
		action_type = ATTACK;
		Animation* anim = nullptr;
		if (direction == EAST)
			anim = App->anim->GetAnimation(unit_type, action_type, WEST);

		else if (direction == NORTH_EAST)
			anim = App->anim->GetAnimation(unit_type, action_type, NORTH_WEST);

		else if (direction == SOUTH_EAST)
			anim = App->anim->GetAnimation(unit_type, action_type, SOUTH_WEST);

		else
			anim = App->anim->GetAnimation(unit_type, action_type, direction);

		anim->Reset();
	}

	if (App->input->GetKey(SDL_SCANCODE_I) == KEY_DOWN)
	{
		action_type = IDLE;
		Animation* anim = nullptr;
		if (direction == EAST)
			anim = App->anim->GetAnimation(unit_type, action_type, WEST);

		else if (direction == NORTH_EAST)
			anim = App->anim->GetAnimation(unit_type, action_type, NORTH_WEST);

		else if (direction == SOUTH_EAST)
			anim = App->anim->GetAnimation(unit_type, action_type, SOUTH_WEST);

		else
			anim = App->anim->GetAnimation(unit_type, action_type, direction);

		anim->Reset();
	}
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
	{
		action_type = WALK;
		Animation* anim = nullptr;
		if (direction == EAST)
			anim = App->anim->GetAnimation(unit_type, action_type, WEST);

		else if (direction == NORTH_EAST)
			anim = App->anim->GetAnimation(unit_type, action_type, NORTH_WEST);

		else if (direction == SOUTH_EAST)
			anim = App->anim->GetAnimation(unit_type, action_type, SOUTH_WEST);

		else
			anim = App->anim->GetAnimation(unit_type, action_type, direction);

		anim->Reset();
	}
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
	{
		action_type = DISAPPEAR;
		Animation* anim = nullptr;
		if (direction == EAST)
			anim = App->anim->GetAnimation(unit_type, action_type, WEST);
		
		else if (direction == NORTH_EAST)
			anim = App->anim->GetAnimation(unit_type, action_type, NORTH_WEST);

		else if (direction == SOUTH_EAST)
			anim = App->anim->GetAnimation(unit_type, action_type, SOUTH_WEST);

		else
			anim = App->anim->GetAnimation(unit_type, action_type, direction);

		anim->Reset();
	}
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
	{
		action_type = DIE;
		Animation* anim = nullptr;
		if (direction == EAST)
			anim = App->anim->GetAnimation(unit_type, action_type, WEST);

		else if (direction == NORTH_EAST)
			anim = App->anim->GetAnimation(unit_type, action_type, NORTH_WEST);

		else if (direction == SOUTH_EAST)
			anim = App->anim->GetAnimation(unit_type, action_type, SOUTH_WEST);

		else
			anim = App->anim->GetAnimation(unit_type, action_type, direction);

		anim->Reset();
	}

}

void Unit::Draw()
{
	SDL_Texture* tex = App->anim->GetTexture(unit_type);
	SDL_Rect rect;
	iPoint pivot;

	App->anim->GetAnimationFrame(rect, pivot, this);

	SetPivot(pivot.x, pivot.y);
	SetRect(rect);
	SetTexture(tex);

	if (direction == NORTH_EAST || direction == EAST || direction == SOUTH_EAST)
		App->render->PushSprite((SDL_Texture*)GetTexture(), GetX(), GetY(), &GetRect(), SDL_FLIP_HORIZONTAL, pivot.x, pivot.y); //Blit has to change to push sprites but it is bugged
	else
		App->render->PushSprite((SDL_Texture*)GetTexture(), GetX() - pivot.x, GetY() - pivot.y, &GetRect());

	if (this->GetEntityStatus() == E_SELECTED)
		App->render->DrawCircle(this->GetX() + App->render->camera.x, this->GetY() + App->render->camera.y, this->unit_radius, 255, 255, 255);
}

const DIRECTION Unit::GetDir() const
{
	return direction;
}

const UNIT_TYPE Unit::GetUnitType() const
{
	return unit_type;
}

const UNIT_CLASS Unit::GetUnitClass() const
{
	return unit_class;
}

const ACTION_TYPE Unit::GetActionType() const
{
	return action_type;
}

int Unit::GetPath(iPoint dest) {
	iPoint ori = App->map->WorldToMap(GetX(), GetY());
	iPoint destinat = App->map->WorldToMap(dest.x, dest.y);
	return App->pathfinding->CreatePath(ori, destinat, path_list);
}

const int Unit::GetAttack() const
{
	return attack;
}

const int Unit::GetRange() const
{
	return range;
}

bool Unit::GetNextTile()
{
	bool ret = true;

	if (path_list.size() == 0)
		return false;

	path_objective = App->map->MapToWorld(path_list.front().x, path_list.front().y);
	path_list.pop_front();

	move_vector.x = (float)path_objective.x - GetX();
	move_vector.y = (float)path_objective.y - GetY();
	float modul = (sqrt(move_vector.x*move_vector.x + move_vector.y * move_vector.y));
	move_vector.x = move_vector.x / modul;
	move_vector.y = move_vector.y / modul;

	iPoint direction_vec;
	direction_vec.x = path_objective.x - GetX();
	direction_vec.y = GetY() - path_objective.y;
	angle = (float)57.29577951 * atan2(direction_vec.y, direction_vec.x);

	if (angle < 0)
		angle += 360;


	if ((0 <= angle &&  angle <= 22.5) || (337.5 <= angle&& angle <= 360))
		this->direction = EAST;

	else if (22.5 <= angle &&  angle <= 67.5)
		this->direction = NORTH_EAST;

	else if (67.5 <= angle &&  angle <= 112.5)
		this->direction = NORTH;

	else if (112.5 <= angle &&  angle <= 157.5)
		this->direction = NORTH_WEST;

	else if (157.5 <= angle &&  angle <= 202.5)
		this->direction = WEST;

	else if (202.5 <= angle &&  angle <= 247.5)
		this->direction = SOUTH_WEST;

	else if (247.5 <= angle &&  angle <= 292.5)
		this->direction = SOUTH;

	else if (292.5 <= angle &&  angle <= 337.5)
		this->direction = SOUTH_EAST;

	else
		this->direction = NO_DIRECTION;

	return ret;
}
