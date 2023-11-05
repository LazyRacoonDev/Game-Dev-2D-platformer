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

	idleAnim.PushBack({ 0,0,128,64 });  //Pixel X, Pixel Y, tAMAÑO x, tAMAÑO y
	idleAnim.PushBack({ 128,0,128,64 });
	idleAnim.PushBack({ 0,64,128,64 });
	idleAnim.PushBack({ 128,64,128,64 });
	idleAnim.PushBack({ 0,128,128,64 });
	idleAnim.PushBack({ 128,128,128,64 });
	idleAnim.PushBack({ 0,192,128,64 });
	idleAnim.PushBack({ 128,192,128,64 });
	idleAnim.loop = true;
	idleAnim.speed = 0.069f;

	runAnimR.PushBack({ 0,256,128,64 });  //Pixel X, Pixel Y, tAMAÑO x, tAMAÑO y
	runAnimR.PushBack({ 128,256,128,64 });
	runAnimR.PushBack({ 0,320,128,64 });
	runAnimR.PushBack({ 128,320,128,64 });
	runAnimR.PushBack({ 0,384,128,64 });
	runAnimR.PushBack({ 128,384,128,64 });
	runAnimR.PushBack({ 0,448,128,64 });
	runAnimR.PushBack({ 128,448,128,64 });
	runAnimR.loop = true;
	runAnimR.speed = 0.069f;

	runAnimL.PushBack({ 0,512,128,64 });  //Pixel X, Pixel Y, tAMAÑO x, tAMAÑO y
	runAnimL.PushBack({ 128,512,128,64 });
	runAnimL.PushBack({ 0,576,256,64 });
	runAnimL.PushBack({ 128,576,128,64 });
	runAnimL.PushBack({ 0,640,128,64 });
	runAnimL.PushBack({ 128,640,128,64 });
	runAnimL.PushBack({ 0,704,128,64 });
	runAnimL.PushBack({ 128,704,128,64 });
	runAnimL.loop = true;
	runAnimL.speed = 0.069f;

	jumpAnimR.PushBack({ 0,768,128,64 });  //Pixel X, Pixel Y, tAMAÑO x, tAMAÑO y
	jumpAnimR.PushBack({ 128,768,128,64 });
	jumpAnimR.PushBack({ 0,832,128,64 });
	jumpAnimR.PushBack({ 128,832,128,64 });
	jumpAnimR.PushBack({ 0,896,128,64 });
	jumpAnimR.PushBack({ 128,896,128,64 });
	jumpAnimR.PushBack({ 0,950,128,64 });
	jumpAnimR.PushBack({ 128,950,128,64 });
	jumpAnimR.loop = true;
	jumpAnimR.speed = 0.069f;

	jumpAnimL.PushBack({ 0,1024,128,64 });  //Pixel X, Pixel Y, tAMAÑO x, tAMAÑO y
	jumpAnimL.PushBack({ 128,1024,128,64 });
	jumpAnimL.PushBack({ 0,1088,128,64 });
	jumpAnimL.PushBack({ 128,1088,128,64 });
	jumpAnimL.PushBack({ 0,1152,128,64 });
	jumpAnimL.PushBack({ 128,1152,128,64 });
	jumpAnimL.PushBack({ 0,1216,128,64 });
	jumpAnimL.PushBack({ 128,1216,128,64 });
	jumpAnimL.loop = true;
	jumpAnimL.speed = 0.069f;

	attackAnimR.PushBack({ 0,1280,128,64 });  //Pixel X, Pixel Y, tAMAÑO x, tAMAÑO y
	attackAnimR.PushBack({ 128,1280,128,64 });
	attackAnimR.PushBack({ 0,1344,128,64 });
	attackAnimR.PushBack({ 128,1344,128,64 });
	attackAnimR.PushBack({ 0,1408,128,64 });
	attackAnimR.PushBack({ 128,1408,128,64 });
	attackAnimR.loop = true;
	attackAnimR.speed = 0.069f;

	attackAnimR.PushBack({ 0,1472,128,64 });  //Pixel X, Pixel Y, tAMAÑO x, tAMAÑO y
	attackAnimR.PushBack({ 128,1472,128,64 });
	attackAnimR.PushBack({ 0,1536,128,64 });
	attackAnimR.PushBack({ 128,1536,128,64 });
	attackAnimR.PushBack({ 0,1600,128,64 });
	attackAnimR.PushBack({ 128,1600,128,64 });
	attackAnimR.PushBack({ 0,1664,128,64 });
	attackAnimR.PushBack({ 128,1664,128,64 });
	attackAnimR.loop = true;
	attackAnimR.speed = 0.069f;

	playerDeath.PushBack({ 0,1728,128,64 });  //Pixel X, Pixel Y, tAMAÑO x, tAMAÑO y
	playerDeath.PushBack({ 128,1728,128,64 });
	playerDeath.PushBack({ 0,1796,128,64 });
	playerDeath.PushBack({ 128,1796,128,64 });
	playerDeath.loop = false;
	playerDeath.speed = 0.069f;
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

	currentAnimation = &idleAnim;
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
	
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		vel = b2Vec2(-speed * dt, -GRAVITY_Y);
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		vel = b2Vec2(speed * dt, -GRAVITY_Y);
	}

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		//vel = b2Vec2(0, jumpForce);
		pbody->body->ApplyLinearImpulse(vel, pbody->body->GetWorldCenter(), true);
		LOG("JUMP");

	}

	if (isGodmode == true){
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
			vel = b2Vec2(0, -speed * dt);
		}
		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
			vel = b2Vec2(0, speed * dt);
		}
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			vel = b2Vec2(speed * dt, 0);
		}
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			vel = b2Vec2(-speed * dt, 0);
		}
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
			vel = b2Vec2(speed * dt, -speed * dt);
		}
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
			vel = b2Vec2(-speed * dt, -speed * dt);
		}
		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
			vel = b2Vec2(speed * dt, speed * dt);
		}
		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
			vel = b2Vec2(-speed * dt, speed * dt);
		}
	}

	//Set the velocity of the pbody of the player
	pbody->body->SetLinearVelocity(vel);
	
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

	if (app->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN) {
		position.x = parameters.attribute("x3").as_int();
		position.y = parameters.attribute("y3").as_int();
	}

	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) {
		if (isGodmode == false) { isGodmode = true; }
		else if (isGodmode == true) { isGodmode = false; }
	}

	//Update player position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	SDL_Rect rect = currentAnimation->GetCurrentFrame();

	app->render->DrawTexture(texture, position.x, position.y, &rect);

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