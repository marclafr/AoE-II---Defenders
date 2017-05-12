#include "Entity.h"
#include "j1Textures.h"
#include "j1App.h"
#include "j1Render.h"

Entity::Entity(ENTITY_TYPE entity_type, fPoint pos, Side side): to_delete (false), entity_type(entity_type), position(pos), side(side)
{}

Entity::~Entity()
{}

void Entity::Die()
{
	to_delete = true;
}

void Entity::DT(float dt)
{
	ai_dt += dt;
}

void Entity::ResetDT()
{
	ai_dt = 0.0f;
}

bool Entity::ToDelete() const
{
	return to_delete;
}

ENTITY_TYPE Entity::GetEntityType() const
{
	return entity_type;
}

ENTITY_STATUS Entity::GetEntityStatus() {

	return entity_status;
}

void Entity::SetEntityStatus(ENTITY_STATUS status)
{
	entity_status = status;
}

void Entity::SetPosition(float x, float y)
{
	position.x = x;
	position.y = y;
}

float Entity::GetArrowPos() const
{
	return Arrow_pos;
}

void Entity::ResetArrowPos()
{
	Arrow_pos = 0;
}

void Entity::UpdateArrow(int StartHeight, fPoint TargetPos, int CurveHeight, float TimeSecs)
{
	SDL_Rect rect = { 0,0,45,8};
	iPoint pos;

	fPoint initial_point = { GetPosition().x,GetPosition().y - StartHeight };
	fPoint last_point = TargetPos;
	fPoint mid_point = { (initial_point.x + last_point.x) / 2,((initial_point.y + last_point.y) / 2) - CurveHeight};

	pos.x = ((1 - Arrow_pos)*(1 - Arrow_pos)*initial_point.x) + ((2 * Arrow_pos)*(1 - Arrow_pos)*mid_point.x) + ((Arrow_pos*Arrow_pos)*last_point.x);
	pos.y = ((1 - Arrow_pos)*(1 - Arrow_pos)*initial_point.y) + ((2 * Arrow_pos)*(1 - Arrow_pos)*mid_point.y) + ((Arrow_pos*Arrow_pos)*last_point.y);

	App->render->PushInGameSprite(App->tex->GetTexture(T_ARROW_BOMB), pos.x, pos.y, &rect, SDL_FLIP_NONE, 0, 0, 1, 0, false);
	float diferential = 1/ TimeSecs;

	Arrow_pos += diferential;
	if (Arrow_pos > 1) Arrow_pos = 1;
}

const float Entity::GetX() const
{
	return position.x;
}

const float Entity::GetY() const
{
	return position.y;
}

const fPoint Entity::GetPosition() const
{
	return position;
}

const int Entity::GetHp() const
{
	return hp;
}

const int Entity::GetArmor() const
{
	return armor;
}

void Entity::IncreaseArmor(int extra_defense)
{
	armor += extra_defense;
}

const iPoint Entity::GetPivot() const
{
	return pivot;
}

const SDL_Rect Entity::GetRect() const
{
	return rect;
}

const TextureID Entity::GetTextureID() const
{
	return texture_id;
}

const Side Entity::GetSide() const
{
	return side;
}

const float Entity::GetAIDT() const
{
	return ai_dt;
}

void Entity::SetArmor(int new_armor)
{
	armor = new_armor;
}

void Entity::SetHp(int new_hp)
{
	hp = new_hp;
}
const int Entity::GetAttack() const {

	return attack;

}
void Entity::SetPivot(int x, int y)
{
	pivot.x = x;
	pivot.y = y;
}

void Entity::SetRect(SDL_Rect rect)
{
	this->rect = rect;
}

void Entity::SetTextureID(TextureID id)
{
	texture_id = id;
}

void Entity::SetSide(Side side)
{
	this->side = side;
}

void Entity::SetToDelete()
{
	to_delete = true;
}

void Entity::SetAttack(int new_attack)
{
	attack = new_attack;
}
void Entity::Attack(Entity* entity)
{
	entity->SetHp(entity->hp - this->attack);
}

void Entity::Damaged(int dmg)
{
	if (armor >= dmg)
		hp--;
	else
		hp -= (dmg - armor);
}

void Entity::UpgradeUnit(int plushealth) {

	this->SetHp(GetHp() + plushealth);

}
