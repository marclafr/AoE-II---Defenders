#include <math.h>
#include "p2Log.h"
#include "j1App.h"
#include "IsoPrimitives.h"
#include "j1Pathfinding.h"
#include "QuadTree.h"


QuadTreeNode::QuadTreeNode(IsoRect area, QuadTreeNode * parent): area(area), parent(parent)
{
	for (int i = 0; i < 4; i++)
		childs[i] = nullptr;

	for (int i = 0; i < NODE_ENTITIES; i++)
		entities[i] = nullptr;

}

QuadTreeNode::~QuadTreeNode()
{
	for (int i = 0; i < 4; i++)
		if (childs[i] != nullptr)
		{
			DELETE_PTR(childs[i]);
		}
		else
			break;

	for (int i = 0; i < NODE_ENTITIES; i++)
		if(entities[i] != nullptr)
		{
			//DELETE_PTR(entities[i]); TODO
		}
		else
			break;
}

bool QuadTreeNode::AddEntity(Entity * entity)
{
	bool ret = false;

	if (childs[0] == nullptr)
	{
		if (entities[NODE_ENTITIES - 1] != nullptr)
		{
			ret = SubDivide(entity);
			if (ret == false)
				LOG("Quadtree-Subdivide fail");
		}
		else
			for (int i = 0; i < NODE_ENTITIES; i++)
				if (entities[i] == nullptr)
				{
					entities[i] = entity;
					ret = true;
					break;
				}
	}
	else
		ret = PushToCorrectChild(entity);
	return ret;
}

bool QuadTreeNode::Inside(const Entity* entity) const
{
	return area.Inside(entity->GetPosition());
}

Entity * QuadTreeNode::SearchFirst(int pixel_range, const fPoint from) const
{
	Circle circle(from, pixel_range);

	if (childs[0] == nullptr)
	{
		for (int i = 0; i < NODE_ENTITIES; i++)
			if (entities[i] != nullptr)
				if (circle.Intersects(&iPoint(entities[i]->GetX(), entities[i]->GetY())))
					return entities[i];
	}
	else
		for(int i = 0; i < 4; i++)
			if (circle.Intersects(&childs[i]->area))
				 return childs[i]->SearchFirst(pixel_range, from);
	return nullptr;
}

Entity * QuadTreeNode::SearchFirst(const SDL_Rect rect) const
{
	if (childs[0] == nullptr)
	{
		for (int i = 0; i < NODE_ENTITIES; i++)
		{
			iPoint entity_pos(entities[i]->GetX(), entities[i]->GetY());
			if (entities[i] != nullptr)
				if(rect.x < entity_pos.x && rect.x + rect.w > entity_pos.x
					&& rect.y < entity_pos.y && rect.y + rect.h > entity_pos.y)
					return entities[i];
		}
	}
	else
		for (int i = 0; i < 4; i++)
			if (childs[i]->area.Overlaps(rect))
				return childs[i]->SearchFirst(rect);
	return nullptr;
}

Entity * QuadTreeNode::SearchFirstEnemy(int pixel_range, const fPoint from, const Side side) const
{
	Circle circle(from, pixel_range);

	if (childs[0] == nullptr)
	{
		for (int i = 0; i < NODE_ENTITIES; i++)
			if (entities[i] != nullptr)
				if (circle.Intersects(&iPoint(entities[i]->GetX(), entities[i]->GetY())))
					if(entities[i]->GetSide() != side && entities[i]->GetHp() > 0)
						return entities[i];
	}
	else
		for (int i = 0; i < 4; i++)
			if (circle.Intersects(&childs[i]->area))
				return childs[i]->SearchFirstEnemy(pixel_range, from, side);
	return nullptr;
}

void QuadTreeNode::Search(int pixel_range, const fPoint from, std::vector<Entity*>& vec) const
{
	vec.clear();
	Circle circle(from, pixel_range);

	if (childs[0] == nullptr)
	{
		for (int i = 0; i < NODE_ENTITIES; i++)
			if (entities[i] != nullptr)
				if (circle.Intersects(&iPoint(entities[i]->GetX(), entities[i]->GetY())))
					vec.push_back(entities[i]);
	}
	else
		for (int i = 0; i < 4; i++)
			if (circle.Intersects(&childs[i]->area))
				childs[i]->Search(pixel_range, from, vec);
}

