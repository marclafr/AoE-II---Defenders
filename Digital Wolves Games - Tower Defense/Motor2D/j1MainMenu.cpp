#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1UIManager.h"
#include "j1MainMenu.h"
#include "j1Scene.h"

j1MainMenu::j1MainMenu() : j1Module()
{
	name.assign("main_menu");
}

// Destructor
j1MainMenu::~j1MainMenu()
{}

// Called before render is available
bool j1MainMenu::Awake()
{
	LOG("Loading Main Menu");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1MainMenu::Start()
{
	background = (UIImage*)App->uimanager->addUIComponent(UIComponent_TYPE::UIIMAGE);
	background->Set({0,0, 1366, 768}, { 0,0, 1366, 768 });
	background->interactive = false;

	singleplayer = (UIButton*)App->uimanager->addUIComponent(UIComponent_TYPE::UIBUTTON);
	singleplayer->Set({ 539, 17, 177, 240 }, { 414, 769, 177, 240 });

	return true;
}

// Called each loop iteration
bool j1MainMenu::PreUpdate()
{

	return true;
}

// Called each loop iteration
bool j1MainMenu::Update(float dt)
{


	return true;
}

// Called each loop iteration
bool j1MainMenu::PostUpdate()
{
	bool ret = true;

	//Change Scene MainMenu to Scene
	if (singleplayer->clicked == true)
	{
		DisableAllUIElements();
		active = false;
		App->scene->EnableScene();
	}

	return ret;
}

// Called before quitting
bool j1MainMenu::CleanUp()
{
	LOG("Freeing Main Menu");

	return true;
}

void j1MainMenu::DisableAllUIElements()
{
	background->draw = false;
	singleplayer->draw = false;
}

