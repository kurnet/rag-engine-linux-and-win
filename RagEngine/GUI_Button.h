#ifndef __GUI_BUTTON_H__
#define __GUI_BUTTON_H__

#include <memory>
#include "SDL.h"
#include "SDL_image.h"

#include "claw/tween/single_tweener.hpp"
#include "claw/tween/tweener_group.hpp"
#include "claw/tween/tweener_sequence.hpp"
#include "claw/tween/easing/easing_linear.hpp"
#include "boost/bind.hpp"

#include "DisplayObject.h"
#include "Event.h"

class MouseEvent;

enum ButtonState
{
	UP,
	DOWN,
	HOVER,
	COUNT,
};

class GUI_Button : public DisplayObject
{
public:
	static GUI_Button* createSimpleButton(const int x, const int y, const int w, const int h, const char* title);
	static GUI_Button* createImageButton(const int x, const int y, const std::string& fname);
	// static GUI_Button* createPNDButton(const int x, const int y)
	
public:
	GUI_Button(const int x, const int y, const int w, const int h, SDL_Texture* texture);
	~GUI_Button();
	
public:
	bool resolved();
	
private:
	void createEventListener();
	bool eventHandler(const Event& event);
	void update(const double dt);
	void freeTexture();
	
private:
	bool clicked_;
	bool hovered_;
	ButtonState state_;
	// SDL_Texture* textures_[ButtonState::COUNT];
	claw::tween::tweener_sequence tweenSeq_;
	// claw::tween::tweener_group tweenGroup_;
	// claw::tween::single_tweener tween;
};

#endif

