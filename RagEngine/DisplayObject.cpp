#include "stdafx.h"
#include "DisplayObject.h"

#include <iostream>
#include "Kernal.h"
#include "Helper.h"

// SDL_RendererFlip flip = SDL_FLIP_NONE);
DisplayObject::DisplayObject():
	parent_(nullptr),
	texture_(nullptr),
	flip_(SDL_FLIP_NONE),
	alpha_(1.0f),
	rotation_(0.0f),
	red_(255),
	green_(255),
	blue_(255)
{
	size_.x = 0.0;
	size_.y = 0.0;
	
	scale_.x = 1.0;
	scale_.y = 1.0;
	
	anchor_.x = 0.5;
	anchor_.y = 0.5;

	renderPosition_.x = 0;
	renderPosition_.y = 0;
	
	// clipRect_.x = 0;
	// clipRect_.y = 0;
	// clipRect_.w = 0;
	// clipRect_.h = 0;
}

DisplayObject:: ~DisplayObject()
{
}

void DisplayObject::free()
{
	if (texture_ != nullptr)
	{
		SDL_DestroyTexture(texture_);
		texture_ = nullptr;
		size_.x = 0;
		size_.y = 0;
	}
}

bool DisplayObject::loadFromFile(const std::string& path)
{
	free();

	SDL_Texture* newTexture = nullptr;
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == nullptr)
	{
		kernal.showSDLError();
		return nullptr;
	}
		
	newTexture = SDL_CreateTextureFromSurface(kernal.getRenderer(), loadedSurface);
	if (newTexture == nullptr)
	{
		kernal.showSDLError();
		return nullptr;
	}

	size_.x = loadedSurface->w;
	size_.y = loadedSurface->h;

	// std::cout << _width << ", " << _height << ", " << std::endl;
	
	SDL_FreeSurface(loadedSurface);

	texture_ = newTexture;
	return texture_ != nullptr;
}

void DisplayObject::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
	SDL_SetTextureColorMod(texture_, red, green, blue);
}

void DisplayObject::setBlendMode(SDL_BlendMode blending)
{
	SDL_SetTextureBlendMode(texture_, blending);
}

void DisplayObject::setAlpha(Uint8 alpha)
{
	SDL_SetTextureAlphaMod(texture_, alpha);
}

void DisplayObject::render()
{
	renderRect_ = {(int)renderPosition_.x,
				   (int)renderPosition_.y,
				   (int)(size_.x * scale_.x),
				   (int)(size_.y * scale_.y)};
	
	SDL_RenderCopyEx(kernal.getRenderer(), texture_, &clipRect_, &renderRect_, rotation_, &center_, flip_);
}

// void DisplayObject::setColor(const Uint8 red, const Uint8 green, const Uint8 blue)
// {
// 	// red_ = red;
// 	// green_ = green;
// 	// blue_ = blue;
// 	if (texture_ != nullptr)
// 	{
// 		texture_->setColor(red, green, blue);
// 	}
// }

// void DisplayObject::setBlendMode(const SDL_BlendMode blending)
// {
// 	if (texture_ != nullptr)
// 	{
// 		texture_->setBlendMode(blending);
// 	}
// 	//Set blending function
// //	SDL_SetTextureBlendMode(texture_, blending);	
// }

// void DisplayObject::setAlpha(const Uint8 alpha)
// {
// 	//Modulate texture alpha
// 	if (texture_ != nullptr)
// 	{
// 		texture_->setAlpha(alpha);
// 	}
// }

// void DisplayObject::setClipRect(const int x, const int y, const int width, const int height)
// {
// 	clipRect_.x = x;
// 	clipRect_.y = y;
// 	clipRect_.w = width;
// 	clipRect_.h = height;
// }

// void DisplayObject::renderTexture(Texture* texture)
// {
// 	// texture->render(position_.x, position_.y, &clipRect_);
// 	texture->render(position_.x, position_.y);
// }

