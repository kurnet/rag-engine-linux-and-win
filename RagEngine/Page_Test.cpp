#include "stdafx.h"
#include "Page_Test.h"

#include <functional>
#include <iostream>

#include "Entity.h"

Page_Test::Page_Test()
{
}

Page_Test::~Page_Test()
{	
}

void Page_Test::enter()
{
	// _man1 = new Entity();
	// _man1->loadFromFile("assets/no_rule_insect.png");
	// _man1->setClipRect(230, 63, 32, 30);
	// addChild(_man1);
	
	// _man2 = new Entity();
	// _man2->setPosition(100, 100);
	// _man2->loadFromFile("assets/no_rule_insect.png");
	// _man2->setClipRect(230, 63, 32, 30);
	// _man2->setColor(100, 100, 100);
	// addChild(_man2);

	// _bg = new Entity();
	// _bg->loadFromFile("assets/background.png");
	// addChild(_bg);

	// std::function<bool(const Event&)> handler = std::bind(&Page_Test::test, this, std::placeholders::_1);
	// _bg->addEventListener("haha_event", handler, this);
	// std::unique_ptr<Event> eptr(new Event("haha_event"));
	// _bg->dispatchEvent(*eptr.get());
	
	// auto haha = "test";
	// std::cout << "haha: " << haha << std::endl;
	
//	std::string eventType = "helloworld";
	// std::function<void(const Event*)> func1 = std::bind(&Page_Test::test, this, std::placeholders::_1);
	// std::function<void(const Event*)> func2 = std::bind(&Page_Test::test, this, std::placeholders::_1);

	// if (func1 == func2)
	// {
	// 	std::cout << "test1!!!" << std::endl;
	// }

//	std::cout << "test 1: " << (func1 == func2) << std::endl;
//	std::cout << "test 2: " << &func1 == &func2 << std::endl;
//	_bg->addEventListener(eventType, func, this);
}

bool Page_Test::test(const Event& event)
{
	std::cout << "haha_event handler test!" << std::endl;
	return true;
}