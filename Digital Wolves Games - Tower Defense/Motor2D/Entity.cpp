#include "Entity.h"

Entity::Entity(ENTITY_TYPE entity_type, iPoint pos): to_delete (false), entity_type(entity_type), position(pos)
{}

Entity::~Entity()
{
}

void Entity::Die()
{
	to_delete = true;
}

bool Entity::ToDelete() const
{
	return to_delete;
}

ENTITY_TYPE Entity::GetEntityType() const
{
	return entity_type;
}

const int Entity::GetX() const
{
	return position.x;
}

const int Entity::GetY() const
{
	return position.y;
}