void QuadTreeNode::Search(const SDL_Rect rect, std::vector<Entity*>& vec) const
{
	vec.clear();

	if (childs[0] == nullptr)
	{
		for (int i = 0; i < NODE_ENTITIES; i++)
			if(entities[i] != nullptr)
				if (entities[i]->Inside(rect))
					vec.push_back(entities[i]);
	}
	else
		for (int i = 0; i < 4; i++)
			if (childs[i]->area.Overlaps(rect))
				childs[i]->Search(rect, vec);
}

bool QuadTreeNode::PushToCorrectChild(Entity * entity)
{
	for (int i = 0; i < 4; i++)
		if (childs[i]->Inside(entity))
			return childs[i]->AddEntity(entity);
	return false;
}

bool QuadTreeNode::SubDivide(Entity * entity)
{
	bool ret = false;

	float half_w = ceil(area.GetWidth() / 2.0f);
	float half_h = ceil(area.GetHeight() / 2.0f);

	fPoint first_pos(area.GetPosition().x, area.GetPosition().y - half_h / 2.0f);
	fPoint second_pos(area.GetPosition().x - half_w / 2.0f, area.GetPosition().y);
	fPoint third_pos(area.GetPosition().x, area.GetPosition().y + half_h / 2.0f);
	fPoint fourth_pos(area.GetPosition().x + half_w / 2.0f, area.GetPosition().y);

	IsoRect first(first_pos, half_w, half_h);
	IsoRect second(second_pos, half_w, half_h);
	IsoRect third(third_pos, half_w, half_h);
	IsoRect forth(fourth_pos, half_w, half_h);

	childs[0] = new QuadTreeNode(first, this);
	childs[1] = new QuadTreeNode(second, this);
	childs[2] = new QuadTreeNode(third, this);
	childs[3] = new QuadTreeNode(forth, this);

	for (int i = 0; i < NODE_ENTITIES; i++)
	{
		ret = PushToCorrectChild(entities[i]);
		
		if (ret == false)
			return ret;
		else
			entities[i] = nullptr;
	}

	ret = PushToCorrectChild(entity);
	return ret;
}

void QuadTreeNode::Reorganise()
{
	for(int i = 0; i < NODE_ENTITIES - 1; i++)
		if (entities[i] == nullptr && entities[i + 1] != nullptr)
		{
			entities[i] = entities[i + 1];
			entities[i+ 1] = nullptr;
			i = 0;
		}
}

void QuadTreeNode::RedoQuads()
{
	if (childs[0] != nullptr)
	{
		int num_entities = 0;

		for (int i = 0; i < 4; i++)
		{
			num_entities += childs[i]->NumEntities();
		}

		if (num_entities <= NODE_ENTITIES)
			for (int i = 0; i < 4; i++)
			{
				childs[i]->MoveUpEntities();
				DELETE_PTR(childs[i]);
			}
	}
}

bool QuadTreeNode::Empty() const
{
	bool ret = true;

	if(entities[0] != nullptr)
		ret = false;
	else
	{
		if (childs[0] != nullptr)
			for (int i = 0; i < 4; i++)
			{
				ret = childs[i]->Empty();
				if (ret == false)
					break;
			}
		else
			ret = true;
	}
	return ret;
}

int QuadTreeNode::NumEntities() const
{
	int ret = 0;

	if (childs[0] == nullptr)
	{
		while (ret <= NODE_ENTITIES)
		{
			if (entities[ret] != nullptr)
				ret++;
			else
				break;
		}
	}
	else
		for (int i = 0; i < 4; i++)
			ret += childs[i]->NumEntities();

	return ret;
}

void QuadTreeNode::MoveUpEntities()
{
	for (int i = 0; i < NODE_ENTITIES; i++)
	{
		if (entities[i] != nullptr)
		{
			for (int j = 0; j < NODE_ENTITIES; j++)
				if (parent->entities[j] == nullptr)
				{
					parent->entities[j] = entities[i];
					entities[i] = nullptr;
				}
		}
		else
			break;
	}
}

