#pragma once
#include "PropertyBase.h"
#include "pch.h"

enum LightType {
	POSITIONAL,
	UNIVERSAL
};

class LightProperty : public Property {
public:
	LightType type;
	double brightness;
	COLORREF color;

public:
	LightProperty(LightType, double);
	LightProperty(LightType, double, COLORREF);
};