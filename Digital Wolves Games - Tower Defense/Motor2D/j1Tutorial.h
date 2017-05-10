#ifndef __j1TUTORIAL_H__
#define __j1TUTORIAL_H__

#include "j1Module.h"

struct SDL_Texture;
class UILabel;
class UIComponents;
class UIButton;


class j1Tutorial : public j1Module
{
public:

	j1Tutorial();

	// Destructor
	virtual ~j1Tutorial();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	void TutorialsProgression();

	void TutorialReset();
	void Tutorial1();
	void Tutorial2();
	void Tutorial3();
	void Tutorial4();

	bool tutorial = false;
	UIComponents* tutorial1;
	UILabel* text_tutorial1;
	UILabel* text1_tutorial1;
	UILabel* text2_tutorial1;
	UILabel* text3_tutorial1;
	UILabel* text4_tutorial1;

	UIComponents* tutorial2;
	UILabel* text_tutorial2;
	UILabel* text1_tutorial2;
	UILabel* text2_tutorial2;

	UIComponents* tutorial3;
	UILabel* text_tutorial3;
	UILabel* text1_tutorial3;
	UILabel* text2_tutorial3;
	UILabel* text3_tutorial3;
	UILabel* text4_tutorial3;

	UIComponents* tutorial4;
	UILabel* text_tutorial4;
	UILabel* text1_tutorial4;
	UILabel* text2_tutorial4;

	bool TownHallSelected = false;
	bool PanelSelected = false;
	bool UniversitySelected = false;

	bool tutorial1_completed = false;
	bool tutorial2_completed = false;
	bool tutorial3_completed = false;
	bool tutorial4_completed = false;

private:
	int count = 0;

	
	bool tutorial1_part1 = false;
	bool tutorial1_part2 = false;
	bool tutorial1_part3 = false;
	bool tutorial1_part4 = false;

	bool tutorial2_part1 = false;

	bool tutorial3_part1 = false;
	bool tutorial3_part2 = false;
	bool tutorial3_part3 = false;
	bool tutorial3_part4 = false;

	bool tutorial4_part1 = false;
	bool tutorial4_part2 = false;

};

#endif // __j1MAINMENU_H__