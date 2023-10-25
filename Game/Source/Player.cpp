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

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool Player::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);

	pbody = app->physics->CreateCircle(position.x + 16, position.y + 16, 16, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;

	pickCoinFxId = app->audio->LoadFx("Assets/Audio/Fx/retro-video-game-coin-pickup-38299.ogg");

	return true;
}

bool Player::Update(float dt)
{
	b2Vec2 vel = b2Vec2(0, 0);
	if (isGodmode == true)
	{
		vel = b2Vec2(0, 0);
	}
	else if (isGodmode == false){
		 vel = b2Vec2(0, -GRAVITY_Y);
	}

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
		if (isGodmode == true)
		{
			vel = b2Vec2(0, -speed * dt);
		}
	}
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
		if (isGodmode == true)
		{
			vel = b2Vec2(0, speed * dt);
		}
	}
	
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		if (isGodmode == true)
		{
			vel = b2Vec2(-speed * dt, 0);
		}
		else {
			vel = b2Vec2(-speed * dt, -GRAVITY_Y);
		}
		
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		if (isGodmode == true)
		{
			vel = b2Vec2(speed * dt, 0);
		}
		else {
			vel = b2Vec2(speed * dt, -GRAVITY_Y);
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		vel = b2Vec2(+0.0f, speed*dt);
	}

	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) {
		position.x = parameters.attribute("x").as_int();
		position.y = parameters.attribute("y").as_int();
	}
	
	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) {
		position.x = parameters.attribute("x2").as_int();
		position.y = parameters.attribute("y2").as_int();
	}
	
	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) {
		position.x = parameters.attribute("x3").as_int();
		position.y = parameters.attribute("y3").as_int();
	}

	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) {
		if (isGodmode == false) { isGodmode = true;}
		else if (isGodmode == true){ isGodmode = false;}
	}

	//Set the velocity of the pbody of the player
	pbody->body->SetLinearVelocity(vel);

	//Update player position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	app->render->DrawTexture(texture, position.x, position.y);

	return true;
}

bool Player::CleanUp()
{

	return true;
}

void Player::OnCollision(PhysBody* physA, PhysBody* physB) {

	switch (physB->ctype)
	{
	case ColliderType::ITEM:
		LOG("Collision ITEM");
		app->audio->PlayFx(pickCoinFxId);
		break;
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	}
}