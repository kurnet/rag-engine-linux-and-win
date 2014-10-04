#ifndef __PAGE_UITEST_H__
#define __PAGE_UITEST_H__

#include "Page.h"

class MouseEvent;

class Page_UITest : public Page
{
public:
	Page_UITest();
	~Page_UITest();
	
public:
   	void enter();
	void update(const double dt);
	
protected:
	bool resolved();
	bool button1Handler(MouseEvent* event);
	bool button2Handler(MouseEvent* event);
	
private:
};

#endif
