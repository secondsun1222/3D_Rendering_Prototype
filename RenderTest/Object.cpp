#include "Object.h"
#include "InputManager.h"

Property* Object::findProperty(PropertyType _type)
{
	for (auto it : properties) {
		if (it->type == _type)return it;
	}
	return nullptr;
}

bool Object::addProperty(Property* _property)
{
	if (std::find(properties.begin(), properties.end(), _property) != properties.end()) return false;	//already in properties

	properties.push_back(_property);

	return true;
}

bool Object::removeProperty(Property* _property)
{
	if (std::find(properties.begin(), properties.end(), _property) == properties.end()) return false;	//not in properties

	auto removeSt = std::remove(properties.begin(), properties.end(), _property);
	properties.erase(removeSt, properties.end());

	delete _property;
	return true;
}

void Object::applyOrigin()
{
	switch (origin) {
	case Origin::WORLD:
		originPosition = position;
		originRotation = rotation;
		break;
	case Origin::PARENT:
		originPosition = parent->originPosition + Quaternion::rotate(parent->originRotation, position);
		originRotation = Quaternion::multiply(parent->originRotation, rotation);
		break;
	}
}

bool Object::setParent(Object* _parent)
{
	if (parent == _parent)return false;		//already parent

	if (parent != nullptr) {	//remove from parent if it has a parent
		auto removeSt = std::remove(children.begin(), children.end(), this);
		children.erase(removeSt, children.end());
	}

	parent = _parent;

	parent->addChild(this);
	return true;
}

bool Object::addChild(Object* _child)
{
	if (std::find(children.begin(), children.end(), _child) != children.end()) return false;	//already in children
	
	children.push_back(_child);

	if (_child->parent != this)
		_child->setParent(this);
	return true;
}

Object::Object()
	:position()
	, rotation()
	, originPosition()
	, originRotation()
	, origin(Origin::WORLD)
	, parent(nullptr)
{
	init();
}

bool Object::init()
{
	return true;
}

bool Object::update()
{
	return true;
}
