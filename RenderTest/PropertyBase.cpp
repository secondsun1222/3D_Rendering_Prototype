#include "PropertyBase.h"

PropertyBase::PropertyBase()
	:type(PropertyType::EMPTY)
{
}

PropertyBase::PropertyBase(PropertyType _type)
	:type(_type)
{
}
