#ifndef __PAGE_TEST_H__
#define __PAGE_TEST_H__

#include "Page.h"

class Entity;

class Page_Test : public Page
{
public:
	Page_Test();
	~Page_Test();
	
public:
   	void enter();
	bool test(const Event& event);

private:
	Entity* _bg;
	Entity* _man1;
	Entity* _man2;
};
#endif
