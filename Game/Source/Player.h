#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "animation.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class Player : public Entity
{
private:
	Animation* currentAnimation = nullptr;
	Animation
		idleAnim,
		jumpAnimR,
		jumpAnimL,
		runAnimR,
		runAnimL,
		attackAnimR,
		attackAnimL,
		playerDeath;
public:

	Player();
	
	virtual ~Player();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);


public:
	float speed = 0.2f;
	const char* texturePath;
	SDL_Texture* texture = NULL;
	PhysBody* pbody;
	int pickCoinFxId;

	// L07 DONE 5: Add physics to the player - declare a Physics body
	PhysBody* pbody;
	int hp = 100;
	float speed = 2.0f;
	float godModeSpeed = 7.5f;
	float jumpForce = 5.0f;
	float maxJumpForce = 55.0f;
	bool canJump = false;
	bool jumping = false;
	bool isGodmode = false;
};

#endif // __PLAYER_H__