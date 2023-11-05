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

	float jumpForce = 20.0f;
	bool isGodmode = false;
};

#endif // __PLAYER_H__