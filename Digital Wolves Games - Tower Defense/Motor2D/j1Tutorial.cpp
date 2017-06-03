#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "Camera.h"
#include "j1Window.h"
#include "j1Scene.h"
#include "j1ScoreScene.h"
#include "j1EntityManager.h"
#include "j1WaveManager.h"
#include "j1Tutorial.h"
#include "j1SceneManager.h"
#include "Task.h"

#include "UIButton.h"
#include "UILabel.h"
#include "j1UIManager.h"

j1Tutorial::j1Tutorial() : j1Module()
{
	name.assign("tutorial");
}

// Destructor
j1Tutorial::~j1Tutorial()
{}

// Called before render is available
bool j1Tutorial::Awake()
{
	LOG("Loading Tutorial");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Tutorial::Start()
{

	return true;
}

// Called each loop iteration
bool j1Tutorial::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Tutorial::Update(float dt)
{

	Tutorial1();
	if (tutorial1_completed)
	{
		Tutorial2();
		if (tutorial2_completed)
		{
			Tutorial3();
			if (tutorial3_completed)
			{
				Tutorial4();
				if (tutorial4_completed)
				{
					Tutorial5();
					if (tutorial5_completed)
					{
						Tutorial6();
						if (tutorial6_completed)
						{
							Tutorial7();
						}
					}
				}
			}
		}
	}

	if (tutorial1_part1 && tutorial1_part2 && tutorial1_part3)
	{
		tutorial1_completed = true;
		tutorial1_part1, tutorial1_part2, tutorial1_part3 = false;
	}

	if (tutorial3_part1 && tutorial3_part2 && tutorial3_part3 && tutorial3_part4)
	{
		tutorial3_completed = true;
		tutorial3_part1, tutorial3_part2, tutorial3_part3, tutorial3_part4 = false;
	}

	if (tutorial4_part2 && tutorial4_part3)
	{
		tutorial4_completed = true;
		tutorial4_part2, tutorial4_part3 = false;
	}

	if (tutorial5_part2 && tutorial5_part3 && App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		tutorial5_completed = true;
		tutorial5_part2, tutorial5_part3 = false;
	}

	if (tutorial6_part2 && tutorial6_part3)
	{
		tutorial6_completed = true;
		tutorial6_part2, tutorial6_part3 = false;
	}

	TutorialsProgression();

	return true;
}

// Called each loop iteration
bool j1Tutorial::PostUpdate()
{
	return true;
}

// Called before quitting
bool j1Tutorial::CleanUp()
{
	LOG("Freeing  MainMenu");
	App->uimanager->SetAllToDelete();
	tutorial = false;
	return true;
}


void j1Tutorial::TutorialsProgression()
{
	if (tutorial1_completed)
	{
		tutorial1->SetToDelete();
		text_tutorial1->SetToDelete();
		text1_tutorial1->SetToDelete();
		text2_tutorial1->SetToDelete();
		text3_tutorial1->SetToDelete();
	}

	if (tutorial2_completed)
	{
		tutorial2->SetToDelete();
		text_tutorial2->SetToDelete();
		text1_tutorial2->SetToDelete();
		text2_tutorial2->SetToDelete();
	}

	if (tutorial3_completed)
	{
		tutorial3->SetToDelete();
		text_tutorial3->SetToDelete();
		text1_tutorial3->SetToDelete();
		text2_tutorial3->SetToDelete();
		text3_tutorial3->SetToDelete();
		text4_tutorial3->SetToDelete();
	}

	if (tutorial4_completed)
	{
		tutorial4->SetToDelete();
		text_tutorial4->SetToDelete();
		text1_tutorial4->SetToDelete();
		text2_tutorial4->SetToDelete();
		text3_tutorial4->SetToDelete();
		text4_tutorial4->SetToDelete();
	}

	if (tutorial5_completed)
	{
		tutorial5->SetToDelete();
		text_tutorial5->SetToDelete();
		text1_tutorial5->SetToDelete();
		text2_tutorial5->SetToDelete();
		text3_tutorial5->SetToDelete();
	}

	if (tutorial6_completed)
	{
		tutorial6->SetToDelete();
		text_tutorial6->SetToDelete();
		text1_tutorial6->SetToDelete();
		text2_tutorial6->SetToDelete();
		text3_tutorial6->SetToDelete();
	}

	if (tutorial7_completed)
	{
		tutorial7->SetToDelete();
		text_tutorial7->SetToDelete();
		text1_tutorial7->SetToDelete();
	}
}

void j1Tutorial::TutorialReset()
{
	tutorial1_completed = false;
	tutorial1_part1 = false;
	tutorial1_part2 = false;
	tutorial1_part3 = false;
	tutorial2_completed = false;
	tutorial2_part1 = false;
	TownHallSelected = false;
	PanelSelected = false;
	tutorial3_completed = false;
	tutorial3_part1 = false;
	tutorial3_part2 = false;
	tutorial3_part3 = false;
	tutorial3_part4 = false;
	tutorial4_completed = false;
	tutorial4_part1 = false;
	tutorial4_part2 = false;
	tutorial4_part3 = false;
	UniversitySelected = false;
	InvestigationSelected = false;
	InvestigationDone = false;
	tutorial5_completed = false;
	tutorial5_part1 = false;
	tutorial5_part2 = false;
	tutorial5_part3 = false;
	TowerSelected = false;
	TowerUpgradeSelected = false;
	tutorial6_completed = false;
	tutorial6_part1 = false;
	tutorial6_part2 = false;
	tutorial6_part3 = false;
	NextWaveButtonSelected = false;
	tutorial7_completed = false;

}

void j1Tutorial::Tutorial1()
{
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN) {
		tutorial1_part1 = true;
		text1_tutorial1->ChangeText("Press 1 to build a Simple Tower Done");
	}

	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN) {
		tutorial1_part2 = true;
		text2_tutorial1->ChangeText("Press 2 to build a Bombard Tower Done");
	}

	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN) {
		tutorial1_part3 = true;
		text3_tutorial1->ChangeText("Press 3 to build a Wall Done");
	}
}

