#include "j1App.h"
#include "j1Render.h"
#include "j1Fonts.h"
#include "Camera.h"
#include "j1Console.h"
#include "j1SceneManager.h"


bool j1Console::Start()
{
	return true;
}

bool j1Console::PreUpdate()
{
	return true;
}

bool j1Console::Update(float dt)
{
	
	return true;
}

bool j1Console::PostUpdate()
{
	if (on == true)
	{
		SDL_RenderDrawRect(App->render->renderer, &rect);
		SDL_SetRenderDrawColor(App->render->renderer, 0, 0, 0, 100);
		SDL_RenderFillRect(App->render->renderer, &rect);
		BlitText();
	}

	return true;
}

bool j1Console::CleanUp()
{
	return true;
}

bool j1Console::PushText(char * text)
{
	if (on)
	{
		text_textures.push_back(App->font->Print(text));
		return true;
	}
	return false;
}

void j1Console::BlitText()
{
	int x = -App->render->camera->GetPosition().x + 5;;
	int y;

	if (App->scene_manager->GetCurrentScene() == SC_GAME)
		y = -App->render->camera->GetPosition().y + 20;
	else
		y = -App->render->camera->GetPosition().y + 3;


	int height = 0;
	int width = 0;
	Uint32* format = 0;
	int* acces = 0;

	for (int i = 0; i < text_textures.size(); i++)
	{
		SDL_QueryTexture(text_textures[i], format, acces, &width, &height);
		App->render->Blit(text_textures[i],x,y);
		y += height;
		
		SDL_DestroyTexture(text_textures[i]);
	}

	text_textures.clear();
}

void j1Console::TurnOnOff()
{
	if (on == true)
		on = false;
	else
		on = true;
}

