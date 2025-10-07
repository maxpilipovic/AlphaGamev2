#pragma once

#include <string>
#include <vector>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <Astra/Entity/Entity.hpp>

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
	float damage = 2.0f;
};

struct PathCollision
{
	std::vector<SDL_FPoint> leftBorder;
	std::vector<SDL_FPoint> rightBorder;
	std::vector<SDL_FPoint> polygon;

	void Serialize(auto& ar)
	{
		ar(leftBorder)(rightBorder)(polygon);
	}
};

struct PathNode {
	SDL_FPoint pos;
};

struct PathFollower {
	int currentWayPoint = 0;
	float speed = 100.0f;
};

struct Tank {
	float health = 6.0f;
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

//UI STUFF

//Position and size of component
struct UITransformComponent {
	SDL_FPoint Position = { 0.0f, 0.0f };
	SDL_FPoint Size = { 100.0f, 50.0f };
};

//Visual info??? Image?
struct UIImageComponent
{
	SDL_Texture* Texture = nullptr;
	SDL_Rect SrcRect = { 0, 0, 0, 0 };
	SDL_Color Color = { 255, 255, 255, 255 };
	bool Visible = true;
};

//Actual text... Data?
struct UITextComponent
{
	std::string Text;
	TTF_Font* Font = nullptr;
	SDL_Color Color = { 255, 255, 255, 255 };
	int FontSize = 24;
	bool Visible = true;

	void Serialize(auto& ar)
	{
		ar(Text);
	}
};

//enum class or enum?
enum class ButtonState
{
	Normal,
	Hovered,
	Pressed
};

enum class UIButtonAction
{
	None,
	CreateRobotRed,
	CreateRobotBlue,
	CreateRobotGreen,
	CreateRobotYellow
};

//Make button interactive. Different colors?
struct UIButtonComponent
{
	ButtonState State = ButtonState::Normal;
	bool Visible = true;
	UIButtonAction Action = UIButtonAction::None;

	// std::function<void()> OnClick;
	// std::function<void()> OnHover;
};

