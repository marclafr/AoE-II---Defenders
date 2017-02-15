#ifndef __j1FONTS_H__
#define __j1FONTS_H__

#include "d1Module.h"
#include "SDL\include\SDL_pixels.h"
#include <list>

#define DEFAULT_FONT "fonts/open_sans/OpenSans-Regular.ttf"
#define DEFAULT_FONT_SIZE 12

struct SDL_Texture;
struct _TTF_Font;

class d1Fonts : public d1Module
{
public:

	d1Fonts();

	// Destructor
	virtual ~d1Fonts();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

	// Load Font
	_TTF_Font* const Load(const char* path, int size = 12);

	// Create a surface from text
	SDL_Texture* Print(const char* text, SDL_Color color = {255, 255, 255, 255}, _TTF_Font* font = NULL);

	bool CalcSize(const char* text, int& width, int& height, _TTF_Font* font = NULL) const;

public:

	std::list<_TTF_Font*> fonts;
	_TTF_Font*			default;
};


#endif // __j1FONTS_H__