#include "stdafx.h"
#include "GUI_Button.h"
#include <iostream>
#include <functional>
#include <memory>
#include "SDL.h"

#include "Kernal.h"
#include "MouseEvent.h"
#include "Helper.h"
#include "Stage.h"

static const float TWEEN_HOVER_DURATION = 0.07f;
static const float TWEEN_HIT_DURATION = 0.07f;
static const float TWEEN_HOVER_SCALE = 1.1f;
static const float TWEEN_HIT_SCALE = 1.3f;

GUI_Button::GUI_Button(const int x, const int y, const int w, const int h, SDL_Texture* texture):
	clicked_(false),
	hovered_(false),
	state_(ButtonState::UP)
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
}

GUI_Button::~GUI_Button()
{
	freeTexture();
}

bool GUI_Button::resolved()
{
	createEventListener();

	return true;
}

void GUI_Button::freeTexture()
{
	if (texture_ != nullptr)
	{
		SDL_DestroyTexture(texture_);		
	}
}

GUI_Button* GUI_Button::createSimpleButton(const int x, const int y, const int w, const int h, const char* title)
{
	// SDL_Texture* textures[ButtonState::COUNT];
	SDL_Texture* texture;
	// create UP texture
	{
		SDL_Surface* surface = SDL_CreateRGBSurface(0, w, h, 24, 0, 0, 0, 0);
		SDL_Rect rect;
		int result;
		rect = {0, 0, w, h};
		result = SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 0, 0, 0));
		if (result == 0)
		{
			kernal.showSDLError();
		}
		rect = {1, 1, w-2, h-2};
		result = SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 150, 150, 150));

		// std::cout << "result: " << result << std::endl;
		texture = SDL_CreateTextureFromSurface(kernal.getRenderer(), surface);

		if (texture == nullptr)
		{
			kernal.showSDLError();
		}

		// textures[ButtonState::UP] = texture;
		// textures[ButtonState::DOWN] = texture;
		// textures[ButtonState::HOVER] = texture;

		SDL_FreeSurface(surface);
	}

	GUI_Button* button = new GUI_Button(x, y, w, h, texture);

	return button;
}

GUI_Button* GUI_Button::createImageButton(const int x, const int y, const std::string& fname)
{
	float width = 0.0f;
	float height = 0.0f;
	SDL_Texture* texture = createTextureFromImage(fname, width, height);

	GUI_Button* button = new GUI_Button(x, y, width, height, texture);

	return button;
}

void GUI_Button::createEventListener()
{
	kernal.getStage().addEventListener(MouseEvent::MOUSE_MOVE, bindEventHandler(&GUI_Button::eventHandler, this), this);
	kernal.getStage().addEventListener(MouseEvent::MOUSE_DOWN, bindEventHandler(&GUI_Button::eventHandler, this), this);
	kernal.getStage().addEventListener(MouseEvent::MOUSE_UP, bindEventHandler(&GUI_Button::eventHandler, this), this);
}

