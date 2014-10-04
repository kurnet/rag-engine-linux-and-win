#include "stdafx.h"
#include "GUI_Sprite.h"

#include <iostream>

#include "Helper.h"

GUI_Sprite::GUI_Sprite(const float x, const float y, const float w, const float h, SDL_Texture* texture)
{
	position_.x = x;
	position_.y = y;
	size_.x = w;
	size_.y = h;
	clipRect_.x = 0;
	clipRect_.y = 0;
	clipRect_.w = w;
	clipRect_.h = h;
	renderRect_.x = 0;
	renderRect_.y = 0;
	renderRect_.w = w;
	renderRect_.h = h;
	texture_ = texture;
	// texture_ = texture;
}

GUI_Sprite::~GUI_Sprite()
{
	
}

GUI_Sprite* GUI_Sprite::createSprite(const float x, const float y, const std::string& fname)
{
	float width = 0.0f;
	float height = 0.0f;
	SDL_Texture* texture = createTextureFromImage(fname, width, height);
	GUI_Sprite* sprite = new GUI_Sprite(x, y, width, height, texture);
	return sprite;
}

bool GUI_Sprite::resolved()
{
	return true;
}

// void GUI_Sprite::render()
// {
// 	DisplayObject::render();
// }