void j1Tutorial::Tutorial2()
{
	if (TownHallSelected) {
		tutorial2_part1 = true;
		text_tutorial2->ChangeText("Click on the Town Hall Done");
	}

	if (PanelSelected) {
		tutorial2_completed = true;
	}
}

void j1Tutorial::Tutorial3()
{
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) {
		tutorial3_part1 = true;
		text1_tutorial3->ChangeText("W Done");
	}

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN) {
		tutorial3_part2 = true;
		text2_tutorial3->ChangeText("A Done");
	}

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) {
		tutorial3_part3 = true;
		text3_tutorial3->ChangeText("S Done");
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN) {
		tutorial3_part4 = true;
		text4_tutorial3->ChangeText("D Done");
	}
}

void j1Tutorial::Tutorial4()
{
	if (!tutorial4_part1)
	{
		App->render->camera->SetPosition(iPoint(-700, -500));
		tutorial4_part1 = true;
	}
	if (UniversitySelected) {
		tutorial4_part2 = true;
		text1_tutorial4->ChangeText("Select the University Done");
	}

	if (InvestigationDone)
	{
		tutorial4_part3 = true;
		text2_tutorial4->ChangeText("Click the fire upgrade on the panel Done");
	}
}

void j1Tutorial::Tutorial5()
{
	if (!tutorial5_part1)
	{
		App->render->camera->SetPosition(iPoint(700, 20));
		tutorial5_part1 = true;
	}

	if (TowerSelected) {
		tutorial5_part2 = true;
		text_tutorial5->ChangeText("Click one tower Done");
	}
	if (TowerUpgradeSelected)
	{
		tutorial5_part3 = true;
		text1_tutorial5->ChangeText("Click the fire upgrade icon Done");
	}
}

void j1Tutorial::Tutorial6()
{
	App->wave_manager->Enable(); 
	tutorial = false;
	if (!tutorial6_part1)
	{
		App->render->camera->SetPosition(iPoint(1700, -2400));
		tutorial6_part1 = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		tutorial6_part2 = true;
		text1_tutorial6->ChangeText("Build a tower on the highground Done");
	}
	if (NextWaveButtonSelected)
	{
		tutorial6_part3 = true;
		text3_tutorial6->ChangeText("Click next wave button (big red button) Done");
	}
}

void j1Tutorial::Tutorial7()
{
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		tutorial7_completed = true;
	}
}