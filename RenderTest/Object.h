#pragma once
#include "Vectors.h"
#include "PropertyBase.h"
#include "TimeManager.h"

enum Origin {
	WORLD,
	PARENT,
};

class Object {
//property list
private:
	std::vector<Property*> properties;
public:
	Property* findProperty(PropertyType);
	bool addProperty(Property*);
	bool removeProperty(Property*);

public:
	Vec3 position;
	Quaternion rotation;

public:
	Vec3 originPosition;		// absolute position, origin applied
	Quaternion originRotation;	// absolute rotation, origin applied

public:
	Origin origin;
	void applyOrigin();

//object tree
private:
	Object* parent;
	std::vector<Object*> children;

public:
	bool setParent(Object*);
	bool addChild(Object*);
	std::vector<Object*>* getChildren() { return &children; };

public:
	Object();

public:
	virtual bool init();
	virtual bool update();
};