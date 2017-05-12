#ifndef _ENTITY
#define _ENTITY

#include "SDL\include\SDL.h"
#include "p2Point.h"

enum TextureID;

enum ENTITY_TYPE
{
	E_NO_ENTITY = 0,
	E_BUILDING,
	E_UNIT,
	E_RESOURCE,
};

enum Side
{
	S_ALLY,
	S_ENEMY,
	S_NEUTRAL,
	S_NO_SIDE
};

enum ENTITY_STATUS
{
	ST_SELECTED = 0,
	ST_NON_SELECTED
};

class Entity
{
private:
	SDL_Rect rect;
	iPoint pivot;
	bool to_delete = false;
	fPoint position;
	int hp = 1;
	int armor = 0;
	int attack = 0;
	enum ENTITY_TYPE entity_type;
	enum ENTITY_STATUS entity_status = ST_NON_SELECTED;
	enum TextureID texture_id;
	enum Side side;
	float Arrow_pos = 0; //It goes from 0 to 1
	float ai_dt = 0;
public:
	Entity(ENTITY_TYPE entity_type, fPoint pos, Side side);

	~Entity();

	virtual void Update(float dt) = 0;
	virtual void AI() = 0;
	virtual void Draw() = 0;

	virtual void Die();//to_delete = true

	void DT(float dt);
	void ResetDT();

	//Geters
	bool ToDelete() const;

	void SetEntityStatus(ENTITY_STATUS status);
	void SetPosition(float x, float y);
	float GetArrowPos() const;
	void ResetArrowPos();
	void UpdateArrow(int StartHeight, fPoint TargetPos, int CurveHeight, float TimeSecs);

	ENTITY_TYPE GetEntityType() const;
	ENTITY_STATUS GetEntityStatus();
	const int GetHp() const;
	const int GetArmor() const;
	void IncreaseArmor(int extra_defense);
	const float GetX() const;
	const float GetY() const;
	const fPoint GetPosition() const;
	const int GetAttack() const;
	const iPoint GetPivot() const;
	const SDL_Rect GetRect() const;
	const TextureID GetTextureID() const;
	const Side GetSide() const;
	const float GetAIDT() const;

	void Attack(Entity*);
	void Damaged(int dmg);
	void UpgradeUnit(int plushealth);
protected:
	//Seters
	void SetArmor(int new_armor);
	void SetHp(int new_hp);
	void SetAttack(int new_attack);
	void SetPivot(int x, int y);
	void SetRect(SDL_Rect rect);
	void SetTextureID(TextureID id);
	void SetSide(Side side);
	void SetToDelete();
};
#endif
