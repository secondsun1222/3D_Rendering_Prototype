#pragma once
enum PropertyType {
	EMPTY,
	MESH,
	TEXTURED_MESH,
	LIGHT,
};

class PropertyBase {
public:
	PropertyType type;

public:
	PropertyBase();
	PropertyBase(PropertyType);
};

typedef PropertyBase Property;