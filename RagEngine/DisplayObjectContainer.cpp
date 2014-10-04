#include "stdafx.h"
#include "DisplayObjectContainer.h"

#include <iostream>
#include <assert.h>
#include <sstream>
#include <algorithm>

// #include "Texture.h"

DisplayObjectContainer::DisplayObjectContainer()
{
}

DisplayObjectContainer::~DisplayObjectContainer()
{
	dispose();
}

void DisplayObjectContainer::dispose()
{
	DisplayObjectVec::iterator iter = _entityVec.begin();
	// std::cout << "size: " << _entityVec.size() << std::endl;
	while (iter != _entityVec.end())
	{
		delete (*iter);
		++iter;
	}

	_entityVec.clear();
}

void DisplayObjectContainer::update(const double dt)
{
	DisplayObject::update(dt);
	
	// std::cout << "display object container update" << std::endl;
	DisplayObjectVec::iterator iter = _entityVec.begin();

	// std::cout << "size: " << _entityVec.size() << std::endl;
	while (iter != _entityVec.end())
	{
		// check if needed to release
		if((*iter)->isReleased())
		{
			releaseChild(*iter);
			--iter;
			continue;
		}
		// std::cout << "update" << std::endl;
		(*iter)->update(dt);
		++iter;
	}

}

void DisplayObjectContainer::render()
{
	DisplayObjectVec::iterator iter = _entityVec.begin();
	while (iter != _entityVec.end())
	{
		(*iter)->render();
		++iter;
	}
}

void DisplayObjectContainer::addChild(DisplayObject* entity)
{
	// std::cout << "add child" << std::endl;
	// check if it existed already

	if (_entityMap[entity->getId()] != nullptr)
	{
		std::stringstream msg;
		msg << "entity " << entity->getId() << " is already added!";
		assert(false && msg);
		return;
	}

	entity->setParent(this);
	_entityMap[entity->getId()] = entity;
	_entityVec.push_back(entity);
}

void DisplayObjectContainer::removeChild(DisplayObject* entity)
{
	// check if it existed already
	if (_entityMap[entity->getId()] == nullptr)
	{
		std::stringstream msg;
		msg << "entity " << entity->getId() << " is not added!";
		assert(false && msg);
		return;
	}
	
	_entityMap[entity->getId()] = nullptr;
	std::vector<int> v;
	DisplayObjectVec::iterator iter = std::remove(_entityVec.begin(), _entityVec.end(), entity);
	_entityVec.erase(iter);
}

void DisplayObjectContainer::releaseChild(DisplayObject* entity)
{
	if (entity != nullptr)
	{
		removeChild(entity);
		delete entity;
		entity = nullptr;		
	}
}

void DisplayObjectContainer::setX(float x)
{
	position_.x = x;
	
	DisplayObjectVec::iterator iter = _entityVec.begin();
	while (iter != _entityVec.end())
	{
		(*iter)->resetRenderX();
		++iter;
	}
}

void DisplayObjectContainer::setY(float y)
{
	position_.y = y;
		
	DisplayObjectVec::iterator iter = _entityVec.begin();
	while (iter != _entityVec.end())
	{
		(*iter)->resetRenderY();
		++iter;
	}
}

int DisplayObjectContainer::getIndex(const DisplayObject* entity) const
{
	return find(_entityVec.begin(), _entityVec.end(), entity) - _entityVec.begin();
}

DisplayObjectContainer::DisplayObjectVec& DisplayObjectContainer::getChildren()
{
	return _entityVec;
}