// void DisplayObject::loadFromFile(const std::string &path)
// {
	// if (texture_ != nullptr)
	// {
	// 	delete texture_;
	// 	texture_ = nullptr;
	// }

//	texture_ = new Texture();
//	texture_->loadFromFile(path);
	// texture_ = std::unique_ptr<Texture>(new Texture());
	// texture_->loadFromFile(path);
	// setWidth(texture_->getWidth());
	// setHeight(texture_->getHeight());
	// setClipRect(0, 0, texture_->getWidth(), texture_->getHeight());

	// std::cout << "load from file, path: " << path.c_str() << ", size: " << texture_->getWidth() << ", " << texture_->getHeight() << std::endl;
// }

void DisplayObject::setPosition(const float x, const float y)
{
	position_.x = x;
	position_.y = y;
}

void DisplayObject::setX(const float x)
{
	position_.x = x;
	resetRenderX();
}

float DisplayObject::getX() const
{
	return position_.x;
}

void DisplayObject::setY(const float y)
{
	position_.y = y;
	resetRenderY();
}

float DisplayObject::getY() const
{
	return position_.y;
}

void DisplayObject::setWidth(const float width)
{
	size_.x = width;
	// if (texture_ != nullptr)
	// {
	// 	texture_->setWidth(width);
	// }
}

float DisplayObject::getWidth() const
{
	return size_.x;
}

void DisplayObject::setHeight(const float height)
{
	size_.y = height;

	// if (texture_ != nullptr)
	// {
	// 	texture_->setHeight(height);
	// }
}

float DisplayObject::getHeight() const
{
	return size_.y;
}

// void DisplayObject::setReleased(const bool released)
// {
// 	needReleased_ = released;
// }

// bool DisplayObject::needReleased()
// {
// 	return needReleased_;
// }

void DisplayObject::setParent(DisplayObjectContainer* parent)
{
	parent_ = parent;

	// setX(parent_->getX() + getX() - parent_->getWidth() * parent_->getAnchorX());
	// setY(parent_->getY() + getY());

	// reset position 
	resetRenderX(); resetRenderY();

	// std::cout << "parent x: " << parent->getX() << std::endl; 
	// std::cout << "setx: " << getX() << std::endl; 
}

bool DisplayObject::hitTest(const int x, const int y)
{
	return hitTestPointVSRect(x, y, position_.x - (size_.x * scale_.x) * anchor_.x, position_.y - (size_.y * scale_.y) * anchor_.y, size_.x * scale_.x, size_.y * scale_.y);
}

void DisplayObject::setScaleX(const float x)
{
	scale_.x = x;
}

float DisplayObject::getScaleX() const
{
	return scale_.x;
}

void DisplayObject::setScaleY(const float y)
{
	scale_.y = y;
}

float DisplayObject::getScaleY() const
{
	return scale_.y;
}

void DisplayObject::setAnchorX(const float x)
{
	anchor_.x = x;
}

float DisplayObject::getAnchorX() const
{
	return anchor_.x;
}

void DisplayObject::setAnchorY(const float y)
{
	anchor_.y = y;
}

float DisplayObject::getAnchorY() const
{
	return anchor_.y;
}

void DisplayObject::setRotation(const float rotation)
{
	rotation_ = rotation;
}

float DisplayObject::getRotation() const
{
	return rotation_;
}

void DisplayObject::resetRenderX()
{
	float modX = 0;
	if (parent_ != nullptr)
	{
		modX = parent_->getX();
	}
	renderPosition_.x = position_.x - (scale_.x * size_.x) * anchor_.x + modX;
}

void DisplayObject::resetRenderY()
{
	float modY = 0;
	if (parent_ != nullptr)
	{
		modY = parent_->getY();
	}
	renderPosition_.y = position_.y - (scale_.y * size_.y) * anchor_.y + modY;
}
