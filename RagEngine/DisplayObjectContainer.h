#ifndef __DISPLAYOBJECTCONTAINER_H__
#define __DISPLAYOBJECTCONTAINER_H__

//#include <ext/hash_map>
//#include <hash_map>
#include <unordered_map>
#include <vector>
#include "DisplayObject.h"

//struct hash_map;
//class Entity;

class DisplayObjectContainer : public DisplayObject
{
public:
	typedef std::unordered_map<int, DisplayObject*> DisplayObjectMap;
	typedef std::vector<DisplayObject*> DisplayObjectVec;
	
public:
	DisplayObjectContainer();
	virtual ~DisplayObjectContainer();

public:
	void dispose();
	void render();
	virtual void update(const double dt);
	void addChild(DisplayObject* entity);
	void removeChild(DisplayObject* entity);
	void releaseChild(DisplayObject* entity);

	virtual void setX(float x);
	virtual void setY(float y);
	int getIndex(const DisplayObject* entity) const;
	DisplayObjectVec& getChildren();
	
protected:
	DisplayObjectMap _entityMap;
	DisplayObjectVec _entityVec;
};

#endif
