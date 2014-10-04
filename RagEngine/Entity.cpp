#include "stdafx.h"
#include "Entity.h"

#include <iostream>
#include "DisplayObjectContainer.h"

Entity::Entity()
{

}

Entity::~Entity()
{
}

void Entity::setRelease(const bool release)
{
	needReleased_ = release;
}
