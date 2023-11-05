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

	idleAnim.PushBack({ 0,0,128,64 });  //Pixel X, Pixel Y, tAMA�O x, tAMA�O y
	idleAnim.PushBack({ 128,0,128,64 });
	idleAnim.PushBack({ 0,64,128,64 });
	idleAnim.PushBack({ 128,64,128,64 });
	idleAnim.PushBack({ 0,128,128,64 });
	idleAnim.PushBack({ 128,128,128,64 });
	idleAnim.PushBack({ 0,192,128,64 });
	idleAnim.PushBack({ 128,192,128,64 });
	idleAnim.loop = true;
	idleAnim.speed = 0.069f;

	runAnimR.PushBack({ 0,256,128,64 });  //Pixel X, Pixel Y, tAMA�O x, tAMA�O y
	runAnimR.PushBack({ 128,256,128,64 });
	runAnimR.PushBack({ 0,320,128,64 });
	runAnimR.PushBack({ 128,320,128,64 });
	runAnimR.PushBack({ 0,384,128,64 });
	runAnimR.PushBack({ 128,384,128,64 });
	runAnimR.PushBack({ 0,448,128,64 });
	runAnimR.PushBack({ 128,448,128,64 });
	runAnimR.loop = true;
	runAnimR.speed = 0.069f;

	runAnimL.PushBack({ 0,512,128,64 });  //Pixel X, Pixel Y, tAMA�O x, tAMA�O y
	runAnimL.PushBack({ 128,512,128,64 });
	runAnimL.PushBack({ 0,576,256,64 });
	runAnimL.PushBack({ 128,576,128,64 });
	runAnimL.PushBack({ 0,640,128,64 });
	runAnimL.PushBack({ 128,640,128,64 });
	runAnimL.PushBack({ 0,704,128,64 });
	runAnimL.PushBack({ 128,704,128,64 });
	runAnimL.loop = true;
	runAnimL.speed = 0.069f;

	jumpAnimR.PushBack({ 0,768,128,64 });  //Pixel X, Pixel Y, tAMA�O x, tAMA�O y
	jumpAnimR.PushBack({ 128,768,128,64 });
	jumpAnimR.PushBack({ 0,832,128,64 });
	jumpAnimR.PushBack({ 128,832,128,64 });
	jumpAnimR.PushBack({ 0,896,128,64 });
	jumpAnimR.PushBack({ 128,896,128,64 });
	jumpAnimR.PushBack({ 0,950,128,64 });
	jumpAnimR.PushBack({ 128,950,128,64 });
	jumpAnimR.loop = true;
	jumpAnimR.speed = 0.069f;

	jumpAnimL.PushBack({ 0,1024,128,64 });  //Pixel X, Pixel Y, tAMA�O x, tAMA�O y
	jumpAnimL.PushBack({ 128,1024,128,64 });
	jumpAnimL.PushBack({ 0,1088,128,64 });
	jumpAnimL.PushBack({ 128,1088,128,64 });
	jumpAnimL.PushBack({ 0,1152,128,64 });
	jumpAnimL.PushBack({ 128,1152,128,64 });
	jumpAnimL.PushBack({ 0,1216,128,64 });
	jumpAnimL.PushBack({ 128,1216,128,64 });
	jumpAnimL.loop = true;
	jumpAnimL.speed = 0.069f;

	attackAnimR.PushBack({ 0,1280,128,64 });  //Pixel X, Pixel Y, tAMA�O x, tAMA�O y
	attackAnimR.PushBack({ 128,1280,128,64 });
	attackAnimR.PushBack({ 0,1344,128,64 });
	attackAnimR.PushBack({ 128,1344,128,64 });
	attackAnimR.PushBack({ 0,1408,128,64 });
	attackAnimR.PushBack({ 128,1408,128,64 });
	attackAnimR.loop = true;
	attackAnimR.speed = 0.069f;

	attackAnimR.PushBack({ 0,1472,128,64 });  //Pixel X, Pixel Y, tAMA�O x, tAMA�O y
	attackAnimR.PushBack({ 128,1472,128,64 });
	attackAnimR.PushBack({ 0,1536,128,64 });
	attackAnimR.PushBack({ 128,1536,128,64 });
	attackAnimR.PushBack({ 0,1600,128,64 });
	attackAnimR.PushBack({ 128,1600,128,64 });
	attackAnimR.PushBack({ 0,1664,128,64 });
	attackAnimR.PushBack({ 128,1664,128,64 });
	attackAnimR.loop = true;
	attackAnimR.speed = 0.069f;

	playerDeath.PushBack({ 0,1728,128,64 });  //Pixel X, Pixel Y, tAMA�O x, tAMA�O y
	playerDeath.PushBack({ 128,1728,128,64 });
	playerDeath.PushBack({ 0,1796,128,64 });
	playerDeath.PushBack({ 128,1796,128,64 });
	playerDeath.loop = false;
	playerDeath.speed = 0.069f;
}

Player::~Player() {

}

bool Player::Awake() {

	position = iPoint(config.attribute("x").as_int(), config.attribute("y").as_int());
	initialPosition = iPoint(config.attribute("x").as_int(), config.attribute("y").as_int());

	return true;
}

bool Player::Start() {

	b2Vec2 velocity;
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
		velocity.x += gravity.x * dt;
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

	if (app->render->camera.x - position.x + 400 <= -24 && app->render->camera.x - position.x + 400 >= -1550) {
		app->render->camera.x = -position.x + 400;

	}

	return true;
}

bool Player::CleanUp()
{

	return true;
}

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