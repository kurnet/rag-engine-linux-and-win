#ifndef __PAGE_LUATEST_H__
#define __PAGE_LUATEST_H__

#include "Page.h"
#include "Event.h"

extern "C"
{
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}

class Page_LuaTest : public Page
{
public:
	Page_LuaTest();
	~Page_LuaTest();
	
public:
   	void enter();
	void update(const double dt);

protected:
	bool resolved();
	void reload();
	bool eventHandler(const Event& event);

private:
	lua_State* luaState_;
};

#endif
