#ifndef __HELPER_H__
#define __HELPER_H__

#include <stdio.h>
//#include <unistd.h>
#include <sstream>
#include <functional>
#include "Event.h"
#include "Kernal.h"

static inline void insertBaseDir(std::string& str)
{
	std::stringstream path;
	// std::cout << kernal.getBaseDir() << std::endl;
	path << kernal.getBaseDir() << "/" << str.c_str();
	str = path.str();
}

template <class T>
std::function<bool(const Event&)> bindEventHandler(bool (T::*handler)(const Event&), T* invoker)
{
	return std::bind(handler, invoker, std::placeholders::_1);
}

static inline bool hitTestPointVSRect(const int x1, const int y1, const int x2, const int y2, const int width, const int height)
{
	if (x1 < x2)
	{
		return false;
	}

	if (y1 < y2)
	{
		return false;
	}

	if (x1 > (x2 + width))
	{
		return false;
	}

	if (y1 > (y2 + height))
	{
		return false;
	}
	
	return true;
}

static inline SDL_Texture* createTextureFromImage(const std::string& path, float& width, float& height)
{
	std::string filepath = path.c_str();
	insertBaseDir(filepath);
	SDL_Texture* texture = nullptr;
	SDL_Surface* surface = IMG_Load(filepath.c_str());
	if (surface == nullptr)
	{
		kernal.showSDLError();
		return nullptr;
	}
		
	texture = SDL_CreateTextureFromSurface(kernal.getRenderer(), surface);
	if (texture == nullptr)
	{
		kernal.showSDLError();
		return nullptr;
	}

// std::cout << _width << ", " << _height << ", " << std::endl;
	
	SDL_FreeSurface(surface);

	width = surface->w;
	height = surface->h;
	
	return texture;
}

#endif
