#include "Page_UITest.h"
#include "stdafx.h"

#include <string>
#include <iostream>
#include <armadillo>

#include "GUI_Button.h"
#include "GUI_Sprite.h"

#include "Kernal.h"
#include "DisplayObjectContainer.h"
#include "MouseEvent.h"

DisplayObjectContainer* container = nullptr;

// using namespace arma;

Page_UITest::Page_UITest()
{
}

Page_UITest::~Page_UITest()
{
}

bool Page_UITest::resolved()
{
	GUI_Button* button1 = GUI_Button::createImageButton(240, 160, "assets/ui/button1.png");
	this->addChild(button1);

	container = new DisplayObjectContainer();
	container->setX(kernal.getScreenWidth() / 2.0f);
	container->setY(kernal.getScreenHeight() / 2.0f);
	this->addChild(container);

	int posX = -206;
	int posY = -4;
	int offsetX = 82;
	int count = 0;
	
	{
		GUI_Sprite* sprite = GUI_Sprite::createSprite(0.0f, 0.0f, "assets/ui/container.png");
		container->addChild(sprite);
	}
	
	{
		GUI_Sprite* sprite = GUI_Sprite::createSprite(0, 0, "assets/ui/icon1.png");
		container->addChild(sprite);
		sprite->setX(posX + count++ * offsetX);
		sprite->setY(posY);
	}

	{
		GUI_Sprite* sprite = GUI_Sprite::createSprite(0, 0, "assets/ui/icon2.png");
		container->addChild(sprite);
		sprite->setX(posX + count++ * offsetX);
		sprite->setY(posY);
	}
	

	{
		GUI_Sprite* sprite = GUI_Sprite::createSprite(0, 0, "assets/ui/icon3.png");
		container->addChild(sprite);
		sprite->setX(posX + count++ * offsetX);
		sprite->setY(posY);
	}
	

	{
		GUI_Sprite* sprite = GUI_Sprite::createSprite(0, 0, "assets/ui/icon4.png");
		container->addChild(sprite);
		sprite->setX(posX + count++ * offsetX);
		sprite->setY(posY);
	}

	{
		GUI_Sprite* sprite = GUI_Sprite::createSprite(0, 0, "assets/ui/icon5.png");
		container->addChild(sprite);
		sprite->setX(posX + count++ * offsetX);
		sprite->setY(posY);
	}

	{
		GUI_Sprite* sprite = GUI_Sprite::createSprite(0, 0, "assets/ui/icon6.png");
		container->addChild(sprite);
		sprite->setX(posX + count++ * offsetX);
		sprite->setY(posY);
	}

	// container->setX(0);
	// container->setY(0);
	
	arma::mat A(3, 4, arma::fill::zeros);
	arma::mat B(3, 4, arma::fill::zeros);
	// A(0, 0) = 2.0f;
	// B(0, 0) = 3.0f;
	std::cout << A << std::endl;
	// std::cout << A*B.t() << std::endl;

	// A = 1 2    B = 7 8 9 
	//     3 4        1 2 3
    //     5 6             
		
	// arma::mat B = randu<mat>(4, 5);
	// this->addChild(sprite1);

	// create a simple image button
	// GUI_Button* button2 = GUI_Button.createSimpleImageButton(0, 0, "asset/button1.png");
	// this->addChild(button2);

	// std::cout << "Page ui test created" << std::endl;
	// GUI_Button* button = new GUI_Button();
	// button->createSimpleButton(64, 32, "hello world");
	// button->setX(240);
	// button->setY(160);
	// this->addChild(button);

	return true;
}

// void Page_UITest::update(const double dt)
// {
// 	Object::update(dt);
// }
		
void Page_UITest::enter()
{
}

float rotation = 0.0f;
void Page_UITest::update(const double dt)
{
	Page::update(dt);

	container->setRotation(rotation+=0.1f);
	// std::cout << container->getRotation() << std::endl;
}

bool Page_UITest::button1Handler(MouseEvent* event)
{
	return true;
}

bool Page_UITest::button2Handler(MouseEvent* event)
{
	return true;
}
	
