#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1PathFinding.h"

j1PathFinding::j1PathFinding() : j1Module(), map(NULL), last_path(DEFAULT_PATH_LENGTH),width(0), height(0)
{
	name.assign("pathfinding");
}

// Destructor
j1PathFinding::~j1PathFinding()
{
	RELEASE_ARRAY(map);
}

// Called before quitting
bool j1PathFinding::CleanUp()
{
	LOG("Freeing pathfinding library");

	last_path.clear();
	RELEASE_ARRAY(map);
	return true;
}

// Sets up the walkability map
void j1PathFinding::SetMap(uint width, uint height, uchar* data)
{
	this->width = width;
	this->height = height;

	RELEASE_ARRAY(map);
	map = new uchar[width*height];
	memcpy(map, data, width*height);
}

// Utility: return true if pos is inside the map boundaries
bool j1PathFinding::CheckBoundaries(const iPoint& pos) const
{
	return (pos.x >= 0 && pos.x <= (int)width &&
			pos.y >= 0 && pos.y <= (int)height);
}

// Utility: returns true is the tile is walkable
bool j1PathFinding::IsWalkable(const iPoint& pos) const
{
	uchar t = GetTileAt(pos);
	return t != INVALID_WALK_CODE && t > 0;
}

// Utility: return the walkability value of a tile
uchar j1PathFinding::GetTileAt(const iPoint& pos) const
{
	if(CheckBoundaries(pos))
		return map[(pos.y*width) + pos.x];

	return INVALID_WALK_CODE;
}

// To request all tiles involved in the last generated path
const std::vector<iPoint>* j1PathFinding::GetLastPath() const
{
	return &last_path;
}

// PathList ------------------------------------------------------------------------
// Looks for a node in this list and returns it's list node or NULL
// ---------------------------------------------------------------------------------
p2List_item<PathNode>* PathList::Find(const iPoint& point) const
{
	p2List_item<PathNode>* item = list.start;
	while(item)
	{
		if(item->data.pos == point)
			return item;
		item = item->next;
	}
	return NULL;
}

// PathList ------------------------------------------------------------------------
// Returns the Pathnode with lowest score in this list or NULL if empty
// ---------------------------------------------------------------------------------
p2List_item<PathNode>* PathList::GetNodeLowestScore() const
{
	p2List_item<PathNode>* ret = NULL;
	int min = 65535;

	p2List_item<PathNode>* item = list.end;
	while(item)
	{
		if(item->data.Score() < min)
		{
			min = item->data.Score();
			ret = item;
		}
		item = item->prev;
	}
	return ret;
}

// PathNode -------------------------------------------------------------------------
// Convenient constructors
// ----------------------------------------------------------------------------------
PathNode::PathNode() : g(-1), h(-1), pos(-1, -1), parent(NULL)
{}

PathNode::PathNode(int g, int h, const iPoint& pos, const PathNode* parent) : g(g), h(h), pos(pos), parent(parent)
{}

PathNode::PathNode(const PathNode& node) : g(node.g), h(node.h), pos(node.pos), parent(node.parent)
{}

// PathNode -------------------------------------------------------------------------
// Fills a list (PathList) of all valid adjacent pathnodes
// ----------------------------------------------------------------------------------
uint PathNode::FindWalkableAdjacents(PathList& list_to_fill) const
{
	iPoint cell;
	uint before = list_to_fill.list.count();

	// north
	cell.create(pos.x, pos.y + 1);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	//north east
	cell.create(pos.x + 1, pos.y + 1);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	//north west
	cell.create(pos.x - 1, pos.y + 1);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	// south
	cell.create(pos.x, pos.y - 1);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	// south east
	cell.create(pos.x + 1, pos.y - 1);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	// south west
	cell.create(pos.x - 1, pos.y - 1);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	// east
	cell.create(pos.x + 1, pos.y);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	// west
	cell.create(pos.x - 1, pos.y);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	return list_to_fill.list.count();
}

// PathNode -------------------------------------------------------------------------
// Calculates this tile score
// ----------------------------------------------------------------------------------
int PathNode::Score() const
{
	return g + h;
}

// PathNode -------------------------------------------------------------------------
// Calculate the F for a specific destination tile
// ----------------------------------------------------------------------------------
int PathNode::CalculateF(const iPoint& destination)
{
	g = parent->g + 1;
	h = pos.DistanceTo(destination);

	return g + h;
}

