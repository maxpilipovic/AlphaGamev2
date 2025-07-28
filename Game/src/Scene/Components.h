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
	uint8_t alpha;
};

struct PlayerComponent
{

};

