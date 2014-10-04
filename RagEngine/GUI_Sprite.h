#ifndef __GUI_SPRITE_H__
#define __GUI_SPRITE_H__

#include <string>
#include "SDL.h"

#include "DisplayObject.h"

class GUI_Sprite : public DisplayObject
{
public:
	static GUI_Sprite* createSprite(const float x, const float y, const std::string& fname);
	
public:
	GUI_Sprite(const float x, const float y, const float w, const float h, SDL_Texture* texture);
	~GUI_Sprite();

public:
	bool resolved();
	// void render();
};

#endif
