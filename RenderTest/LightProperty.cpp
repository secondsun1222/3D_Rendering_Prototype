#include "LightProperty.h"

LightProperty::LightProperty(LightType _type, double _brightness)
	:Property(PropertyType::LIGHT)
	, type(_type)
	, brightness(_brightness)
	, color(RGB(255,255,255))
{}

LightProperty::LightProperty(LightType _type, double _brightness, COLORREF _color)
	:Property(PropertyType::LIGHT)
	, type(_type) 
	, brightness(_brightness)
	, color(_color)
{}
