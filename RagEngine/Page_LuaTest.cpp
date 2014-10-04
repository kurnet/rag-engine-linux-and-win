#include "stdafx.h"
#include "Page_LuaTest.h"

#include <iostream>
#include "Helper.h"
#include "GUI_Button.h"
#include "MouseEvent.h"

Page_LuaTest::Page_LuaTest()
{

}

Page_LuaTest::~Page_LuaTest()
{
	if (luaState_ != nullptr)
	{
		lua_close(luaState_);
	}
}
	
void Page_LuaTest::enter()
{
	Page::enter();
}

void Page_LuaTest::update(const double dt)
{
	Page::update(dt);
}

bool Page_LuaTest::resolved()
{
	Page::resolved();

	reload();

	GUI_Button* button1 = GUI_Button::createImageButton(200, 100, "assets/ui/button1.png");
	this->addChild(button1);
	button1->addEventListener(MouseEvent::MOUSE_DOWN, bindEventHandler(&Page_LuaTest::eventHandler, this), this);

	return true;
}

bool Page_LuaTest::eventHandler(const Event& event)
{
	// std::cout << "page lua test evenhandler" << std::endl;
	reload();
	return true;
}

void Page_LuaTest::reload()
{
	if (luaState_ != nullptr)
	{
		lua_close(luaState_);		
	}
	
	{
		std::string filename = "scripts/hello.lua";
		insertBaseDir(filename);
		std::cout << filename << std::endl;
		luaState_ = luaL_newstate();
	
		// load the libs
		luaL_openlibs(luaState_);
	
		// if ( luaL_loadfile(luaState_, filename.c_str()) ||
		// 	 lua_pcall(luaState_, 0, 0, 0)) {
		int result = luaL_dofile(luaState_, filename.c_str());
		if ( result != 0 ) {		
			std::cout<<"Error: script not loaded ("<<filename<<")"<<std::endl;
			// luaState_ = nullptr;
		}

		if (luaState_ != nullptr)
		{
			lua_Debug info;
			int level = 0;
			while (lua_getstack(luaState_, level, &info)) {
				lua_getinfo(luaState_, "nSl", &info);
				fprintf(stderr, "  [%d] %s:%d -- %s [%s]\n",
						level, info.short_src, info.currentline,
						(info.name ? info.name : "<unknown>"), info.what);
				++level;
			}
		}		
	}
}
