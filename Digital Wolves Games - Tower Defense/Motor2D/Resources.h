#ifndef _RESOURCES
#define _RESOURCES

#include "SDL/include/SDL_rect.h"
#include "Entity.h"
#include "j1Textures.h"
#include "j1Module.h"
#include "Entity.h"
#include "Units.h"
#include "j1PerfTimer.h"
#include <vector>

enum RESOURCE_TYPE
{
	NO_RESOURCE,
	WOOD,
	STONE,
	GOLD,
	FOOD,
};

class Resources : public Entity
{
private:
	int resource = 1000;
	int amount_collected;
	enum RESOURCE_TYPE resource_type;
	float collect_time;
	float build_time;
	j1PerfTimer buildtimer;
	j1PerfTimer CollectTimer;
	bool totallybuilded = false;
public:

	Resources(RESOURCE_TYPE b_type, fPoint pos);

	void Update(); // defines order

	virtual void AI();
	virtual void Draw();

	const RESOURCE_TYPE GetResourceType() const;
	const double GetBuildTime() const;
	void AddResource(int add);
	bool CanUseResource(int cost);
	bool UseResource(int cost);
	int GetResource();
};

#endif
