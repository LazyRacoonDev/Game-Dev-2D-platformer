#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");
}

Player::~Player() {

}

bool Player::Awake() {

	//L03: DONE 2: Initialize Player parameters
	position = iPoint(config.attribute("x").as_int(), config.attribute("y").as_int());
	initialPosition = iPoint(config.attribute("x").as_int(), config.attribute("y").as_int());

	return true;
}

bool Player::Start() {

	texture = app->tex->Load(config.attribute("texturePath").as_string());

	// L07 DONE 5: Add physics to the player - initialize physics body
	app->tex->GetSize(texture, texW, texH);
	pbody = app->physics->CreateCircle(position.x, position.y, texW / 2, bodyType::DYNAMIC);

	// L07 DONE 6: Assign player class (using "this") to the listener of the pbody. This makes the Physics module to call the OnCollision method
	pbody->listener = this;

	// L07 DONE 7: Assign collider type
	pbody->ctype = ColliderType::PLAYER;

	//initialize audio effect
	pickCoinFxId = app->audio->LoadFx(config.attribute("coinfxpath").as_string());

	return true;
}

bool Player::Update(float dt)
{
	// L07 DONE 5: Add physics to the player - updated player position using physics

	//L03: DONE 4: render the player texture and modify the position of the player using WSAD keys and render the texture
	
	b2Vec2 velocity = b2Vec2(0, 0);
	b2Vec2 gravity(0, GRAVITY_Y);

	if (isGodmode == true)
	{
		velocity = b2Vec2(0, 0);
	}
	else if (isGodmode == false) {
		velocity = b2Vec2(0, -GRAVITY_Y);
	}

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		velocity.x = -speed;
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		velocity.x = speed;
	}

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && canJump && !jumping) {
		if (canJump)
		{
			velocity.y = -jumpForce;
			canJump = false;
			jumping = true;
		}
		LOG("JUMP");
	}

	if (jumping)
	{
		velocity.y += gravity.y * dt;
		jumping = false;
	}

	if (velocity.y < -maxJumpForce) {
		velocity.y = -maxJumpForce;
	}

	if (isGodmode == true) {
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
			velocity.y =  -godModeSpeed;
		}
		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
			velocity.y = godModeSpeed;
		}
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			velocity.x = godModeSpeed;
		}
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			velocity.x = -godModeSpeed;
		}
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
			velocity.y = -godModeSpeed;
			velocity.x = godModeSpeed;
		}
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
			velocity.y = -godModeSpeed;
			velocity.x = -godModeSpeed;
		}
		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
			velocity.y = godModeSpeed;
			velocity.x = godModeSpeed;
		}
		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
			velocity.y = godModeSpeed;
			velocity.x = -godModeSpeed;
		}
	}

	pbody->body->SetLinearVelocity(velocity);
	b2Transform pbodyPos = pbody->body->GetTransform();
	position.x = METERS_TO_PIXELS(pbodyPos.p.x) - texH / 2;
	position.y = METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2;
	app->render->DrawTexture(texture, position.x, position.y);

	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) {
		position.x = initialPosition.x;
		position.y = initialPosition.y;
		pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(position.x), PIXEL_TO_METERS(position.y)), 0);
		pbody->body->SetLinearVelocity(b2Vec2(0, 0));
	}
	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) {
		position.x = initialPosition.x;
		position.y = initialPosition.y;
		pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(position.x), PIXEL_TO_METERS(position.y)), 0);
		pbody->body->SetLinearVelocity(b2Vec2(0, 0));
	}

	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) {
		isGodmode = !isGodmode;
	}

	return true;
}

bool Player::CleanUp()
{
	return true;
}

// L07 DONE 6: Define OnCollision function for the player. 
void Player::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		canJump = true;
		break;
	case ColliderType::ITEM:
		LOG("Collision ITEM");
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	default:
		break;
	}
}