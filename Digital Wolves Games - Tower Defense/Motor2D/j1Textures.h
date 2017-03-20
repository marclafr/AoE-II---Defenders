#ifndef __j1TEXTURES_H__
#define __j1TEXTURES_H__

#include "j1Module.h"
#include "p2List.h"
#include <vector>

struct SDL_Texture;
struct SDL_Surface;

enum TextureID
{
	T_UI,
	T_MAP,
	T_TILESET,
	T_AOE_UI_FONT,
	T_TURRET,
	T_VILLAGER,
	T_TWOHANDEDSWORDMAN,
	T_CAVALRYARCHER,
	T_SIEGERAM
};


struct Texture
{
private:
	SDL_Texture* texture;
	TextureID id;

public:

	Texture(SDL_Texture* tex, TextureID id);
	Texture(const Texture& copy);
	~Texture();

	TextureID GetID() const;
	SDL_Texture* GetTexture() const;

};

class j1Textures : public j1Module
{
public:

	j1Textures();

	// Destructor
	virtual ~j1Textures();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called before quitting
	bool CleanUp();

	// Load Texture
	SDL_Texture* const	Load(const char* path, TextureID id);
	bool UnLoad(Texture* texture);
	bool UnLoad(TextureID id);
	SDL_Texture* const	LoadSurface(SDL_Surface* surface, TextureID id);

	void GetSize(const SDL_Texture* texture, uint& width, uint& height) const;
	SDL_Texture* GetTexture(const TextureID id) const;

private:

	std::vector<Texture*> textures;
};


#endif // __j1TEXTURES_H__