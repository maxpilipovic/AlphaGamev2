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

struct Robot {

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



