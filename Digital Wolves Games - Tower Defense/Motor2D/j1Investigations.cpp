#include "j1Investigations.h"
#include "j1App.h"
#include "Resources.h"
#include "j1Scene.h"
#include "j1Audio.h"
#include "j1Tutorial.h"

j1Investigations::j1Investigations()
{
	name.assign("investigations");
}

j1Investigations::~j1Investigations()
{
}

bool j1Investigations::Start()
{
	bool ret = true;

	//SOUNDS
	fx_inv_completed = App->audio->LoadFx("audio/fx/Investigation.wav");
	//--

	//RESOURCES
	CreateInvestigation(INV_FOOD, true, 300, 5.0);
	CreateInvestigation(INV_WOOD, true, 300, 5.0);
	CreateInvestigation(INV_GOLD, true, 300, 5.0);
	CreateInvestigation(INV_STONE, true, 300, 5.0);
	//TROPS
//	CreateInvestigation(INV_CAVALRY_UNLOCK, false, 150, 15.0);
	CreateInvestigation(INV_CAVALRY_ATTACK, false, 300, 30.0);
	CreateInvestigation(INV_CAVALRY_DEFENSE, false, 300, 25.0);
//	CreateInvestigation(INV_ARCHERS_UNLOCK, false, 150, 15.0);
	CreateInvestigation(INV_ARCHERS_ATTACK, false, 300, 30.0);
	CreateInvestigation(INV_ARCHERS_DEFENSE, false, 300, 25.0);
//	CreateInvestigation(INV_INFANTRY_UNLOCK, false, 100, 15.0);
	CreateInvestigation(INV_INFANTRY_ATTACK, false, 300, 30.0);
	CreateInvestigation(INV_INFANTRY_DEFENSE, false, 300, 25.0);
	//TOWERS
	CreateInvestigation(INV_FIRE_TOWER, false, 450, 10.0);
	CreateInvestigation(INV_ICE_TOWER, false, 320, 10.0);
	CreateInvestigation(INV_AIR_TOWER, false, 400, 10.0);
	//--

	return ret;
}

bool j1Investigations::Update(float dt)
{
	bool ret = true;

	for (int i = 0; i < investigations.size(); i++)
	{
		switch (investigations[i]->inv_state)
		{
		case INV_S_IDLE:
			break;

		case INV_S_WAITING_TO_INVESTIGATE:
			CanInvestigate(investigations[i]);
			break; 

		case INV_S_IN_COURSE:
			UpgradeInvestigation(investigations[i]);
			break;

		case INV_S_COMPLETED:
			if (investigations[i]->has_levels == true)
				investigations[i]->inv_state = INV_S_IDLE;
			break;

		default:
			break;
		}
	}
	return ret;
}

bool j1Investigations::CleanUp()
{
	bool ret = true;

	for (int i = 0; i < investigations.size(); i++)
		DeleteInvestigation(investigations[i]);
	
	return ret;
}

Investigation* j1Investigations::CreateInvestigation(INVESTIGATION_TYPE investigation, bool has_lvls, uint cost, float time_to_upgrade)
{
	Investigation* new_inv = new Investigation(investigation, has_lvls, cost, time_to_upgrade);
	investigations.push_back(new_inv);

	return new_inv;
}

bool j1Investigations::DeleteInvestigation(Investigation* ptr)
{
	for (std::vector<Investigation*>::iterator it = investigations.begin(); it != investigations.end();)
	{
		if (*it == ptr)
		{
			investigations.erase(it);
			delete ptr;
			return true;
		}
		else
			it++;
	}
	return false;
}

bool j1Investigations::CanInvestigate(Investigation* investigation)
{
	if (App->scene->resources->GetGold() > investigation->cost)
	{
		investigation->inv_state = INV_S_IN_COURSE;
		DoInvestigationUpgrade(investigation);
		//TODO: If there is a visual feedback when you click an investigation delete the fx line
		App->audio->PlayFx(fx_inv_completed);
		return true;
	}

	if (investigation->investigation_level == INV_LVL_UNLOCKED)
	{
		investigation->inv_state = INV_S_COMPLETED;
	}
		
	else
		investigation->inv_state = INV_S_IDLE;
	return false;
}

void j1Investigations::DoInvestigationUpgrade(Investigation* investigation)
{
	App->scene->resources->UseResource(R_GOLD, investigation->cost);

	if (investigation->has_levels == true)
		investigation->cost += COST_INCREASE_BY_LVL;

	investigation->upgrade_timer.Start();
}

Investigation * j1Investigations::GetInvestigation(INVESTIGATION_TYPE name)
{
	for (int i = 0; i < investigations.size(); i++)
	{
		if (investigations[i]->investigation_type == name)
			return investigations[i];
	}
	return nullptr;
}

LEVEL j1Investigations::GetLevel(Investigation * investigation)
{
	return investigation->investigation_level;
}

bool j1Investigations::WantToInvestigate(Investigation * investigation)
{
	if (investigation->inv_state == INV_S_IDLE)
	{
		investigation->inv_state = INV_S_WAITING_TO_INVESTIGATE;
		return true;
	}
	return false;
}

int j1Investigations::GetInvestigationCost(Investigation* investigation)
{
	return investigation->cost;
}

bool j1Investigations::UpgradeInvestigation(Investigation* investigation)
{
	if (investigation->upgrade_timer.ReadSec() >= investigation->time_to_upgrade)
	{
		investigation->inv_state = INV_S_COMPLETED;
		App->audio->PlayFx(fx_inv_completed);

		if (investigation->has_levels == false)
		{
			investigation->investigation_level = INV_LVL_UNLOCKED;
			if (App->tutorial->tutorial3_completed && investigation->investigation_type == INV_FIRE_TOWER)
			{
				App->tutorial->InvestigationDone = true;
			}
			return true;
		}

		switch (investigation->investigation_type)
		{
		case INV_FOOD:
			if (!App->scene->resources->ReduceFoodCollectTime(2.0f))
				App->scene->resources->IncreaseResourceAmount(R_FOOD, 25.0f);
			break;
		case INV_WOOD:
			if(!App->scene->resources->ReduceWoodCollectTime(2.0f))
				App->scene->resources->IncreaseResourceAmount(R_WOOD, 25.0f);
			break;
		case INV_GOLD:
			if(!App->scene->resources->ReduceGoldCollectTime(2.0f))
				App->scene->resources->IncreaseResourceAmount(R_GOLD, 25.0f);
			break;
		case INV_STONE:
			if(!App->scene->resources->ReduceStoneCollectTime(2.0f))
				App->scene->resources->IncreaseResourceAmount(R_STONE, 25.0f);
			break;
		default:
			break;
		}
		return true;
	}
	return false;
}