void QuadTreeNode::Update(float dt)
{
	if (childs[0] == nullptr)
	{
		for (int i = 0; i < NODE_ENTITIES; i++)
			if (entities[i] != nullptr)
			{
				entities[i]->Update(dt);

				if (entities[i]->GetEntityType() == E_UNIT && Inside(entities[i]) == false)
				{
					QuadTreeNode* up_iterator = parent;
					while (up_iterator != nullptr)
					{
						if (up_iterator->Inside(entities[i]))
						{
							if (up_iterator->AddEntity(entities[i]))
							{
								entities[i] = nullptr;
								break;
							}						
						}
						else
							up_iterator = up_iterator->parent;
					}		
					Reorganise();
					RedoQuads();
				}
			}
			else
				break;
	}
	else
		for (int i = 0; i < 4; i++)
			childs[i]->Update(dt);
}

void QuadTreeNode::DeleteEntities()
{
	if (childs[0] == nullptr)
	{
		for (int i = 0; i < NODE_ENTITIES; i++)
			if (entities[i] != nullptr)
				if (entities[i]->ToDelete())
				{
					DELETE_PTR(entities[i]);
					Reorganise();
				}
			else
				break;
	}
	else
	{	
		for(int i = 0; i < 4; i++)
			childs[i]->DeleteEntities();
		RedoQuads();
	}
}

void QuadTreeNode::CheckCollision() const
{
	if (childs[0] == nullptr)
	{
		for (int i = 0; i < NODE_ENTITIES; i++)
			if (entities[i] != nullptr && entities[i]->GetEntityType() == E_UNIT)
			{
				Unit* unit = (Unit*)entities[i];
				unit->CheckCollisions();
			}
				
			else
				break;
	}
	else
		for (int i = 0; i < 4; i++)
			childs[i]->CheckCollision();
}

void QuadTreeNode::CheckUnitCollisions(const Unit * ptr) const
{
	if (childs[0] == nullptr)
	{
		for (int i = 0; i < NODE_ENTITIES; i++)
		{
			if (entities[i] != nullptr)
			{
				if (entities[i]->GetEntityType() == E_UNIT)
				{
					Unit* unit = (Unit*)entities[i];
					if (ptr->GetUnitCircle().Overlap(&unit->GetUnitCircle()))
						if (unit->GetAction() == A_IDLE)
						{
							iPoint destination;
							if (App->pathfinding->FindNearestUnocupied(destination))
								unit->GoTo(destination);
						}
						else if (unit->GetAction() == A_ATTACK)
						{
							iPoint destination;
							if (unit->FindEmptyAttackPos(destination))
								unit->GoTo(destination);
						}
				}
			}
		}
	}
	else
		for (int i = 0; i < 4; i++)
			if (ptr->GetUnitCircle().Intersects(&childs[i]->area))
				childs[i]->CheckUnitCollisions(ptr);
}

void QuadTreeNode::DrawArea()
{
	if (childs[0] != nullptr)
		for (int i = 0; i < 4; i++)
			childs[i]->DrawArea();

	area.SetColor(SDL_Color{ 255,0,0,255 });
	area.Draw();
}

QuadTree::QuadTree(IsoRect area)
{
	origin = new QuadTreeNode(area, nullptr);
}

QuadTree::~QuadTree()
{
	DELETE_PTR(origin);
}

bool QuadTree::PushBack(Entity * entity) const
{
	if(!origin->Inside(entity))
		return false;

	origin->AddEntity(entity);
	return true;
}

Entity * QuadTree::SearchFirst(int pixel_range, fPoint from) const
{
	return origin->SearchFirst(pixel_range, from);
}

Entity * QuadTree::SearchFirstEnemy(int pixel_range, fPoint from, Side side) const
{
	return origin->SearchFirstEnemy(pixel_range, from, side);
}

void QuadTree::Search(int pixel_range, fPoint from, std::vector<Entity*>& vec) const
{
	origin->Search(pixel_range, from, vec);
}

void QuadTree::Search(SDL_Rect rect, std::vector<Entity*>& vec) const
{
	origin->Search(rect, vec);
}

void QuadTree::UpdateAll(float dt) const
{	
	origin->Update(dt);
}

void QuadTree::DeleteEntities() const
{
	origin->DeleteEntities();
}

void QuadTree::CheckCollisions() const
{
	origin->CheckCollision();
}

void QuadTree::CheckUnitCollisions(const Unit * ptr) const
{
	origin->CheckUnitCollisions(ptr);
}

void QuadTree::DrawRects() const
{
	origin->DrawArea();
}
