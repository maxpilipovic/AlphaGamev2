#pragma once

struct Transform {
	float x, y;
	float width, height;
	float rotation;
};

struct SpriteRendererComponent {
	SDL_FRect sprite;
	uint8_t red;
	uint8_t green;
	uint8_t blue;
	uint8_t alpha = 255;
};

struct Projectile {
	//velocity
	//direction
	//lifetime (live forever in registry)
	float velocity;
	float directionAngle;
	float lifetime;
	float age = 0.0f; //Time since spawned!
};

struct PathFollower {
	int currentWayPoint = 0;
	float speed = 100.0f;
};

struct Tank {
	float health = 5.0f;
};

struct OrangeTank {

};

struct RedTank {

};

struct TealTank {

};

struct VioletTank {

};

struct Robot {
	Astra::Entity target; //This will be used for the robots to lock onto a tank
	float fireRate = 1.0f; //Shots per second
	float fireCooldown = 0.0f; //Time until next shot
};

struct RedRobot
{

};

struct BlueRobot
{

};

struct GreenRobot
{

};

struct YellowRobot
{

};

struct CircleCollider
{
	float x;
	float y;
	float radius;
};

struct RobotRange
{
	float x;
	float y;
	float radius;
};