// ----------------------------------------------------------------------------------
// Actual A* algorithm: return number of steps in the creation of the path or -1 ----
// ----------------------------------------------------------------------------------
int j1PathFinding::CreatePath(const iPoint& origin, const iPoint& destination)
{
	if (App->pathfinding->IsWalkable(origin) == false || App->pathfinding->IsWalkable(destination) == false)
		return -1;

	int ret = 0;

	
	iPoint save_destination = destination;
	iPoint save_origin = origin;
	last_path.clear();
	// Adds the origin tile to open
	// Iterate while we have tile in the open list
	PathList open;
	PathList close;
	PathList neighbors;
	open.list.add(PathNode(0, 0, origin, NULL));
	
	while (open.list.count() > 0)
	{
		//Moves the lowest score cell from open list to the closed list
		p2List_item<PathNode>* lowest = open.GetNodeLowestScore();
		p2List_item<PathNode>* node = close.list.add(lowest->data);
		open.list.del(lowest);

		// If we just added the destination, we are done!
		// Backtrack to create the final path
		// Use the Pathnode::parent and Flip() the path when you are finish
		if (close.list.end->data.pos == destination)
		{
			while (node->data.pos != origin && node->data.pos != save_origin)
			{
				last_path.push_back(node->data.pos);
				node->data = *node->data.parent;
			}
			last_path.push_back(origin);

			std::vector<iPoint>::iterator start = last_path.begin();
			start++;
			std::vector<iPoint>::iterator end = last_path.end();
			end--;
			while (start < end)
				SWAP(*start++, *end--);
			//last_path.Flip();
			ret = last_path.size();
			break;
		}

		else
		{
			//Fill a list of all adjancent nodes
			neighbors.list.clear();
			node->data.FindWalkableAdjacents(neighbors);

			// Iterate adjancent nodes:
			// ignore nodes in the closed list
			// If it is NOT found, calculate its F and add it to the open list
			// If it is already in the open list, check if it is a better path (compare G)
			// If it is a better path, Update the parent
			p2List_item<PathNode>* temp = neighbors.list.start;
			for (; temp; temp = temp->next)
			{
				if (close.Find(temp->data.pos) == NULL)
				{
					temp->data.CalculateF(destination);

					p2List_item<PathNode>* item = open.Find(temp->data.pos);
					if (item == NULL)
					{
						open.list.add(temp->data);
					}
					else
					{
						if (item->data.g > temp->data.g)
						{
							item->data.parent = temp->data.parent;
						}
					}
				}
			}
		}
	}

	return ret;
}

int j1PathFinding::CreatePath(const iPoint& origin, const iPoint& destination, p2List<iPoint> lista)
{
	if (App->pathfinding->IsWalkable(origin) == false || App->pathfinding->IsWalkable(destination) == false)
		return -1;

	int ret = 0;


	iPoint save_destination = destination;
	iPoint save_origin = origin;
	last_path.clear();
	// Adds the origin tile to open
	// Iterate while we have tile in the open list
	PathList open;
	PathList close;
	PathList neighbors;
	open.list.add(PathNode(0, 0, origin, NULL));

	while (open.list.count() > 0)
	{
		//Moves the lowest score cell from open list to the closed list
		p2List_item<PathNode>* lowest = open.GetNodeLowestScore();
		p2List_item<PathNode>* node = close.list.add(lowest->data);
		open.list.del(lowest);

		// If we just added the destination, we are done!
		// Backtrack to create the final path
		// Use the Pathnode::parent and Flip() the path when you are finish
		if (close.list.end->data.pos == destination)
		{
			while (node->data.pos != origin && node->data.pos != save_origin)
			{
				last_path.push_back(node->data.pos);
				node->data = *node->data.parent;
			}
			last_path.push_back(origin);

			std::vector<iPoint>::iterator start = last_path.begin();
			start++;
			std::vector<iPoint>::iterator end = last_path.end();
			end--;
			while (start < end)
				SWAP(*start++, *end--);
			//last_path.Flip();
			std::vector<iPoint>::iterator start2 = last_path.begin();

			while (start2 != last_path.end()) {
				iPoint point(start2->x, start2->y);
				lista.add(point);
				*start2++;
			}

			ret = last_path.size();
			break;
		}

		else
		{
			//Fill a list of all adjancent nodes
			neighbors.list.clear();
			node->data.FindWalkableAdjacents(neighbors);

			// Iterate adjancent nodes:
			// ignore nodes in the closed list
			// If it is NOT found, calculate its F and add it to the open list
			// If it is already in the open list, check if it is a better path (compare G)
			// If it is a better path, Update the parent
			p2List_item<PathNode>* temp = neighbors.list.start;
			for (; temp; temp = temp->next)
			{
				if (close.Find(temp->data.pos) == NULL)
				{
					temp->data.CalculateF(destination);

					p2List_item<PathNode>* item = open.Find(temp->data.pos);
					if (item == NULL)
					{
						open.list.add(temp->data);
					}
					else
					{
						if (item->data.g > temp->data.g)
						{
							item->data.parent = temp->data.parent;
						}
					}
				}
			}
		}
	}

	return ret;
}
