#ifndef __DISPLAYOBJECT_H__
#define __DISPLAYOBJECT_H__

#include <memory>
#include "SDL.h"
#include "SDL_image.h"

#include "EventDispatcher.h"
#include "Vector2D.h"

class DisplayObjectContainer;
class Texture;

class DisplayObject : public EventDispatcher
{
public:
	DisplayObject();
	virtual ~DisplayObject();

public:
	void free();
	bool loadFromFile(const std::string& path);
	virtual void render();
	void setColor(Uint8 red, Uint8 green, Uint8 blue);
	void setBlendMode(SDL_BlendMode blending);
	void setAlpha(Uint8 alpha);
	// void setClipRect(const int x, const int y, const int width, const int height);

	void setParent(DisplayObjectContainer* parent);
	bool hitTest(const int x, const int y);

public:
	// getter and setter
	void setPosition(const float x, const float y);
	
	virtual void setX(const float x);
	float getX() const;
	
	virtual void setY(const float y);
	float getY() const;

	void setWidth(const float width);
	float getWidth() const;

	void setHeight(const float height);
	float getHeight() const;

	void setScaleX(const float x);
	float getScaleX() const;

	void setScaleY(const float y);
	float getScaleY() const;
	
	void setAnchorX(const float x);
	float getAnchorX() const;

	void setAnchorY(const float y);
	float getAnchorY() const;

	void setRotation(const float rotation);
	float getRotation() const;
	// void setReleased(const bool released);
	// bool needReleased();

	void resetRenderX();
	void resetRenderY();

protected:
	DisplayObjectContainer* parent_;
	SDL_Texture* texture_;
	SDL_Point center_;
	SDL_Rect clipRect_;
	SDL_Rect renderRect_;
	SDL_RendererFlip flip_;
	Vector2D position_;
	Vector2D renderPosition_;
	Vector2D anchor_;
	Vector2D scale_;
	Vector2D size_;	
	float alpha_;
	float rotation_;
	Uint8 red_;
	Uint8 green_;
	Uint8 blue_;
};

#endif
