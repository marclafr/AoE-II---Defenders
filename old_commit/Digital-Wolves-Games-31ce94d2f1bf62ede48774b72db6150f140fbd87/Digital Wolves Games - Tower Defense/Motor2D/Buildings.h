#ifndef _BUILDINGS
#define _BUILDINGS

#include "SDL/include/SDL_rect.h"
#include "Entity.h"
#include "j1Textures.h"
#include "j1Module.h"
#include "Entity.h"
#include "Units.h"
#include "j1PerfTimer.h"
#include <vector>
#include "j1Investigations.h"

enum BUILDING_TYPE
{
	B_NO_BUILDING = 0,
	B_TURRET,
	B_CANNON,
	B_STONE_WALL,
	B_TOWNHALL//PER TENIR ALGO
};

class Building : public Entity
{
private:

	enum BUILDING_TYPE building_type;
	float rate_of_fire;
	float range;
	float build_time;
	j1PerfTimer buildtimer;
	j1PerfTimer AttackTimer;
	j1Timer DieTimer;
	bool alive = true;
	Entity* Target = nullptr;
	bool totallybuilded = false;
	enum LEVEL lvl = INV_LVL_1;
	int base_height = 0;
	int base_width = 0;
public:
	iPoint arrowpos;
	Building(BUILDING_TYPE b_type, fPoint pos, Side side);

	void Update(); // defines order
	void UpdateArrow(iPoint &arrow, iPoint Target);
	void UpdateArrowSprite(iPoint Target, SDL_Rect& rect, iPoint &pivots);
	virtual void AI();
	virtual void Draw();
	bool attacking = false;
	const BUILDING_TYPE GetBuildingType() const;
	const int GetRange() const;
	void UpgradeTurret();
	void UpgradeWall();
	const LEVEL GetLevel() const;
	const double GetBuildTime() const;
	const int GetHeight() const;
	const int GetWidth() const;
};

#endif

