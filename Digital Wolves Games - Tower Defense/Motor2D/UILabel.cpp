#include "UILabel.h"

#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "Camera.h"

UILabel::UILabel(UICOMPONENT_TYPE type) : UIComponents(type) {}

UILabel::~UILabel()
{
	if (font != nullptr)
		delete font;
}

void UILabel::Set(int pos_x, int pos_y, const char * text, _TTF_Font*  font)
{
	rect_position.x = pos_x;
	rect_position.y = pos_y;

	text_img = App->font->Print(text, T_AOE_UI_FONT, { 255,0,0,0 });

	this->text.assign(text);

	App->render->GetDimensionsFromTexture(text_img, rect_position.w, rect_position.h);
}

void UILabel::Draw()
{
	App->render->Blit(text_img, rect_position.x - App->render->camera->GetPosition().x, rect_position.y - App->render->camera->GetPosition().y, 0, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, true);
}

void UILabel::ChangeText(const char* text)
{
	if (this->text != text)
	{
		text_img = App->font->Print(text, T_AOE_UI_FONT, { 255,0,0,0 });

		App->render->GetDimensionsFromTexture(text_img, rect_position.w, rect_position.h);

		this->text.erase();
		this->text.assign(text);
	}
}