bool GUI_Button::eventHandler(const Event& event)
{
	MouseEvent* mouseEvent = (MouseEvent*)(&event);
	int mouseX = mouseEvent->getMouseX();
	int mouseY = mouseEvent->getMouseY();
	
	// std::cout << mouseEvent->getMouseX() << std::endl;
	// std::cout << "gui button event handler: " << event.getType() << std::endl;
	if (event.getType().compare(MouseEvent::MOUSE_MOVE) == 0)
	{
		// std::cout << "mouse move " << mouseEvent->getMouseX() << ", " << mouseEvent->getMouseY() << std::endl;

		if (hitTest(mouseX, mouseY))
		{
			if (state_ != ButtonState::DOWN)
			{
				if (clicked_)
				{
					state_ = ButtonState::DOWN;
				}
				else
				{
					if (!hovered_)
					{
						hovered_ = true;

						// clicked handler
						claw::tween::tweener_group group;
						group.insert(
							claw::tween::single_tweener(1.0f, TWEEN_HOVER_SCALE, TWEEN_HOVER_DURATION, boost::bind(&DisplayObject::setScaleX, this, _1), claw::tween::easing_linear::ease_in )
							);
						group.insert(
							claw::tween::single_tweener(1.0f, TWEEN_HOVER_SCALE, TWEEN_HOVER_DURATION, boost::bind(&DisplayObject::setScaleY, this, _1), claw::tween::easing_linear::ease_in )
							);

						tweenSeq_.insert(group);

						this->dispatchEvent(Event(MouseEvent::MOUSE_HOVER));
					}
					state_ = ButtonState::HOVER;
				}
			}
		}
		else
		{
			state_ = ButtonState::UP;

			if (hovered_)
			{
				claw::tween::tweener_group group;
				group.insert(
					claw::tween::single_tweener(TWEEN_HOVER_SCALE, 1.0f, TWEEN_HOVER_DURATION, boost::bind(&DisplayObject::setScaleX, this, _1), claw::tween::easing_linear::ease_in )
					);
				group.insert(
					claw::tween::single_tweener(TWEEN_HOVER_SCALE, 1.0f, TWEEN_HOVER_DURATION, boost::bind(&DisplayObject::setScaleY, this, _1), claw::tween::easing_linear::ease_in )
					);

				tweenSeq_.insert(group);

				this->dispatchEvent(Event(MouseEvent::MOUSE_LOSE_HOVER ));
			}
			hovered_ = false;
		}
		// texture_ = textures_[state_];
	}
	else if (event.getType().compare(MouseEvent::MOUSE_DOWN) == 0)
	{
		// std::cout << "mouse down " << mouseEvent->getMouseX() << ", " << mouseEvent->getMouseY() << std::endl;

		if (hitTest(mouseX, mouseY))
		{
			state_ = ButtonState::DOWN;
			// texture_ = textures_[state_];

			if (!clicked_)
			{
				// std::cout << "click tween" << std::endl;
				claw::tween::tweener_group group1;
				claw::tween::tweener_group group2;
				group1.insert(
					claw::tween::single_tweener(TWEEN_HOVER_SCALE, TWEEN_HIT_SCALE, TWEEN_HIT_DURATION, boost::bind(&DisplayObject::setScaleX, this, _1), claw::tween::easing_linear::ease_in )
					);
				group1.insert(
					claw::tween::single_tweener(TWEEN_HOVER_SCALE, TWEEN_HIT_SCALE, TWEEN_HIT_DURATION, boost::bind(&DisplayObject::setScaleY, this, _1), claw::tween::easing_linear::ease_in )
					);
				group2.insert(
					claw::tween::single_tweener(TWEEN_HIT_SCALE, TWEEN_HOVER_SCALE, TWEEN_HIT_DURATION, boost::bind(&DisplayObject::setScaleX, this, _1), claw::tween::easing_linear::ease_in )
					);
				group2.insert(
					claw::tween::single_tweener(TWEEN_HIT_SCALE, TWEEN_HOVER_SCALE, TWEEN_HIT_DURATION, boost::bind(&DisplayObject::setScaleY, this, _1), claw::tween::easing_linear::ease_in )
					);

				tweenSeq_.insert(group1);
				tweenSeq_.insert(group2);

				this->dispatchEvent(Event(MouseEvent::MOUSE_DOWN));
			}
			clicked_ = true;
		}

	}
	else if (event.getType().compare(MouseEvent::MOUSE_UP) == 0)
	{
		// std::cout << "mouse hover " << mouseEvent->getMouseX() << ", " << mouseEvent->getMouseY() << std::endl;

		if (hitTest(mouseX, mouseY))
		{
			state_ = ButtonState::HOVER;
			this->dispatchEvent(Event(MouseEvent::MOUSE_UP));
			// handler
		}
		else
		{
			state_ = ButtonState::UP;
		}
		clicked_ = false;
		// texture_ = textures_[state_];
	}
	// SDL_MOUSEBUTTONDOWN
	// SDL_MOUSEBUTTONUP

	return true;
}

void GUI_Button::update(const double dt)
{
	DisplayObject::update(dt);

	// rotation_ += 1.5f;
	// if (!tweenGroup_.is_finished())
	// {
	// 	tweenGroup_.update(dt);
	// }

	// std::cout << "scale: " << scale_.x << ", " << scale_.y << std::endl;
	if (!tweenSeq_.is_finished())
	{
		tweenSeq_.update(dt);
	}
}
