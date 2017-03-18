#ifndef __j1RENDER_H__
#define __j1RENDER_H__

#include "SDL/include/SDL.h"
#include "p2Point.h"
#include "j1Module.h"
#include <deque>



class Sprite
{
private:
	iPoint position;
	SDL_Rect* section;
	SDL_Texture* texture; 
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	int pivot_x = 0;
	int pivot_y = 0;
	float speed = 1.0f;
	double angle = 0;

public:
	Sprite(SDL_Texture* texture, int x, int y, const SDL_Rect* section = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE, int pivot_x = 0, int pivot_y = 0, float speed = 1.0f, double angle = 0);
	Sprite();
	Sprite(const Sprite &copy);
	~Sprite();

	void operator = (Sprite& sprite);

	iPoint GetPosition() const;
	SDL_Texture* GetTexture() const;
	const SDL_Rect* GetSection() const;
	SDL_RendererFlip GetFlip() const;
	int GetPivotX() const;
	int GetPivotY() const;
	float GetSpeed() const;
	double GetAngle() const;

};

class j1Render : public j1Module
{
public:

	j1Render();

	// Destructor
	virtual ~j1Render();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Load / Save
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	// Utils
	void SetViewPort(const SDL_Rect& rect);
	void ResetViewPort();
	iPoint ScreenToWorld(int x, int y) const;

	// Draw & Blit
	bool Blit(SDL_Texture* texture, int x, int y, const SDL_Rect* section = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE, int pivot_x = 0, int pivot_y = 0, float speed = 1.0f, double angle = 0) const;
	bool DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool filled = true, bool use_camera = true) const;
	bool DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool use_camera = true) const;
	bool DrawCircle(int x1, int y1, int redius, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool use_camera = true) const;

	void PushSprite(SDL_Texture* texture, int x, int y, const SDL_Rect* section = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE, int pivot_x = 0, int pivot_y = 0, float speed = 1.0f, double angle = 0);
	void BlitAllEntities();

	// Set background color
	void SetBackgroundColor(SDL_Color color);

	//Get width and height from texture
	void GetDimensionsFromTexture(SDL_Texture* tex, int& w, int& h);

public:

	SDL_Renderer*	renderer;
	SDL_Rect		camera;
	SDL_Rect		viewport;
	SDL_Color		background;

private:
	std::deque<Sprite> sprite_queue;
};


#endif // __j1RENDER_H__